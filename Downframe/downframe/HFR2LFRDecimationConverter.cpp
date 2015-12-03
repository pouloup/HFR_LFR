#include "HFR2LFRDecimationConverter.h"



HFR2LFRDecimationConverter::HFR2LFRDecimationConverter(const string HFRVideoPath, const string LFRVideoPath)
	: HFR2LFRConverter(HFRVideoPath, LFRVideoPath)
{
	cpt = 0;
}


HFR2LFRDecimationConverter::~HFR2LFRDecimationConverter()
{
}

/*
 * Naievely keep even frames.
 */
void HFR2LFRDecimationConverter::processFrame()
{
	if (cpt % 2) {
		m_finalFrameBGR = m_currentFrameBGR.clone();
		cpt++;
	}
	else {
		cpt++;
	}
}
