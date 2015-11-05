/**
 * Authors: Anthony ARCH - Pierre-Henri DAUVERGNE - Jérémy VIGOUR (ESIR)
**/

#ifndef HFR2LFR_FLOW_DRAWER_H
#define HFR2LFR_FLOW_DRAWER_H

#include "HFR2LFRConverter.h"

class HFR2LFRFlowDrawer: public HFR2LFRConverter
{
public:
	HFR2LFRFlowDrawer(const string HFRVideoPath, const string LFRVideoPath);
	virtual ~HFR2LFRFlowDrawer();

protected:
	/**
	 * Process current frame
	 * Draw the optical flow in the current frame (no motion blur)
	**/
	virtual void processFrame();
};

#endif // HFR2LFR_FLOW_DRAWER_H