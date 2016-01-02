/***************************************************************************
 *
 * TODO: Project Title
 *
 * Author: Florent Guiotte <guiotte.florent@gmail.com>
 *
 **************************************************************************/

#include "HFR2LFRSpatiotemporalGradient.h"

HFR2LFRSpatiotemporalGradient::HFR2LFRSpatiotemporalGradient(const string & HFRVideoPath, const string & LFRVideoPath)
	: HFR2LFRConverter(HFRVideoPath, LFRVideoPath)
{
}


HFR2LFRSpatiotemporalGradient::~HFR2LFRSpatiotemporalGradient()
{
}


void HFR2LFRSpatiotemporalGradient::processFrame()
{
    // Reduce the noise (3x3 kernel)
    GaussianBlur(m_nextFrame, m_nextFrame, Size(3, 3), 0, 0, BORDER_DEFAULT);
    GaussianBlur(m_currentFrame, m_currentFrame, Size(3, 3), 0, 0, BORDER_DEFAULT);

    // Compute x sobel gradient
    Mat currentGradientX, nextGradientX;
    Sobel(m_currentFrame, currentGradientX, CV_16S, 1, 0, 3);
    Sobel(m_nextFrame, nextGradientX, CV_16S, 1, 0, 3);
    convertScaleAbs(currentGradientX, currentGradientX);
    convertScaleAbs(nextGradientX, nextGradientX);
    addWeighted(currentGradientX, 0.5, nextGradientX, 0.5, 0, m_finalFrameBGR);
    //m_finalFrameBGR = m_nextFrame - m_currentFrame + 127;
    imshow("TEMP Player", m_finalFrameBGR);
    waitKey(0);
}

