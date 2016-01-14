#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/video/background_segm.hpp>
#include <vector>
#include <opencv\cv.h>

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
 * Utilisé pour faire du 25/50/25 sur 3 frame YUV ;
 * À modifier pour pouvoir enregistrer directement tout notre flux YUV dans des Mat ;
 * Pour un traitement OpenCV.											  
 */
int HFR2LFRYuvReader::YuvReader(int argc, char **argv)
{

	int width;
	int height;
	int nbFrame;
	int cpt = 1;
	Mat src1, src2, src0, tempi, tempo, dst;
	double alpha = 0.50;
	double beta;

	IplImage *temp0 = cvCreateImage(cvSize(1920, 1080), IPL_DEPTH_8U, 3);
	IplImage *temp1 = cvCreateImage(cvSize(1920, 1080), IPL_DEPTH_8U, 3);
	IplImage *temp2 = cvCreateImage(cvSize(1920, 1080), IPL_DEPTH_8U, 3);

	double ssimVal = 0;

	FILE *fin = NULL;

	CvVideoWriter* writer = 0;
	writer = cvCreateVideoWriter("E:/Antoine/video.avi", CV_FOURCC('I', '4', '2', '0'), 60.0, cvSize(1920, 1080), 1);

	struct YUV_Capture cap;
	enum YUV_ReturnValue ret;
	IplImage *bgr;

	if (argc != 5)
	{
		fprintf(stderr, "usage: %s file.yuv width height frame\n", argv[0]);
		return 1;
	}

	width = atoi(argv[2]);
	height = atoi(argv[3]);
	nbFrame = atoi(argv[4]);

	if (width <= 0 || height <= 0)
	{
		fprintf(stderr, "error: bad frame dimensions: %d x %d\n", width, height);
		return 1;
	}

	fin = fopen(argv[1], "rb");
	if (!fin)
	{
		fprintf(stderr, "error: unable to open file: %s\n", argv[1]);
		return 1;
	}

	ret = YUV_init(fin, width, height, &cap);
	assert(ret == YUV_OK);

	bgr = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
	assert(bgr);

	for (; ;)
	{
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
		cvShowImage(argv[1], bgr);

		beta = (1.0 - alpha);

		if (cpt == 1)
		{
			temp0 = bgr;
			cvSaveImage("E:/Antoine/DownscaleHfr/DownscaleHfr/DownscaleHfr/sample/temp[0].bmp", temp0);
		}

		if (cpt == 2)
		{
			temp1 = bgr;
			cvSaveImage("E:/Antoine/DownscaleHfr/DownscaleHfr/DownscaleHfr/sample/temp[1].bmp", temp1);
		}

		if (cpt == 3)
		{
			temp2 = bgr;
			cvSaveImage("E:/Antoine/DownscaleHfr/DownscaleHfr/DownscaleHfr/sample/temp[2].bmp", temp2);

			src0 = imread("E:/Antoine/DownscaleHfr/DownscaleHfr/DownscaleHfr/sample/temp[0].bmp");
			src1 = imread("E:/Antoine/DownscaleHfr/DownscaleHfr/DownscaleHfr/sample/temp[1].bmp");
			src2 = imread("E:/Antoine/DownscaleHfr/DownscaleHfr/DownscaleHfr/sample/temp[2].bmp");

			addWeighted(src1, alpha, src0, beta, 0.0, tempi);
			addWeighted(src1, alpha, src2, beta, 0.0, tempo);
			addWeighted(tempi, 0.5, tempo, 0.5, 0.0, dst);

			IplImage* image2 = cvCloneImage(&(IplImage)dst);
			cvWriteFrame(writer, image2);
			cvReleaseImage(&image2);
			cpt = 0;
		}

		if (cpt == 0)
		{
			temp0 = bgr;
			cvSaveImage("E:/Antoine/DownscaleHfr/DownscaleHfr/DownscaleHfr/sample/temp[0].bmp", temp0);
			cpt++;
		}
		cpt++;
	}

	/*remplir la vidéo d'une frame de plus en cas de nombre impaire de frame*/
	if (nbFrame % 2 == 1)
	{
		cvWriteFrame(writer, bgr);
	}

	cvReleaseVideoWriter(&writer);
	cout << "the End" << endl;
	system("PAUSE");
	return EXIT_SUCCESS;
}