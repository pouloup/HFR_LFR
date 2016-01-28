/**
 * Authors: Anthony ARCH - Pierre-Henri DAUVERGNE - Jérémy VIGOUR (ESIR)
**/

#include "HFR2LFRConverter.h"
#include "HFR2LFRYuvReader.h"

void displayError(const std::string & msg)
{
	std::cerr << "[ERR ] " << msg << endl;
}


void displayInfo(const std::string & msg)
{
	std::cout << "[INFO] " << msg << endl;
}


HFR2LFRConverter::HFR2LFRConverter(const string HFRVideoPath, const string LFRVideoPath)
	: m_HFRVideoPath(HFRVideoPath), m_LFRVideoPath(LFRVideoPath), m_initialized(false)
{
}


HFR2LFRConverter::~HFR2LFRConverter()
{
}


bool HFR2LFRConverter::initialize()
{
	HFR2LFRYuvReader stream(m_HFRVideoPath.c_str(),1920, 1080);
	Mat firstFrame;

	if(!stream.read(firstFrame))
	{
		displayError("Cannot read video, initialization failed!");
		m_initialized = false;
	}
	else // YUV is RAW VIDEO = NO HEADER
	{
		m_width = 1920; //(int)stream.get(CV_CAP_PROP_FRAME_WIDTH);
		m_height = 1080; //(int)stream.get(CV_CAP_PROP_FRAME_HEIGHT);
		m_fps = 120;//(int) stream.get(CV_CAP_PROP_FPS); // unreliable
		m_fourCCCode = CV_FOURCC('I', '4', '2', '0'); //stream.get(CV_CAP_PROP_FOURCC); // unreliable
		int frameCount = -1; //(int) stream.get(CV_CAP_PROP_FRAME_COUNT);

		displayInfo("Properties: ");
		cout << "  " << m_width << "x" << m_height << endl;
		cout << "  " << m_fps << " fps" << endl;
		cout << "  " << frameCount << " frames" << endl;
		m_initialized = true;
	}

	return m_initialized;
}


int HFR2LFRConverter::convert(unsigned int nbFrames, unsigned int startFrame)
{
	if(!m_initialized)
	{
		displayError("Converter not initialized or initialization has failed!");
		return ERR_INIT;
	}

	unsigned int cpt = 1;

	//VideoCapture stream(m_HFRVideoPath);
	HFR2LFRYuvReader stream(m_HFRVideoPath.c_str(), 1920, 1080);

	VideoWriter writer;
	writer.open(m_LFRVideoPath, m_fourCCCode, 60/*m_fps*/, Size(m_width, m_height), 1);

	// Aller à l'image demandée
	stream.set(CV_CAP_PROP_POS_FRAMES, max((int)startFrame-1, 0));

	// Récupérer 3 frames successives
	if (!stream.read(m_prevFrameBGR) || !stream.read(m_currentFrameBGR) || !stream.read(m_nextFrameBGR))
	{
		displayError("No more frame to read");
		return ERR_NO_MORE_FRAME;
	}
	
	// Convertir images en niveaux de gris
	cvtColor(m_prevFrameBGR,    m_prevFrame,    CV_BGR2GRAY);
	cvtColor(m_currentFrameBGR, m_currentFrame, CV_BGR2GRAY);
	cvtColor(m_nextFrameBGR,    m_nextFrame,    CV_BGR2GRAY);

	while(nbFrames == 0 || cpt <= nbFrames)
	{
		ostringstream msg;
		msg << "Processing frame " << cpt;
		displayInfo(msg.str());

		// Créer la frame finale à partir des 3 frames HFR
		processFrame();

		// Enregistrer la frame finale
		writer.write(m_finalFrameBGR);
		cpt++;

		// La frame suivante (n+1) devient la frame précédente
		m_prevFrameBGR = m_nextFrameBGR.clone();
		m_prevFrame = m_nextFrame.clone();

		// Récupérer la frame principale (n+2) et la frame suivante  (n+3)
		if (!stream.read(m_currentFrameBGR) || !stream.read(m_nextFrameBGR))
		{
			displayError("No more frame to read");
			return ERR_NO_MORE_FRAME;
		}

		cvtColor(m_currentFrameBGR, m_currentFrame, CV_BGR2GRAY);
		cvtColor(m_nextFrameBGR, m_nextFrame, CV_BGR2GRAY);
	}

	return 0;
}


void HFR2LFRConverter::drawOpticalFlowMap(Mat & frame, const Mat & opticalFlow, int step, const Scalar & color)
{
	for(int row = 0; row < frame.rows; row += step)
	{
		for(int col = 0; col < frame.cols; col += step)
		{
			const Point2f & motionVector = opticalFlow.at<Point2f>(row, col);
			if(motionVector.x != -1.0 && motionVector.y != -1.0)
			{
				line(frame,   Point(col, row), Point(cvRound(col + motionVector.x), cvRound(row + motionVector.y)), color);
				circle(frame, Point(cvRound(col + motionVector.x), cvRound(row + motionVector.y)), 1, color, -1);
			}
		}
	}
}
