#pragma once

#include <openvr_driver.h>
#include "driverlog.h"

#include <thread>

using namespace vr;

class CWatchdogDriver_OpenPSVR : public IVRWatchdogProvider {
public:
	CWatchdogDriver_OpenPSVR() : m_bExiting(false) {}
public:
	virtual EVRInitError Init(IVRDriverContext * pDriverContext) override;
	virtual void Cleanup() override;
	virtual void run();
private:
	std::thread *m_pWatchdogThread;
	bool m_bExiting;
};

