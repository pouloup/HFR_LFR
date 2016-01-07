/***************************************************************************
 *
 * TODO: Project Title
 *
 * Author: Florent Guiotte <guiotte.florent@gmail.com>
 *
 **************************************************************************/

#ifndef __HFR2LFRSPATIOTEMPORALGRADIENT2_H__
#define __HFR2LFRSPATIOTEMPORALGRADIENT2_H__
#include "HFR2LFRConverter.h"

class HFR2LFRSpationtemporalGradient2: public HFR2LFRConverter
{
public:
	HFR2LFRSpationtemporalGradient2(const string & HFRVideoPath, const string & LFRVideoPath);
	virtual ~HFR2LFRSpationtemporalGradient2();

protected:
	/**
	 * Process current frame
	 * Reproduce motion blur through convolutions with blur filters
	 * Convolution is done pixel-by-pixel, with a filter adapted to the movement of the pixel
	**/
	virtual void processFrame();

};

#endif /* __HFR2LFRSPATIOTEMPORALGRADIENT2_H__ */
