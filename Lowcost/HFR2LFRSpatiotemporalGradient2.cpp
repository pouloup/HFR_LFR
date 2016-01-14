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

    Mat Gx, Gy;
    simpleSTGradient(Gx, Gy);

    addWeighted(Gx, 0.5, Gy, 0.5, 0, m_finalFrameBGR);

    imshow("TEMP Player", m_finalFrameBGR);
    waitKey(0);
}

void HFR2LFRSpatiotemporalGradient2::simpleSTGradient(Mat & Gx, Mat & Gy) const {
    Gx.create(m_height, m_width, CV_8UC1);
    Gy.create(m_height, m_width, CV_8UC1);

    for (int j = 0; j < m_height; j++)
        for (int i = 1; i < m_width - 1; i++)
            Gx.at<uchar>(j,i) = (m_prevFrame.at<uchar>(j,i-1) - m_nextFrame.at<uchar>(j,i+1) + 127) / 2; 

    for (int j = 1; j < m_height - 1; j++)
        for (int i = 0; i < m_width; i++)
            Gy.at<uchar>(j,i) = (m_prevFrame.at<uchar>(j-1,i) - m_nextFrame.at<uchar>(j+1,i) + 127) / 2; 
}
