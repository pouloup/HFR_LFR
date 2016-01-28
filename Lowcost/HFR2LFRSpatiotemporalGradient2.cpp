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
    //simpleSTGradient(Gx, Gy);
    sobelSTGradient(Gx, Gy);


    Mat tensor;
    buildTensor(Gx, Gy, tensor);

    normalize(Gx, Gx, 0, 255, NORM_MINMAX, CV_8UC1);
    normalize(Gy, Gy, 0, 255, NORM_MINMAX, CV_8UC1);
    addWeighted(Gx, 0, Gy, 1, 0, m_finalFrameBGR);
    imshow("TEMP Player", m_finalFrameBGR);
    waitKey(1);
}

void HFR2LFRSpatiotemporalGradient2::simpleSTGradient(Mat & Gx, Mat & Gy) const {
    Gx.create(m_height, m_width, CV_8UC1);
    Gy.create(m_height, m_width, CV_8UC1);

    for (int j = 0; j < m_height; j++)
        for (int i = 1; i < m_width - 1; i++)
            Gx.at<uchar>(j,i) = (m_prevFrame.at<uchar>(j,i-1) - m_nextFrame.at<uchar>(j,i+1) + 127) / 2;  // TODO: prevent overflow!

    for (int j = 1; j < m_height - 1; j++)
        for (int i = 0; i < m_width; i++)
            Gy.at<uchar>(j,i) = (m_prevFrame.at<uchar>(j-1,i) - m_nextFrame.at<uchar>(j+1,i) + 127) / 2; 
}

void HFR2LFRSpatiotemporalGradient2::sobelSTGradient_8UC1(Mat & Gx, Mat & Gy) const {
    Mat Gx_f, Gy_f;
    sobelSTGradient(Gx_f, Gy_f);
    normalize(Gx_f, Gx, 0, 255, NORM_MINMAX, CV_8UC1);
    normalize(Gy_f, Gy, 0, 255, NORM_MINMAX, CV_8UC1);
}

void HFR2LFRSpatiotemporalGradient2::sobelSTGradient(Mat & Gx, Mat & Gy) const {
    Gx.create(m_height, m_width, CV_32FC1);
    Gy.create(m_height, m_width, CV_32FC1);

    Mat Gx_h(m_height, m_width, CV_32FC1);

    for (int j = 0; j < m_height; j++)
        for (int i = 1; i < m_width - 1; i++)
            Gx_h.at<float>(j,i) = m_prevFrame.at<uchar>(j,i-1) - m_nextFrame.at<uchar>(j,i+1);

    Mat Gx_v(m_height, m_width, CV_32FC1);

    for (int j = 1; j < m_height - 1; j++)
        for (int i = 1; i < m_width - 1; i++)
            Gx.at<float>(j,i) = 1 * Gx_h.at<float>(j-1, i) + 2 * Gx_h.at<float>(j, i) + 1 * Gx_h.at<float>(j+1, i);

    Mat Gy_h(m_height, m_width, CV_32FC1);

    for (int j = 1; j < m_height - 1; j++)
        for (int i = 0; i < m_width; i++)
            Gy_h.at<float>(j,i) = m_prevFrame.at<uchar>(j-1, i) - m_nextFrame.at<uchar>(j+1, i);

    Mat Gy_v(m_height, m_width, CV_32FC1);

    for (int j = 1; j < m_height - 1; j++)
        for (int i = 1; i < m_width - 1; i++)
            Gy.at<float>(j,i) = 1 * Gy_h.at<float>(j, i-1) + 2 * Gy_h.at<float>(j, i) + 1 * Gy_h.at<float>(j, i+1);
}

void HFR2LFRSpatiotemporalGradient2::buildTensor(const Mat & Gx, const Mat & Gy, Mat & tensor) const {
    tensor.create(m_height * 2, m_width * 2, CV_8UC1);

    // Gx^2
    Rect roi(0, 0, Gx.cols, Gx.rows);
    Mat roiTmp(tensor, roi);

    Mat Gx2;
    pow(Gx, 2, Gx2);
    Gx2.copyTo(roiTmp);

    // Gy^2
    Rect roi2(m_width, m_height, m_width, m_height);
    Mat roiTmp2 = tensor(roi2);

    Mat Gy2;
    pow(Gy, 2, Gy2);
    Gy.copyTo(roiTmp2);

    // GxGy
    Rect roi3(m_width, 0, m_width, m_height);
    Mat roiTmp3(tensor, roi3);

    Mat GxGy(Gx.t() * Gy);
    GxGy.copyTo(roiTmp3);

    // GyGx
    Rect roi4(0, m_height, m_width, m_height);
    Mat roiTmp4(tensor, roi4);

    Mat GyGx(Gy.t() * Gx);
    GyGx.copyTo(roiTmp4);
}
