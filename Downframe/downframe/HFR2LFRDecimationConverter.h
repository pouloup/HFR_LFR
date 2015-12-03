/*
 * Author : Paul LE DENN (ESIR)
 */

#pragma once

#include "HFR2LFRConverter.h"

class HFR2LFRDecimationConverter : public HFR2LFRConverter
{
public:
	HFR2LFRDecimationConverter(const string HFRVideoPath, const string LFRVideoPath);
	virtual ~HFR2LFRDecimationConverter();

protected:
	/*
	 * Process current frame
	 * Either delete 1 frame or save it.
	 * Deleting 1/2 frame allows to make a low quality downsample
	 */
	virtual void processFrame();

private:
	unsigned long long int cpt;
};

