#pragma once
#include <openvr_driver.h>

#include <psvr.h>

#include "OpenPSVRDeviceDriver.h"

using namespace vr;

class CServerDriver_OpenPSVR : public IServerTrackedDeviceProvider {
public:
	CServerDriver_OpenPSVR();
public:
	// Inherited via IServerTrackedDeviceProvider
	virtual EVRInitError Init(IVRDriverContext * pDriverContext) override;
	virtual void Cleanup() override;
	virtual const char * const * GetInterfaceVersions() override { return vr::k_InterfaceVersions; }
	virtual void RunFrame() override;
	virtual bool ShouldBlockStandbyMode() override { return false; }
	virtual void EnterStandby() override;
	virtual void LeaveStandby() override;
private:
	COpenPSVRDeviceDriver * m_pPSVRHmdLatest;

	bool m_bEnablePSVRDriver;
private:
	psvr_context * mp_psvr_ctx;
};