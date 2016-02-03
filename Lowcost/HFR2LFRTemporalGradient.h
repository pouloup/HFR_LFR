/***************************************************************************
 *
 * TODO: Project Title
 *
 * Author: Florent Guiotte <guiotte.florent@gmail.com>
 *
 **************************************************************************/

#ifndef __HFR2LFRTEMPORALGRADIENT_H__
#define __HFR2LFRTEMPORALGRADIENT_H__
#include "HFR2LFRConverter.h"

class HFR2LFRTemporalGradient: public HFR2LFRConverter
{
public:
	HFR2LFRTemporalGradient(const string & HFRVideoPath, const string & LFRVideoPath);
	virtual ~HFR2LFRTemporalGradient();

protected:
	/**
	 * Process current frame
	 * Reproduce motion blur through convolutions with blur filters
	 * Convolution is done pixel-by-pixel, with a filter adapted to the movement of the pixel
	**/
	virtual void processFrame();

};

#endif /* __HFR2LFRTEMPORALGRADIENT_H__ */
