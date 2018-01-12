#include "ServerDriver_OpenPSVR.h"
#include "driverlog.h"

#include "OpenPSVR.h"

void psvr_driver_log(const char * msg, va_list args) {
	char buff[256];
	vsnprintf(buff, 256, msg, args);

	DriverLog(buff);
}

CServerDriver_OpenPSVR::CServerDriver_OpenPSVR()
	: m_pPSVRHmdLatest(NULL)
	, m_bEnablePSVRDriver(false) {
	psvr_set_log((psvr_log*)psvr_driver_log);
}

// Inherited via IServerTrackedDeviceProvider
EVRInitError CServerDriver_OpenPSVR::Init(IVRDriverContext * pDriverContext) {
	psvr_set_log((psvr_log*)psvr_driver_log);

	VR_INIT_SERVER_DRIVER_CONTEXT(pDriverContext);
	InitDriverLog(vr::VRDriverLog());

	psvr_context* ctx = nullptr;

	DriverLog("Establishing Connection.\n");
	int err = psvr_open(&ctx);

	if (err == -1) {
		DriverLog("Error connecting.\n");
		return VRInitError_Init_HmdNotFoundPresenceFailed;
	}

	this->mp_psvr_ctx = ctx;

	m_pPSVRHmdLatest = new COpenPSVRDeviceDriver(this->mp_psvr_ctx);

	if (!vr::VRServerDriverHost()->TrackedDeviceAdded(m_pPSVRHmdLatest->GetSerialNumber().c_str(), vr::TrackedDeviceClass_HMD, m_pPSVRHmdLatest)) {
		DriverLog("Error adding psvr.\n");

		delete m_pPSVRHmdLatest;
		m_pPSVRHmdLatest = nullptr;
		return VRInitError_Driver_Failed;
	}

	return VRInitError_None;
}

void CServerDriver_OpenPSVR::Cleanup() {
	delete m_pPSVRHmdLatest;
	m_pPSVRHmdLatest = NULL;

	if (this->mp_psvr_ctx) psvr_close(this->mp_psvr_ctx);
	this->mp_psvr_ctx = nullptr;

	psvr_set_log(nullptr);
	CleanupDriverLog();
}

void CServerDriver_OpenPSVR::RunFrame() {
	if (m_pPSVRHmdLatest) {
		m_pPSVRHmdLatest->RunFrame();
	}
}

void CServerDriver_OpenPSVR::EnterStandby() {
}

void CServerDriver_OpenPSVR::LeaveStandby() {
}