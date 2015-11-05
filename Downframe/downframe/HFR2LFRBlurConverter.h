/**
 * Authors: Anthony ARCH - Pierre-Henri DAUVERGNE - Jérémy VIGOUR (ESIR)
**/

#ifndef HFR2LFR_BLUR_CONVERTER_H
#define HFR2LFR_BLUR_CONVERTER_H

#include "HFR2LFRConverter.h"

class HFR2LFRBlurConverter: public HFR2LFRConverter
{
public:
	HFR2LFRBlurConverter(const string HFRVideoPath, const string LFRVideoPath);
	virtual ~HFR2LFRBlurConverter();

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

#endif // HFR2LFR_BLUR_CONVERTER_H