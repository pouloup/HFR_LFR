#include "HFR2LFRYuvReader.h"

using namespace std;
using namespace cv;

HFR2LFRYuvReader::HFR2LFRYuvReader(const char * file, int w, int h)
{
	YuvReader(file, w, h);
}

HFR2LFRYuvReader::~HFR2LFRYuvReader()
{
	YUV_cleanup(&cap);
}

bool HFR2LFRYuvReader::read(cv::OutputArray image) {

	ret = YUV_read(&cap);
	if (ret == YUV_EOF)
	{
		cv::waitKey(1);
		return false;
	}
	else if (ret == YUV_IO_ERROR)
	{
		fprintf(stderr, "I/O error\n");
		return false;
	}

	cvtColor(cap.ycrcb, image, CV_YCrCb2BGR);
	return true;
}

bool HFR2LFRYuvReader::set(int propId, double value)
{
	bool status = false;

	for (unsigned int i = 0; i < value; i++)
		status = read(bgr);

	return status;
}

int HFR2LFRYuvReader::YuvReader(const char * file, int w, int h)
{
	int width;
	int height;

	int cpt = 0;
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

	bgr = Mat(Size(width, height), CV_8UC3);

	return EXIT_SUCCESS;
}
