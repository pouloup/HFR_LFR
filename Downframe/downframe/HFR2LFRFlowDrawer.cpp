/**
 * Authors: Anthony ARCH - Pierre-Henri DAUVERGNE - Jérémy VIGOUR (ESIR)
**/

#include "HFR2LFRFlowDrawer.h"


HFR2LFRFlowDrawer::HFR2LFRFlowDrawer(const string HFRVideoPath, const string LFRVideoPath)
	: HFR2LFRConverter(HFRVideoPath, LFRVideoPath)
{
}


HFR2LFRFlowDrawer::~HFR2LFRFlowDrawer()
{
}


void HFR2LFRFlowDrawer::processFrame()
{
	m_finalFrameBGR = m_currentFrameBGR.clone();

	Mat opticalFlowMap;

	// Flot optique
	calcOpticalFlowFarneback(m_currentFrame, m_nextFrame, opticalFlowMap,
		0.5,	// pyramid scale (< 1.0)
		4,		// number of levels in pyramid	(1 : only original frame)
		11,		// window size
		4,		// number of iterations at each pyramid level
		7,		// size of pixel neighborhood (larger values mean that the image will be approximated with smoother surfaces, yielding more robust algorithm and more blurred motion field)
		1.4,	// standard dev
		0);		// flags

	drawOpticalFlowMap(m_finalFrameBGR, opticalFlowMap, 10, Scalar(0, 0, 255));
}
