#include "HFR2LFRMix5050.h"
#include "Utils.h"

HFR2LFRMix5050::HFR2LFRMix5050(const string HFRVideoPath, const string LFRVideoPath)
	: HFR2LFRConverter(HFRVideoPath, LFRVideoPath)
{
}

HFR2LFRMix5050::~HFR2LFRMix5050()
{
}

void HFR2LFRMix5050::processFrame()
{
	src1 = m_prevFrameBGR.clone();
	src2 = m_currentFrameBGR.clone();

	addWeighted(src1, alpha, src2, beta, 0.0, dst);

	m_finalFrameBGR = dst.clone();
}
