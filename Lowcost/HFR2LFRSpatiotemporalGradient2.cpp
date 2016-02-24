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
    //sobelSTGradient(Gx, Gy);
    sobelSTGradient2(Gx, Gy);


    Mat tensor;
    buildTensor(Gx, Gy, tensor);

    //constructNormDisp(m_finalFrameBGR, tensor);
    //constructNormalDirDisp(m_finalFrameBGR, tensor);
    constructNormalDirDispBGR(m_finalFrameBGR, tensor);
    //normalize(Gx, Gx, 0, 255, NORM_MINMAX, CV_8UC1);
    //normalize(Gy, Gy, 0, 255, NORM_MINMAX, CV_8UC1);
    //addWeighted(Gx, 0, Gy, 1, 0, m_finalFrameBGR);
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

void HFR2LFRSpatiotemporalGradient2::sobelSTGradient2_8UC1(Mat & Gx, Mat & Gy) const {
    Mat Gx_f, Gy_f;
    sobelSTGradient2(Gx_f, Gy_f);
    normalize(Gx_f, Gx, 0, 255, NORM_MINMAX, CV_8UC1);
    normalize(Gy_f, Gy, 0, 255, NORM_MINMAX, CV_8UC1);
}

void HFR2LFRSpatiotemporalGradient2::sobelSTGradient_8UC1(Mat & Gx, Mat & Gy) const {
    Mat Gx_f, Gy_f;
    sobelSTGradient(Gx_f, Gy_f);
    normalize(Gx_f, Gx, 0, 255, NORM_MINMAX, CV_8UC1);
    normalize(Gy_f, Gy, 0, 255, NORM_MINMAX, CV_8UC1);
}

void HFR2LFRSpatiotemporalGradient2::sobelSTGradient2(Mat & Gx, Mat & Gy) const {
    Gx.create(m_height, m_width, CV_32FC1);
    Gy.create(m_height, m_width, CV_32FC1);

    Mat Gx_h(m_height, m_width, CV_32FC1);

    for (int j = 0; j < m_height; j++)
        for (int i = 1; i < m_width - 1; i++)
            Gx_h.at<float>(j,i) = m_prevFrame.at<uchar>(j,i-0) - m_nextFrame.at<uchar>(j,i+0);

    Mat Gx_v(m_height, m_width, CV_32FC1);

    for (int j = 1; j < m_height - 1; j++)
        for (int i = 1; i < m_width - 1; i++)
            Gx.at<float>(j,i) = 1 * Gx_h.at<float>(j-1, i) + 2 * Gx_h.at<float>(j, i) + 1 * Gx_h.at<float>(j+1, i);

    Mat Gy_h(m_height, m_width, CV_32FC1);

    for (int j = 1; j < m_height - 1; j++)
        for (int i = 0; i < m_width; i++)
            Gy_h.at<float>(j,i) = m_prevFrame.at<uchar>(j-0, i) - m_nextFrame.at<uchar>(j+0, i);

    Mat Gy_v(m_height, m_width, CV_32FC1);

    for (int j = 1; j < m_height - 1; j++)
        for (int i = 1; i < m_width - 1; i++)
            Gy.at<float>(j,i) = 1 * Gy_h.at<float>(j, i-1) + 2 * Gy_h.at<float>(j, i) + 1 * Gy_h.at<float>(j, i+1);

    //Mat grad_x, grad_y;

    //// Gradient X
    //Sobel( m_currentFrame, grad_x, CV_32FC1, 1, 0);//, 3, scale, delta, BORDER_DEFAULT );
    //// Gradient Y
    //Sobel( m_currentFrame, grad_y, CV_32FC1, 0, 1);//, 3, scale, delta, BORDER_DEFAULT );

    //Gy -= grad_y ;
    //Gx -= grad_x ;
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

    //Mat grad_x, grad_y;

    //// Gradient X
    //Sobel( m_currentFrame, grad_x, CV_32FC1, 1, 0);//, 3, scale, delta, BORDER_DEFAULT );
    //// Gradient Y
    //Sobel( m_currentFrame, grad_y, CV_32FC1, 0, 1);//, 3, scale, delta, BORDER_DEFAULT );

    //Gy -= grad_y ;
    //Gx -= grad_x ;
}

void HFR2LFRSpatiotemporalGradient2::buildTensor(const Mat & Gx, const Mat & Gy, Mat & tensor) const {
    tensor.create(m_height, m_width, CV_32FC4);

    MatConstIterator_<float> it = Gx.begin<float>();
    MatConstIterator_<float> jt = Gy.begin<float>();
    MatIterator_<Vec4f> tt = tensor.begin<Vec4f>();

    for (; it != Gx.end<float>(); it++, jt++, tt++) {
        Vec4f cell;
        // Gx²
        cell[0] = *it * *it;
    
        // GxGy
        cell[1] = *it * *jt;
        cell[2] = *it * *jt;

        // Gy²
        cell[3] = *jt * *jt;

        *tt = cell;
    }
}

void HFR2LFRSpatiotemporalGradient2::constructNormDisp(Mat & norm, const Mat & tensor) const {
    Mat normf(m_height, m_width, CV_32FC1);

    MatConstIterator_<Vec4f> tt = tensor.begin<Vec4f>();
    MatIterator_<float> nt = normf.begin<float>();

    for (; tt != tensor.end<Vec4f>(); tt++, nt++)
        *nt = getNormValue(*tt);

    normalize(normf, norm, 0, 255, NORM_MINMAX, CV_8UC1);
}

void HFR2LFRSpatiotemporalGradient2::constructNormalDirDisp(Mat & normal, const Mat & tensor) const {
    normal.create(m_height, m_width, CV_8UC1);

    MatConstIterator_<Vec4f> tt = tensor.begin<Vec4f>();
    MatIterator_<unsigned char> nt = normal.begin<unsigned char>();

    for (; tt != tensor.end<Vec4f>(); tt++, nt++) {
        Vec2f ndir = getNormalDirection(*tt);
        float rdir = atan2(ndir[0], ndir[1]);
        *nt = 127 + rdir * 40; 
    }
}

void HFR2LFRSpatiotemporalGradient2::constructNormalDirDispBGR(Mat & normalBGR, const Mat & tensor) const {
    //normal.create(m_height, m_width, CV_8UC1);
    Mat normalHSV(m_height, m_width, CV_8UC3);

    MatConstIterator_<Vec4f> tt = tensor.begin<Vec4f>();
    MatIterator_<Vec3b> nt = normalHSV.begin<Vec3b>();

    for (; tt != tensor.end<Vec4f>(); tt++, nt++) {
        Vec2f ndir = getNormalDirection(*tt);
        float rdir = atan2(ndir[0], ndir[1]);
        (*nt)[0] = (180 + rdir * 180 / CV_PI) / 1; 
        (*nt)[1] = 255; 
        (*nt)[2] = getNormValue(*tt) * 255; 
        //cout << *nt << " ";
    }

    cvtColor(normalHSV, normalBGR, CV_HSV2BGR);
}

void HFR2LFRSpatiotemporalGradient2::constructMotionBlur(Mat & finalFrameBGR, const Mat & tensor) const {
    //normal.create(m_height, m_width, CV_8UC1);
    Mat normalHSV(m_height, m_width, CV_8UC3);

    MatConstIterator_<Vec4f> tt = tensor.begin<Vec4f>();
    MatIterator_<Vec3b> nt = normalHSV.begin<Vec3b>();

    for (; tt != tensor.end<Vec4f>(); tt++, nt++) {
        Vec2f ndir = getNormalDirection(*tt);
        float rdir = atan2(ndir[0], ndir[1]);
        (*nt)[0] = (180 + rdir * 180 / CV_PI) / 1; 
        (*nt)[1] = 255; 
        (*nt)[2] = getNormValue(*tt) * 255; 
        //cout << *nt << " ";
    }

    //cvtColor(normalHSV, normalBGR, CV_HSV2BGR);
}

Vec2f HFR2LFRSpatiotemporalGradient2::getNormalDirection(const Vec4f & tensor) const {
    float a = 2 * tensor[1];
    float b = tensor[3] - tensor[0] + getDeltaSqrt(tensor);

    return Vec2f(a, b);
}

float HFR2LFRSpatiotemporalGradient2::getNormValue(const Vec4f & tensor) const {
    double wtf = tensor[0] + tensor[3];
    double d = getDeltaSqrt(tensor);
    double a = (wtf + d) / 2.0f;
    double b = (wtf - d) / 2.0f;
    //double a = (tensor[0] + tensor[3] + getDeltaSqrt(tensor)) / 2.0f;
    //double b = (tensor[0] + tensor[3] - getDeltaSqrt(tensor)) / 2.0f;

    if ( a + b == 0 ) return 0;

    double c = (a - b) / (a + b);
    //cout << wtf << ":" << d << " ";
    //cout << a << ":" << b << " ";
    return c * c;
}

float HFR2LFRSpatiotemporalGradient2::getDeltaSqrt(const Vec4f & tensor) const {
    double a = tensor[0] - tensor[3];
    double b = tensor[1] * tensor[1];
    // TODO: tweak computation
    return sqrt(a * a  + 4 * b);
}
