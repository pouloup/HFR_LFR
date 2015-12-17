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

};

#endif /* __HFR2LFRTEMPORALSOBEL_H__ */
