#include "WatchdogDriver_OpenPSVR.h"

// Inherited via IVRWatchdogProvider
EVRInitError CWatchdogDriver_OpenPSVR::Init(IVRDriverContext * pDriverContext) {
	VR_INIT_WATCHDOG_DRIVER_CONTEXT(pDriverContext);
	InitDriverLog(vr::VRDriverLog());

	DriverLog("Initializing Watchdog.\n");

	// Watchdog mode on Windows starts a thread that listens for the 'Y' key on the keyboard to 
	// be pressed. A real driver should wait for a system button event or something else from the 
	// the hardware that signals that the VR system should start up.
	m_bExiting = false;
	m_pWatchdogThread = new std::thread(&CWatchdogDriver_OpenPSVR::run, this);
	if (!m_pWatchdogThread) {
		DriverLog("Unable to create watchdog thread\n");
		return VRInitError_Driver_Failed;
	}

	return VRInitError_None;
}

void CWatchdogDriver_OpenPSVR::Cleanup() {
	m_bExiting = true;
	if (m_pWatchdogThread) {
		m_pWatchdogThread->join();
		delete m_pWatchdogThread;
		m_pWatchdogThread = nullptr;
	}

	CleanupDriverLog();
}

void CWatchdogDriver_OpenPSVR::run() {
	while (!m_bExiting) {
	#if defined( _WINDOWS )
		// on windows send the event when the Y key is pressed.
		/*if ((0x01 & GetAsyncKeyState('Y')) != 0) {
		// Y key was pressed.
		vr::VRWatchdogHost()->WatchdogWakeUp();
		}*/
		//look for when the psvr usb is connected
		//std::this_thread::sleep_for(std::chrono::microseconds(500));
		//vr::VRWatchdogHost()->WatchdogWakeUp();
		std::this_thread::sleep_for(std::chrono::seconds(5));
	#else
		// for the other platforms, just send one every five seconds
		std::this_thread::sleep_for(std::chrono::seconds(5));
		vr::VRWatchdogHost()->WatchdogWakeUp();
	#endif
	}
}