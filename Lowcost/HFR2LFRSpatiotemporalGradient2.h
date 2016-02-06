/***************************************************************************
 *
 * TODO: Project Title
 *
 * Author: Florent Guiotte <guiotte.florent@gmail.com>
 *
 **************************************************************************/

#ifndef __HFR2LFRSPATIOTEMPORALGRADIENT2_H__
#define __HFR2LFRSPATIOTEMPORALGRADIENT2_H__
#include "HFR2LFRConverter.h"

class HFR2LFRSpatiotemporalGradient2: public HFR2LFRConverter
{
public:
	HFR2LFRSpatiotemporalGradient2(const string & HFRVideoPath, const string & LFRVideoPath);
	virtual ~HFR2LFRSpatiotemporalGradient2();

protected:
	/**
	 * Process current frame
	 * Reproduce motion blur through convolutions with blur filters
	 * Convolution is done pixel-by-pixel, with a filter adapted to the movement of the pixel
	**/
	virtual void processFrame();

private:
    void simpleSTGradient(Mat & Gx, Mat & Gy) const;
    void sobelSTGradient(Mat & Gx, Mat & Gy) const;
    void sobelSTGradient_8UC1(Mat & Gx, Mat & Gy) const;
    void buildTensor(const Mat & Gx, const Mat & Gy, Mat & tensor) const;

    void constructNormDisp(Mat & norm, const Mat & tensor) const;

private:
    cv::Vec2f getNormalDirection(const cv::Vec4f & tensor) const;
    float getNormValue(const cv::Vec4f & tensor) const;
    float getDeltaSqrt(const cv::Vec4f & tensor) const;
};

#endif /* __HFR2LFRSPATIOTEMPORALGRADIENT2_H__ */
