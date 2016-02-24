/**
 * Authors: Anthony ARCH - Pierre-Henri DAUVERGNE - Jy VIGOUR (ESIR)
**/

#include "HFR2LFRBlurConverter.h"

HFR2LFRBlurConverter::HFR2LFRBlurConverter(const string HFRVideoPath, const string LFRVideoPath)
	: HFR2LFRConverter(HFRVideoPath, LFRVideoPath)
{
}


HFR2LFRBlurConverter::~HFR2LFRBlurConverter()
{
}


void HFR2LFRBlurConverter::processFrame()
{
	m_finalFrameBGR = Mat(m_height, m_width, CV_8UC3);

	Mat opticalFlowMap;

	Mat opticalFlowNormMap;
	Mat opticalFlowNormMapMedian;

	Mat opticalFlowAngleMap;
	Mat opticalFlowAngleMapMedian;

	GpuMat gpuopticalFlowMap;

	GpuMat gpuopticalFlowNormMap;
	GpuMat gpuopticalFlowNormMapMedian;

	GpuMat gpuopticalFlowAngleMap;
	GpuMat gpuopticalFlowAngleMapMedian;

	// Flot optique
	calcOpticalFlowFarneback(gpum_currentFrame, gpum_nextFrame, gpuopticalFlowMap,
		0.5,	// pyramid scale (< 1.0)
		4,		// number of levels in pyramid	(1 : only original frame)
		11,		// window size
		4,		// number of iterations at each pyramid level
		7,		// size of pixel neighborhood (larger values mean that the image will be approximated with smoother surfaces, yielding more robust algorithm and more blurred motion field)
		1.4,	// standard dev
		0);		// flags


	gpuopticalFlowMap.download(opticalFlowMap);
	// Get angle and norm of each motion vector
	computeNormAndAngleOpticalFlow(opticalFlowMap, opticalFlowNormMap, opticalFlowAngleMap);

	gpuopticalFlowNormMap.upload(opticalFlowNormMap);
	gpuopticalFlowAngleMap.upload(opticalFlowAngleMap);

	medianBlur(gpuopticalFlowNormMap, gpuopticalFlowNormMapMedian, 5);
	medianBlur(gpuopticalFlowAngleMap, gpuopticalFlowAngleMapMedian, 5);

	gpuopticalFlowNormMapMedian.download(opticalFlowNormMapMedian);
	gpuopticalFlowAngleMapMedian.download(opticalFlowAngleMapMedian);

	// Pour chaque pixel de la frame
#pragma omp parallel for
	for(int pixelRow = 0 ; pixelRow < m_height ; pixelRow++)
	{
		for(int pixelCol = 0 ; pixelCol < m_width ; pixelCol++)
		{
			// Pas de blur pour les pixels avec peu ou pas de mouvement
			if(opticalFlowNormMap.at<float>(pixelRow, pixelCol) < 4.0)
			{
				m_finalFrameBGR.at<Vec3b>(pixelRow, pixelCol) = m_currentFrameBGR.at<Vec3b>(pixelRow, pixelCol);
			}
			else
			{
				Mat filter;
				getMotionBlurFilter(
					opticalFlowNormMap.at<float>(pixelRow, pixelCol),
					opticalFlowAngleMap.at<float>(pixelRow, pixelCol),
					filter);

				// Calculer le filtre gaussien � y appliquer (params d閜endent du flot optique)
				int shiftRow = pixelRow - filter.rows/2;	// passage entre rep鑢e image et rep鑢e filtre
				int shiftCol = pixelCol - filter.cols/2;

				// Gestion des bords
				int rowMin = std::max(shiftRow, 0);
				int rowMax = std::min(shiftRow + filter.rows, m_height);
				int colMin = std::max(shiftCol, 0);
				int colMax = std::min(shiftCol + filter.cols, m_width);

				// Application du filtre
				float weight;
				Vec3f finalValue;
				float sumWeights = 0.0;

				for(int row = rowMin ; row < rowMax ; row++)
				{
					for(int col = colMin ; col < colMax ; col++)
					{
						weight = filter.at<float>(row - shiftRow, col - shiftCol);
						sumWeights += weight;

						finalValue[0] += m_currentFrameBGR.at<Vec3b>(row, col)[0] * weight;
						finalValue[1] += m_currentFrameBGR.at<Vec3b>(row, col)[1] * weight;
						finalValue[2] += m_currentFrameBGR.at<Vec3b>(row, col)[2] * weight;
					}
				}
				m_finalFrameBGR.at<Vec3b>(pixelRow, pixelCol) = (Vec3b) finalValue / sumWeights;
			}
		}
	}
}


void HFR2LFRBlurConverter::computeNormAndAngleOpticalFlow(const Mat & opticalFlowMap, Mat & opticalFlowNormMap, Mat & opticalFlowAngleMap)
{
	opticalFlowNormMap = Mat(m_height, m_width, CV_32F);
	opticalFlowAngleMap = Mat(m_height, m_width, CV_32F);

#pragma omp parallel for
	for(int row = 0; row < m_height; row++)
	{
		for(int col = 0; col < m_width; col++)
		{
			Point2f vecteur = opticalFlowMap.at<Point2f>(row, col);

			float angle = 0.0;
			if(abs(vecteur.x) < 0.001)
			{
				angle = M_PI/2;
			}
			else
			{
				angle = atan(vecteur.y/vecteur.x);
			}

			opticalFlowNormMap.at<float>(row, col) = (float) norm(vecteur);
			opticalFlowAngleMap.at<float>(row, col) = angle;
		}
	}
}


void HFR2LFRBlurConverter::getMotionBlurFilter(float norm, float angle, Mat & filter)
{
	float length = std::max(1.0f, norm);
	float half = (length-1)/2;
	float phi = fmod(angle, M_PI);

	float cosPhi = cos(phi);
	float sinPhi = sin(phi);
	int xsign = (cosPhi > 0.0) ? 1 : -1;
	float lineWidth = 1.0;

	int sCol = 1 + abs((int) (half*cosPhi + xsign*(lineWidth-length*EPS)));
	int sRow = 1 + abs((int) (half*sinPhi + lineWidth -length*EPS));
	Mat mX(sRow, sCol, CV_32F);
	Mat mY(sRow, sCol, CV_32F);

	for(int row = 0 ; row < sRow ; row++)
	{
		for(int col = 0 ; col < sCol ; col++)
		{
			mX.at<int>(row, col) = xsign*col;
			mY.at<int>(row, col) = row;
		}
	}

	Mat h = lineWidth + EPS - abs(cosPhi*mY - sinPhi*mX);

	Mat hFlip;
	flip(h, hFlip, -1);

	filter = Mat::zeros(2*sRow-1, 2*sCol-1, CV_32F);
	for(int row = 0; row < sRow; row++)
	{
		for(int col = 0; col < sCol; col++)
		{
			filter.at<float>(row, col) = hFlip.at<float>(row, col);
			filter.at<float>(row + sRow-1, col + sCol-1) = h.at<float>(row, col);
		}
	}

	filter /= sum(filter).val[0];

	if(cosPhi > 0)
	{
		flip(filter, filter, 0);
	}
}
