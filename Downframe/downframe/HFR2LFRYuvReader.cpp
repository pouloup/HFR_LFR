#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/video/background_segm.hpp>
#include <vector>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__) // TODO: check win32 include
#include <opencv\cv.h>
#endif


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <iostream>
#include <sstream>

#include "yuv.h"
#include "HFR2LFRYuvReader.h"

using namespace std;
using namespace cv;

HFR2LFRYuvReader::HFR2LFRYuvReader()
{
}

HFR2LFRYuvReader::~HFR2LFRYuvReader()
{
}

/*
 * Utilisé pour faire du 50/50 sur 2 frame YUV ;
 * À modifier pour pouvoir enregistrer directement tout notre flux YUV dans des Mat ;
 * Pour un traitement OpenCV.											  
 */
int HFR2LFRYuvReader::YuvReader(const char * file, int w, int h)
{
    int width;
    int height;

	int cpt = 0;
	Mat src1, src2, dst;

	double alpha = 0.5;
	double beta;

	IplImage *temp0 = cvCreateImage(cvSize(1920, 1080), IPL_DEPTH_8U, 3);
	IplImage *temp1 = cvCreateImage(cvSize(1920, 1080), IPL_DEPTH_8U, 3);

	//VideoWriter outputVideo;
	//outputVideo.open("video.avi", CV_FOURCC('I', '4', '2', '0'), 60.0, Size(1920,1080), 1);

    FILE *fin = NULL;

	CvVideoWriter* writer = 0;
	writer = cvCreateVideoWriter("X:/video.avi", CV_FOURCC('I', '4', '2', '0'), 60.0, cvSize(1920,1080), 1);

    struct YUV_Capture cap;
    enum YUV_ReturnValue ret;
    IplImage *bgr;

    width = w;
    height = h;

    if (width <= 0 || height <= 0)
    {
        fprintf(stderr, "error: bad frame dimensions: %d x %d\n", width, height);
        return 1;
    }

    fin = fopen(file, "rb");
    if (!fin)
    {
        fprintf(stderr, "error: unable to open file: %s\n", file);
        return 1;
    }

    ret = YUV_init(fin, width, height, &cap);
    assert(ret == YUV_OK);

    bgr = cvCreateImage(cvSize(width,height), IPL_DEPTH_8U, 3);
    assert(bgr);

    for (; ;)
	{
		cout << cpt << endl;
	    ret = YUV_read(&cap);
        if (ret == YUV_EOF)
        {
            cvWaitKey(0);
            break;
        }
        else if (ret == YUV_IO_ERROR)
        {
            fprintf(stderr, "I/O error\n");
            break;
        }

		cvCvtColor(cap.ycrcb, bgr, CV_YCrCb2BGR);
		//cvShowImage(file, bgr);

		if(cpt %2 ==0)
		{
			temp0 = bgr;
			cvSaveImage("X:/temp[0].bmp",temp0);
		}

		else if(cpt %2 ==1)
		{
			temp1 = bgr;
			cvSaveImage("X:/temp[1].bmp",temp1);

			src1 = imread("X:/temp[0].bmp");
			src2 = imread("X:/temp[1].bmp");

			if( !src1.data ) { printf("Error loading src1 \n"); return -1; }
			if( !src2.data ) { printf("Error loading src2 \n"); return -1; }
			beta = ( 1.0 - alpha );

			addWeighted( src1, alpha, src2, beta, 0.0, dst);
			IplImage* image2=cvCloneImage(&(IplImage)dst);
			cvWriteFrame(writer, image2);
			cvReleaseImage(&image2);

			//cvSaveImage(reconstruct, image2);
		}
		cpt++;
	}

	//outputVideo.release();
	cvReleaseVideoWriter(&writer);

	return EXIT_SUCCESS;
}
