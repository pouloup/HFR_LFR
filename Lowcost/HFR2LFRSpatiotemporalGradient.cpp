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
    GaussianBlur(m_prevFrame, m_prevFrame, Size(3, 3), 0, 0, BORDER_DEFAULT);

    // Compute x sobel gradient
    Mat currentGradientX, nextGradientX, prevGradientX;
    Sobel(m_currentFrame, currentGradientX, CV_16S, 1, 0, 3);
    Sobel(m_nextFrame, nextGradientX, CV_16S, 1, 0, 3);
    Sobel(m_prevFrame, prevGradientX, CV_16S, 1, 0, 3);
    convertScaleAbs(currentGradientX, currentGradientX);
    convertScaleAbs(nextGradientX, nextGradientX);
    convertScaleAbs(prevGradientX, prevGradientX);
    //addWeighted(currentGradientX, 0.5, nextGradientX, 0.5, 0, m_finalFrameBGR);
    //m_finalFrameBGR = (currentGradientX + 2 * nextGradientX - 2 * prevGradientX) / 5 + 127;
    Mat tempPrevX = abs(currentGradientX - prevGradientX) / 2;
    Mat tempNextX = abs(nextGradientX - currentGradientX) / 2;
    //m_finalFrameBGR = prevGradientX;
    //m_finalFrameBGR = (tempNextX + tempPrevX ) * .5;
    //GaussianBlur(tempNextX, tempNextX, Size(9, 9), 0, 0, BORDER_DEFAULT);
    //GaussianBlur(tempPrevX, tempPrevX, Size(9, 9), 0, 0, BORDER_DEFAULT);
    m_finalFrameBGR = tempPrevX + tempNextX;
    GaussianBlur(m_finalFrameBGR, m_finalFrameBGR, Size(9, 9), 0, 0, BORDER_DEFAULT);
    //addWeighted(tempNextX, 0.5, tempNextX, 0.5, 0, m_finalFrameBGR);
    //m_finalFrameBGR += 127;
    //m_finalFrameBGR = (currentGradientX*0) + (nextGradientX/2) - (prevGradientX/2) + 127;

    //m_finalFrameBGR = m_nextFrame - m_currentFrame + 127;
    imshow("TEMP Player", m_finalFrameBGR);
    waitKey(0);
}
