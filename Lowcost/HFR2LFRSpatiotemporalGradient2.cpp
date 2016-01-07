/***************************************************************************
 *
 * TODO: Project Title
 *
 * Author: Florent Guiotte <guiotte.florent@gmail.com>
 *
 **************************************************************************/

#include "HFR2LFRSpatiotemporalGradient2.h"

HFR2LFRSpatiotemporalGradient2::HFR2LFRSpatiotemporalGradient2(const string & HFRVideoPath, const string & LFRVideoPath)
	: HFR2LFRConverter(HFRVideoPath, LFRVideoPath)
{
}


HFR2LFRSpatiotemporalGradient2::~HFR2LFRSpatiotemporalGradient2()
{
}


void HFR2LFRSpatiotemporalGradient2::processFrame()
{
    // Reduce the noise (3x3 kernel)
    GaussianBlur(m_nextFrame, m_nextFrame, Size(3, 3), 0, 0, BORDER_DEFAULT);
    GaussianBlur(m_currentFrame, m_currentFrame, Size(3, 3), 0, 0, BORDER_DEFAULT);
    GaussianBlur(m_prevFrame, m_prevFrame, Size(3, 3), 0, 0, BORDER_DEFAULT);

    m_finalFrameBGR = m_currentFrame;

    imshow("TEMP Player", m_finalFrameBGR);
    waitKey(0);
}
