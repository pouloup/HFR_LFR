/**
 * Authors: Anthony ARCH - Pierre-Henri DAUVERGNE - Jérémy VIGOUR (ESIR)
**/

#include "HFR2LFRFusionConverter.h"

HFR2LFRFusionConverter::HFR2LFRFusionConverter(const string HFRVideoPath, const string LFRVideoPath, unsigned int framesTubeSize /*= 3*/)
	: HFR2LFRConverter(HFRVideoPath, LFRVideoPath)
{
}


HFR2LFRFusionConverter::~HFR2LFRFusionConverter()
{
}


void HFR2LFRFusionConverter::processFrame()
{
	m_finalFrameBGR = Mat(m_height, m_width, CV_8UC3);

	Mat prevToCurrOpticalFlowMap;
	Mat nextToCurrOpticalFlowMap;

	Mat prevFrameWeightsMap;
	Mat nextFrameWeightsMap;

	GpuMat gpuprevToCurrOpticalFlowMap;
	GpuMat gpunextToCurrOpticalFlowMap;


	calcOpticalFlowFarneback(gpum_prevFrame, gpum_currentFrame, gpuprevToCurrOpticalFlowMap,
		0.5,	// pyramid scale (< 1.0)
		4,		// number of levels in pyramid	(1 : only original frame)
		11,		// window size
		4,		// number of iterations at each pyramid level
		7,		// size of pixel neighborhood (larger values mean that the image will be approximated with smoother surfaces, yielding more robust algorithm and more blurred motion field)
		1.4,	// standard dev
		0);		// flags

	gpuprevToCurrOpticalFlowMap.download(prevToCurrOpticalFlowMap);

	calcOpticalFlowFarneback(gpum_nextFrame, gpum_currentFrame, gpunextToCurrOpticalFlowMap,
		0.5,
		4,
		11,
		4,
		7,
		1.4,
		0);
	gpunextToCurrOpticalFlowMap.download(nextToCurrOpticalFlowMap);

	computeWeightFromOpticalFlow(prevToCurrOpticalFlowMap, prevFrameWeightsMap);
	computeWeightFromOpticalFlow(nextToCurrOpticalFlowMap, nextFrameWeightsMap);

	#pragma omp parallel for
	for(int row = 0; row < m_height; row++)
	{
		for(int col = 0; col < m_width; col++)
		{
			Vec3f finalPixel = 0.0;
			float weights = 0.0;
			// Current pixel
			const Point currentPixel = Point(col, row);

			float prevPixelWeight = prevFrameWeightsMap.at<float>(currentPixel);
			float nextPixelWeight = nextFrameWeightsMap.at<float>(currentPixel);

			finalPixel = m_currentFrameBGR.at<Vec3b>(currentPixel);
			weights += 1.0;

			finalPixel += prevPixelWeight * m_prevFrameBGR.at<Vec3b>(currentPixel);
			weights += prevPixelWeight;

			finalPixel += nextPixelWeight * m_nextFrameBGR.at<Vec3b>(currentPixel);
			weights += nextPixelWeight;

			Vec3b res(finalPixel/ weights);
			m_finalFrameBGR.at<Vec3b>(currentPixel) = res;
		}
	}
}


void HFR2LFRFusionConverter::computeWeightFromOpticalFlow(const Mat & opticalFlowMap, Mat & weightMap)
{
	weightMap = Mat(m_height, m_width, CV_32F);
	float maxWeight = 0.0;

	#pragma omp parallel for
	for(int row = 0; row < m_height; row++)
	{
		for(int col = 0; col < m_width; col++)
		{
			float weight = (float) exp(-2.0/norm(opticalFlowMap.at<Point2f>(row, col)));
			weightMap.at<float>(row, col) = weight;

			if(weight > maxWeight)
				maxWeight = weight;
		}
	}

	#pragma omp parallel for
	for(int row = 0; row < m_height; row++)
	{
		for(int col = 0; col < m_width; col++)
		{
			weightMap.at<float>(row, col) /= 2*maxWeight;
		}
	}
}
