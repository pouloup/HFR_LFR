#pragma once

#include "HFR2LFRConverter.h"

class HFR2LFRMix5050 : public HFR2LFRConverter
{
public:
	HFR2LFRMix5050(const string HFRVideoPath, const string LFRVideoPath);
	virtual ~HFR2LFRMix5050();

protected:
	/*
	* Process current frame
	* Mix fifty percent of 2 frames into another one.
	*/
	virtual void processFrame();

private:
	double alpha = 0.5;
	double beta = (1.0 - alpha);

	Mat src1, src2, dst;
};

