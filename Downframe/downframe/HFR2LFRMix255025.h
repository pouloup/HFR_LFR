#pragma once

#include "HFR2LFRConverter.h"

class HFR2LFRMix255025 : public HFR2LFRConverter
{
public:
	HFR2LFRMix255025(const string HFRVideoPath, const string LFRVideoPath);
	virtual ~HFR2LFRMix255025();

protected:
	/*
	* Process current frame
	* Mix fifty percent of 2 frames into another one.
	*/
	virtual void processFrame();

private:
	double alpha = 0.5;
	double beta = (1.0 - alpha);

	Mat src0, src1, src2, dst;
	Mat tempi, tempo;
};

