#include "HFR2LFRYuv.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

enum YUV_ReturnValue
YUV_init(FILE *fin, size_t w, size_t h, struct YUV_Capture *out)
{
    if (!fin || w % 2 == 1 || h % 2 == 1)
        return YUV_PARAMETER_ERROR;

    out->fin = fin;
    out->width = w;
    out->height = h;
	
	out->ycrcb = cv::Mat(cv::Size(w, h), CV_8UC3);
	out->y = cv::Mat(cv::Size(w, h), CV_8UC1);
	out->cb = cv::Mat(cv::Size(w, h), CV_8UC1);
	out->cr = cv::Mat(cv::Size(w, h), CV_8UC1);
	out->cb_half = cv::Mat(cv::Size(w / 2, h / 2), CV_8UC1);
	out->cr_half = cv::Mat(cv::Size(w / 2, h / 2), CV_8UC1);

    if
    (
       out->ycrcb.empty()
       ||
       out->y.empty()
       ||
       out->cb.empty()
       ||
       out->cr.empty()
       ||
       out->cb_half.empty()
       ||
       out->cr_half.empty()
    )
    {
        YUV_cleanup(out);
        return YUV_OUT_OF_MEMORY;
    }

    return YUV_OK;
}

enum YUV_ReturnValue
YUV_read(struct YUV_Capture *cap)
{
    size_t bytes_read;
    size_t npixels;

    npixels = cap->width*cap->height;
    bytes_read = fread(cap->y.data, sizeof(uint8_t), npixels, cap->fin);
    if (bytes_read == 0)
        return YUV_EOF;
    else if (bytes_read != npixels)
        return YUV_IO_ERROR;
    bytes_read = fread(cap->cb_half.data, sizeof(uint8_t), npixels/4, cap->fin);
    if (bytes_read != npixels/4)
        return YUV_IO_ERROR;

    bytes_read = fread(cap->cr_half.data, sizeof(uint8_t), npixels/4, cap->fin);
    if (bytes_read != npixels/4)
        return YUV_IO_ERROR;
	
    cv::resize(cap->cb_half, cap->cb, cap->cb.size(), 0, 0, CV_INTER_CUBIC);
	cv::resize(cap->cr_half, cap->cr, cap->cr.size(), 0, 0, CV_INTER_CUBIC);

	cv::Mat vec[3] = { cap->y, cap->cr, cap->cb };
	cv::merge(vec, 3, cap->ycrcb);
	

	/*cvResize(cap->cb_half, cap->cb, CV_INTER_CUBIC);
	cvResize(cap->cr_half, cap->cr, CV_INTER_CUBIC);
	cvMerge(cap-y, cap->cr, cap->cb, NULL, cap->ycrcb);*/

    return YUV_OK;
}

void
YUV_cleanup(struct YUV_Capture *cap)
{
    if (!cap)
        return;

    if (cap->ycrcb.data)
        cap->ycrcb.deallocate();
    if (cap->y.data)
        cap->y.deallocate();
    if (cap->cb.data)
        cap->cb.deallocate();
    if (cap->cr.data)
        cap->cr.deallocate();
    if (cap->cb_half.data)
        cap->cb_half.deallocate();
    if (cap->cr_half.data)
        cap->cr_half.deallocate();
}
