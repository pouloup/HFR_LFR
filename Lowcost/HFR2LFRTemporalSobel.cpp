/***************************************************************************
 *
 * TODO: Project Title
 *
 * Author: Florent Guiotte <guiotte.florent@gmail.com>
 *
 **************************************************************************/

#include "HFR2LFRTemporalSobel.h"

HFR2LFRTemporalSobel::HFR2LFRTemporalSobel(const string & HFRVideoPath, const string & LFRVideoPath)
	: HFR2LFRConverter(HFRVideoPath, LFRVideoPath)
{
}


HFR2LFRTemporalSobel::~HFR2LFRTemporalSobel()
{
}


void HFR2LFRTemporalSobel::processFrame()
{
	m_finalFrameBGR = Mat(m_height, m_width, CV_8UC3);
}
