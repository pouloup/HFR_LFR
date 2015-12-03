/**
 * Authors: Anthony ARCH - Pierre-Henri DAUVERGNE - Jérémy VIGOUR (ESIR)
**/

#ifndef HFR2LFR_CONVERTER_H
#define HFR2LFR_CONVERTER_H

#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/video/tracking.hpp>

#include "Utils.h"

using namespace cv;
using namespace std;

#define ERR_INIT 1
#define ERR_NO_MORE_FRAME 2

void displayError(const std::string & msg);
void displayInfo(const std::string & msg);

class HFR2LFRConverter
{
public:
	HFR2LFRConverter(const string HFRVideoPath, const string LFRVideoPath);
	virtual ~HFR2LFRConverter();

	/**
	 * Try to open and read the first frame of the HFR video to initialize converter
	 * If initialization fails, video cannot be converted
	 * @return true if successful initialization, false otherwise
	**/
	virtual bool initialize();

	/**
	 * Convert HFR video to LFR version
	 * Converter must be initialized before conversion!
	 * @param nbFrames: number of LFR frames to generate (all by default)
	 * @param startFrame: start conversion from specified frame (first one by default)
	 * @return 0 if successful conversion, error code otherwise
	**/
	virtual int convert(unsigned int nbFrames = 0, unsigned int startFrame = 0);

protected:
	/**
	 * Process current frame
	 * This method contains every steps to convert the current HFR frame into a LFR frame
	**/
	virtual void processFrame() = 0;

	/**
	 * Draw motion vectors in a frame from the associated optical flow matrix
	 * @param frame: frame where motion vectors are added
	 * @param opticalFlowMap: computed flow matrix of the frame
	 * @param step: motion vector is draw every step pixel
	 * @param color: color of motion vectors
	**/
	virtual void drawOpticalFlowMap(Mat & frame, const Mat & opticalFlowMap, int step, const Scalar & color);

protected:
	string m_HFRVideoPath;
	string m_LFRVideoPath;

	bool m_initialized;

	int m_width;
	int m_height;
	int m_fps;
	int m_fourCCCode;

	Mat m_prevFrameBGR;
	Mat m_prevFrame;

	Mat m_currentFrameBGR;
	Mat m_currentFrame;

	Mat m_nextFrameBGR;
	Mat m_nextFrame;

	Mat m_finalFrameBGR;
};

#endif // HFR2LFR_CONVERTER_H
