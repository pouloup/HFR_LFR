#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__) // TODO: check win32 include
#include <opencv\cv.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <iostream>
#include <sstream>

#include "HFR2LFRYuv.h"

class HFR2LFRYuvReader {
public:
	HFR2LFRYuvReader(const char * file, int w, int h);
	virtual ~HFR2LFRYuvReader();
	bool read(cv::OutputArray image);
	bool set(int propId, double value);

private:
	FILE *fin = NULL;

	YUV_Capture cap;
	enum YUV_ReturnValue ret;

	cv::Mat bgr;

	int YuvReader(const char * file, int w, int h);
};


