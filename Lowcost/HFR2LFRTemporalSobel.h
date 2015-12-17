/***************************************************************************
 *
 * TODO: Project Title
 *
 * Author: Florent Guiotte <guiotte.florent@gmail.com>
 *
 **************************************************************************/

#ifndef __HFR2LFRTEMPORALSOBEL_H__
#define __HFR2LFRTEMPORALSOBEL_H__
#include "HFR2LFRConverter.h"

class HFR2LFRTemporalSobel: public HFR2LFRConverter
{
public:
	HFR2LFRTemporalSobel(const string & HFRVideoPath, const string & LFRVideoPath);
	virtual ~HFR2LFRTemporalSobel();

protected:
	/**
	 * Process current frame
	 * Reproduce motion blur through convolutions with blur filters
	 * Convolution is done pixel-by-pixel, with a filter adapted to the movement of the pixel
	**/
	virtual void processFrame();

	/**
	 * Compute norm and angle of each motion vector of the optical flow
	 * opticalFlowMap : Optical flow
	 * opticalFlowNormMap : norms of optical flow vectors
	 * opticalFlowAngleMap : angles of optical flow vectors
	**/
	void computeNormAndAngleOpticalFlow(const Mat & opticalFlowMap, Mat & opticalFlowNormMap, Mat & opticalFlowAngleMap);

	/**
	 * Get motion blur filter for a pixel from the norm and the angle of its motion vector
	**/
	void getMotionBlurFilter(float norm, float angle, Mat & filter);
};

#endif /* __HFR2LFRTEMPORALSOBEL_H__ */
