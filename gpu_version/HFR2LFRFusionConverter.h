/**
 * Authors: Anthony ARCH - Pierre-Henri DAUVERGNE - Jérémy VIGOUR (ESIR)
**/

#ifndef HFR2LFR_FUSION_CONVERTER_H
#define HFR2LFR_FUSION_CONVERTER_H

#include "HFR2LFRConverter.h"

/*
	.at<...>(row, col)
	.at<...>(Point(col, row)) !!
	x = col, y = row
*/

class HFR2LFRFusionConverter: public HFR2LFRConverter
{
public:
	HFR2LFRFusionConverter(const string HFRVideoPath, const string LFRVideoPath, unsigned int framesTubeSize = 3);
	~HFR2LFRFusionConverter();

protected:
	/**
	 * Process current frame
	 * Reproduce motion blur through a linear combination of 3 frames
	 * Combination is done pixel-by-pixel, relative to the optical flow
	**/
	virtual void processFrame();

	/**
	 * Compute weights for each pixel according to the matching optical flow
	 * The faster the pixel, the bigger the weight
	**/
	void computeWeightFromOpticalFlow(const Mat & opticalFlowMap, Mat & weightMap);
};

#endif // HFR2LFR_FUSION_CONVERTER_H
