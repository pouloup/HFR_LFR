/***************************************************************************
 *
 * TODO: Project Title
 *
 * Author: Florent Guiotte <guiotte.florent@gmail.com>
 *
 **************************************************************************/

#include "HFR2LFRTemporalGradient.h"

HFR2LFRTemporalGradient::HFR2LFRTemporalGradient(const string & HFRVideoPath, const string & LFRVideoPath)
	: HFR2LFRConverter(HFRVideoPath, LFRVideoPath)
{
}


HFR2LFRTemporalGradient::~HFR2LFRTemporalGradient()
{
}


void HFR2LFRTemporalGradient::processFrame()
{
    GaussianBlur(m_nextFrame, m_nextFrame, Size(3, 3), 0, 0, BORDER_DEFAULT);
    GaussianBlur(m_currentFrame, m_currentFrame, Size(3, 3), 0, 0, BORDER_DEFAULT);
	m_finalFrameBGR = Mat(m_height, m_width, CV_8UC3);
    m_finalFrameBGR = m_nextFrame - m_currentFrame + 127;
    imshow("TEMP Player", m_finalFrameBGR);
    waitKey(0);
}
