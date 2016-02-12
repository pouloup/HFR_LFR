#include "HFR2LFRMix255025.h"


HFR2LFRMix255025::HFR2LFRMix255025(const string HFRVideoPath, const string LFRVideoPath)
	: HFR2LFRConverter(HFRVideoPath, LFRVideoPath)
{
}

HFR2LFRMix255025::~HFR2LFRMix255025()
{
}

void HFR2LFRMix255025::processFrame()
{
	src0 = m_prevFrameBGR.clone();
	src1 = m_currentFrameBGR.clone();
	src2 = m_nextFrameBGR.clone();

	addWeighted(src1, alpha, src0, beta, 0.0, tempi);
	addWeighted(src1, alpha, src2, beta, 0.0, tempo);
	addWeighted(tempi, 0.5, tempo, 0.5, 0.0, dst);

	m_finalFrameBGR = dst.clone();
}
