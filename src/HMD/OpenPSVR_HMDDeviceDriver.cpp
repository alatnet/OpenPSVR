#include "OpenPSVR_HMDDeviceDriver.h"

#include <stdlib.h>
#if defined( _WINDOWS )
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <morpheus.h>

//#include "BMI055Integrator.h"
//#include "MadgwickAHRS.hpp"

#if defined( _WINDOWS )
struct MonitorEnumParam {
	COpenPSVR_HMDDeviceDriver* caller;
	std::string display;
};

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
#endif

// keys for use with the settings API
static const char * const k_pch_Section = "openpsvr";
static const char * const k_pch_WindowX_Int32 = "windowX";
static const char * const k_pch_WindowY_Int32 = "windowY";
static const char * const k_pch_WindowWidth_Int32 = "windowWidth";
static const char * const k_pch_WindowHeight_Int32 = "windowHeight";
static const char * const k_pch_RenderWidth_Int32 = "renderWidth";
static const char * const k_pch_RenderHeight_Int32 = "renderHeight";
static const char * const k_pch_SecondsFromVsyncToPhotons_Float = "secondsFromVsyncToPhotons";
static const char * const k_pch_DisplayFrequency_Float = "displayFrequency";

COpenPSVR_HMDDeviceDriver::COpenPSVR_HMDDeviceDriver(psvr_context* psvr_ctx) {
	this->mp_sensorThread = nullptr;
	this->mp_psvr_ctx = nullptr;

	m_unObjectId = vr::k_unTrackedDeviceIndexInvalid;
	m_ulPropertyContainer = vr::k_ulInvalidPropertyContainer;
	this->mp_psvr_ctx = psvr_ctx;

	DriverLog("Using settings values\n");
	m_flIPD = vr::VRSettings()->GetFloat(k_pch_SteamVR_Section, k_pch_SteamVR_IPD_Float);

	/*
	char buf[1024];
	vr::VRSettings()->GetString(k_pch_Section, k_pch_Sample_SerialNumber_String, buf, sizeof(buf));
	m_sSerialNumber = buf;
	*/

	//request psvr device info.
	uint8_t deviceCmd[] = { 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	psvr_send_command_sync(this->mp_psvr_ctx, eRID_DeviceInfo, deviceCmd, 8);

	bool serial_set = false;

	//try and get the serial number of the psvr.
	for (int i = 0; i < 10; i++) {
		psvr_control_frame frame;
		int err = psvr_read_sync_timeout(PSVR_INTERFACE_HID_CONTROL, this->mp_psvr_ctx, (uint8_t*)&frame, sizeof(psvr_control_frame), 500);

		if (err > 0) {
			if (frame.s.header.r_id == eRT_Info) {
				m_sSerialNumber = "";
				for (int i = 0; i < 16; i++)
					m_sSerialNumber += (char)frame.s.dinfo.s.serialNumber[i];
				serial_set = true;
				break;
			}
		}
	}

	//either the control interface "freaked out" and wont respond, or after 10 tries, it timed out on trying to get the serial.
	//so create a random serial number for usage.
	if (!serial_set) {
		m_sSerialNumber = "PSVRDT";
		for (int i = 0; i<3; i++) m_sSerialNumber += std::to_string(std::rand());
	}

	m_sModelNumber = "CUH-ZVR";

	//find where the psvr is displaying on.
	this->FindHMDMonitor();

	//get settings version on window position.  If not there, use the position found.
	int32_t wX = vr::VRSettings()->GetInt32(k_pch_Section, k_pch_WindowX_Int32);
	int32_t wY = vr::VRSettings()->GetInt32(k_pch_Section, k_pch_WindowY_Int32);
	if (wX != 0) m_nWindowX = wX;
	if (wY != 0) m_nWindowY = wY;

	//get settings version of window size.  If not there, use the size found.
	int32_t wWidth = vr::VRSettings()->GetInt32(k_pch_Section, k_pch_WindowWidth_Int32);
	int32_t wHeight = vr::VRSettings()->GetInt32(k_pch_Section, k_pch_WindowHeight_Int32);
	if (wWidth != 0) m_nWindowWidth = wWidth;
	if (wHeight != 0) m_nWindowHeight = wHeight;

	//get settings version of render size.  If not there, use window size.
	int32_t rWidth = vr::VRSettings()->GetInt32(k_pch_Section, k_pch_RenderWidth_Int32);
	int32_t rHeight = vr::VRSettings()->GetInt32(k_pch_Section, k_pch_RenderHeight_Int32);
	m_nRenderWidth = m_nWindowWidth;
	m_nRenderHeight = m_nWindowHeight;
	if (rWidth != 0) m_nRenderWidth = rWidth;
	if (rHeight != 0) m_nRenderHeight = rHeight;

	m_flSecondsFromVsyncToPhotons = vr::VRSettings()->GetFloat(k_pch_Section, k_pch_SecondsFromVsyncToPhotons_Float);
	m_flDisplayFrequency = vr::VRSettings()->GetFloat(k_pch_Section, k_pch_DisplayFrequency_Float);

	DriverLog("Serial Number: %s\n", m_sSerialNumber.c_str());
	DriverLog("Model Number: %s\n", m_sModelNumber.c_str());
	DriverLog("Window: %d %d %d %d\n", m_nWindowX, m_nWindowY, m_nWindowWidth, m_nWindowHeight);
	DriverLog("Render Target: %d %d\n", m_nRenderWidth, m_nRenderHeight);
	DriverLog("Seconds from Vsync to Photons: %f\n", m_flSecondsFromVsyncToPhotons);
	DriverLog("Display Frequency: %f\n", m_flDisplayFrequency);
	DriverLog("IPD: %f\n", m_flIPD);

	this->m_sensorFrame = { 0 };

	this->mp_sensorMutex = new std::mutex();

	//angle[0] = glm::vec3(0.0f, 0.0f, 0.0f);
	//angle[1] = glm::vec3(0.0f, 0.0f, 0.0f);

	//ahrsFilter[0].begin(1000);
	//ahrsFilter[1].begin(1000);

	this->integrator = new BMI055Integrator(AFS_2G, GFS_2000DPS);
}

COpenPSVR_HMDDeviceDriver::~COpenPSVR_HMDDeviceDriver() {
	delete this->mp_sensorMutex;
	delete this->integrator;
}

EVRInitError COpenPSVR_HMDDeviceDriver::Activate(uint32_t unObjectId) {
	DriverLog("Activating PSVR Device. Index: %i\n", unObjectId);
	m_unObjectId = unObjectId;
	m_ulPropertyContainer = vr::VRProperties()->TrackedDeviceToPropertyContainer(m_unObjectId);

	vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, Prop_ModelNumber_String, m_sModelNumber.c_str());
	vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, Prop_RenderModelName_String, m_sModelNumber.c_str());
	vr::VRProperties()->SetFloatProperty(m_ulPropertyContainer, Prop_UserIpdMeters_Float, m_flIPD);
	vr::VRProperties()->SetFloatProperty(m_ulPropertyContainer, Prop_UserHeadToEyeDepthMeters_Float, 0.f);
	vr::VRProperties()->SetFloatProperty(m_ulPropertyContainer, Prop_DisplayFrequency_Float, m_flDisplayFrequency);
	vr::VRProperties()->SetFloatProperty(m_ulPropertyContainer, Prop_SecondsFromVsyncToPhotons_Float, m_flSecondsFromVsyncToPhotons);

	vr::VRProperties()->SetBoolProperty(m_ulPropertyContainer, Prop_ContainsProximitySensor_Bool, true);

	// return a constant that's not 0 (invalid) or 1 (reserved for Oculus)
	vr::VRProperties()->SetUint64Property(m_ulPropertyContainer, Prop_CurrentUniverseId_Uint64, 2);

	// avoid "not fullscreen" warnings from vrmonitor
	vr::VRProperties()->SetBoolProperty(m_ulPropertyContainer, Prop_IsOnDesktop_Bool, false);

	//edid of the psvr monitor
	vr::VRProperties()->SetInt32Property(m_ulPropertyContainer, Prop_EdidVendorID_Int32, 0xD94D);
	vr::VRProperties()->SetInt32Property(m_ulPropertyContainer, Prop_EdidProductID_Int32, 0xB403);

	// Icons can be configured in code or automatically configured by an external file "drivername\resources\driver.vrresources".
	// Icon properties NOT configured in code (post Activate) are then auto-configured by the optional presence of a driver's "drivername\resources\driver.vrresources".
	// In this manner a driver can configure their icons in a flexible data driven fashion by using an external file.
	//
	// The structure of the driver.vrresources file allows a driver to specialize their icons based on their HW.
	// Keys matching the value in "Prop_ModelNumber_String" are considered first, since the driver may have model specific icons.
	// An absence of a matching "Prop_ModelNumber_String" then considers the ETrackedDeviceClass ("HMD", "Controller", "GenericTracker", "TrackingReference")
	// since the driver may have specialized icons based on those device class names.
	//
	// An absence of either then falls back to the "system.vrresources" where generic device class icons are then supplied.
	//
	// Please refer to "bin\drivers\sample\resources\driver.vrresources" which contains this sample configuration.
	//
	// "Alias" is a reserved key and specifies chaining to another json block.
	//
	// In this sample configuration file (overly complex FOR EXAMPLE PURPOSES ONLY)....
	//
	// "Model-v2.0" chains through the alias to "Model-v1.0" which chains through the alias to "Model-v Defaults".
	//
	// Keys NOT found in "Model-v2.0" would then chase through the "Alias" to be resolved in "Model-v1.0" and either resolve their or continue through the alias.
	// Thus "Prop_NamedIconPathDeviceAlertLow_String" in each model's block represent a specialization specific for that "model".
	// Keys in "Model-v Defaults" are an example of mapping to the same states, and here all map to "Prop_NamedIconPathDeviceOff_String".
	//
	bool bSetupIconUsingExternalResourceFile = true;
	if (!bSetupIconUsingExternalResourceFile) {
		// Setup properties directly in code.
		// Path values are of the form {drivername}\icons\some_icon_filename.png
		vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceOff_String, "{sample}/icons/headset_sample_status_off.png");
		vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceSearching_String, "{sample}/icons/headset_sample_status_searching.gif");
		vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceSearchingAlert_String, "{sample}/icons/headset_sample_status_searching_alert.gif");
		vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceReady_String, "{sample}/icons/headset_sample_status_ready.png");
		vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceReadyAlert_String, "{sample}/icons/headset_sample_status_ready_alert.png");
		vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceNotReady_String, "{sample}/icons/headset_sample_status_error.png");
		vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceStandby_String, "{sample}/icons/headset_sample_status_standby.png");
		vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_NamedIconPathDeviceAlertLow_String, "{sample}/icons/headset_sample_status_ready_low.png");
	}

	DriverLog("Turning Headset On.\n");
	uint32_t on = 1;
	psvr_send_command_sync(this->mp_psvr_ctx, eRID_HeadsetPower, (uint8_t*)&on, 4);


	DriverLog("Setting VR Mode.\n");
	uint8_t vrmode[] = { 0x01, 0x00, 0x00, 0x00 };
	psvr_send_command_sync(this->mp_psvr_ctx, eRID_SetMode, vrmode, 4);

	this->m_brunning = true;

	if (!this->mp_sensorThread) {
		DriverLog("Starting up Sensor Thread.\n");
		this->mp_sensorThread = new std::thread(&COpenPSVR_HMDDeviceDriver::sensorUpdate, this);
	}
	//this->mp_controlThread = new std::thread(&COpenPSVR_HMDDeviceDriver::controlUpdate, this);

	return VRInitError_None;
}

void COpenPSVR_HMDDeviceDriver::Deactivate() {
	DriverLog("Deactivating PSVR.\n");

	//kill the sensor thread
	this->m_brunning = false;
	this->mp_sensorThread->join();
	delete this->mp_sensorThread;
	this->mp_sensorThread = nullptr;

	DriverLog("Turning Headset Off.\n");
	uint32_t off = 0;
	psvr_send_command_sync(this->mp_psvr_ctx, eRID_HeadsetPower, (uint8_t*)&off, 4);
	m_unObjectId = vr::k_unTrackedDeviceIndexInvalid;
}

void COpenPSVR_HMDDeviceDriver::EnterStandby() {}

void * COpenPSVR_HMDDeviceDriver::GetComponent(const char * pchComponentNameAndVersion) {
	if (!_stricmp(pchComponentNameAndVersion, vr::IVRDisplayComponent_Version)) {
		return (vr::IVRDisplayComponent*)this;
	}

	// override this to add a component to a driver
	return nullptr;
}

void COpenPSVR_HMDDeviceDriver::DebugRequest(const char * pchRequest, char * pchResponseBuffer, uint32_t unResponseBufferSize) {
	if (unResponseBufferSize >= 1)
		pchResponseBuffer[0] = 0;
}

DriverPose_t COpenPSVR_HMDDeviceDriver::GetPose() {
	this->mp_sensorMutex->lock();	

	DriverPose_t pose = { 0 };

	pose.poseIsValid = true;
	pose.result = integrator->isCalibrating() ? TrackingResult_Calibrating_InProgress : TrackingResult_Running_OK;
	pose.deviceIsConnected = true;
	pose.willDriftInYaw = true;
	pose.shouldApplyHeadModel = true;
	pose.poseTimeOffset = m_poseTimer.elapsed();

	//parse sensor data to quat
	//glm::quat quat = BMI055Integrator::Parse((void *)&this->m_sensorFrame);
	//glm::quat quat = COpenPSVR_HMDDeviceDriver::fixQuat(BMI055Integrator::Parse((void *)&this->m_sensorFrame));
	//auto euler = BMI055Integrator::ToEuler(&quat);

	//DriverLog("%f, %f, %f - %s\n", euler.x, euler.y, euler.z, BMI055Integrator::calibrating ? "Calibrating" : "OK");

	//if (BMI055Integrator::calibrating) {
		//pose.qWorldFromDriverRotation = COpenPSVR_HMDDeviceDriver::HmdQuaternion_Init(1, 0, 0, 0);
		//pose.qDriverFromHeadRotation = COpenPSVR_HMDDeviceDriver::HmdQuaternion_Init(1, 0, 0, 0);
		//pose.qRotation = COpenPSVR_HMDDeviceDriver::HmdQuaternion_Init(1, 0, 0, 0);

		/*glm::vec3 gyro[2] = {
			(GFS_2000DPS * glm::vec3(this->m_sensorFrame.s.data[0].gyro.pitch, this->m_sensorFrame.s.data[0].gyro.yaw, this->m_sensorFrame.s.data[0].gyro.roll)),
			(GFS_2000DPS * glm::vec3(this->m_sensorFrame.s.data[1].gyro.pitch, this->m_sensorFrame.s.data[1].gyro.yaw, this->m_sensorFrame.s.data[1].gyro.roll))
		};

		glm::vec3 accel[2] = {
			AFS_2G * glm::vec3(this->m_sensorFrame.s.data[0].accel.x, this->m_sensorFrame.s.data[0].accel.y, this->m_sensorFrame.s.data[0].accel.z),
			AFS_2G * glm::vec3(this->m_sensorFrame.s.data[1].accel.x, this->m_sensorFrame.s.data[1].accel.y, this->m_sensorFrame.s.data[1].accel.z)
		};*/

		glm::vec3 gyro[2] = {
			glm::vec3(this->m_sensorFrame.s.data[0].gyro.pitch, this->m_sensorFrame.s.data[0].gyro.yaw, this->m_sensorFrame.s.data[0].gyro.roll),
			glm::vec3(this->m_sensorFrame.s.data[1].gyro.pitch, this->m_sensorFrame.s.data[1].gyro.yaw, this->m_sensorFrame.s.data[1].gyro.roll)
		};

		glm::vec3 accel[2] = {
			glm::vec3(this->m_sensorFrame.s.data[0].accel.x, this->m_sensorFrame.s.data[0].accel.y, this->m_sensorFrame.s.data[0].accel.z),
			glm::vec3(this->m_sensorFrame.s.data[1].accel.x, this->m_sensorFrame.s.data[1].accel.y, this->m_sensorFrame.s.data[1].accel.z)
		};

		uint32_t timestamp[2] = {
			this->m_sensorFrame.s.data[0].timestamp,
			this->m_sensorFrame.s.data[1].timestamp
		};

		glm::vec3 euler = integrator->ParseToEuler(gyro, accel, timestamp);

		//euler.z -= glm::radians(180.0f);

		glm::quat filterPose = glm::quat(euler);
		
		//kalman filter here?
		//fuse both sensor readings into one output
		//madgwick update on one input.
		//use sse for kalman filter

		//Madgwick::MadgwickAHRSupdateIMU(gyro[0].x, gyro[0].y, gyro[0].z, accel[0].x, accel[0].y, accel[0].z);
		//Madgwick::MadgwickAHRSupdateIMU(gyro[1].x, gyro[1].y, gyro[1].z, accel[1].x, accel[1].y, accel[1].z);

		//glm::quat madgwick = glm::quat(Madgwick::q0, Madgwick::q1, Madgwick::q2, Madgwick::q3);

		//for (int i = 0; i < 2; i++) {
			//float currTime = *((float*)&(this->m_sensorFrame.s.data[i].timestamp));

			//float offset = currTime - prevTime[i];

			//DriverLog("CurrTime-%i: %f = offset: %f", i, currTime, offset);

			//ahrsFilter[i].update(gyro[i].x, gyro[i].y, gyro[i].z, accel[i].x, accel[i].y, accel[i].z, 0.0f, 0.0f, 0.0f);

			//ax, ay, az, gx*PI/180.0f, gy*PI/180.0f, gz*PI/180.0f
		//	ahrsFilter[i].updateIMU(glm::radians(gyro[i].x), glm::radians(gyro[i].y), glm::radians(gyro[i].z), accel[i].x, accel[i].y, accel[i].z);
			
			//float angle = 0.98 * (angle + gyro * dt) + 0.02 * accel
		//	angle[i].x = 0.98 * (angle[i].x + gyro[i].x * pose.poseTimeOffset) + 0.02 * accel[i].x;
		//	angle[i].y = 0.98 * (angle[i].y + gyro[i].y * pose.poseTimeOffset) + 0.02 * accel[i].y;
		//	angle[i].z = 0.98 * (angle[i].z + gyro[i].z * pose.poseTimeOffset) + 0.02 * accel[i].z;
		//}

		//glm::quat filterPose = glm::quat(
		//	glm::vec3(
		//		glm::radians(((ahrsFilter[0].getPitch() + ahrsFilter[1].getPitch()) / 2.0f)),
		//		glm::radians(((ahrsFilter[0].getYaw() + ahrsFilter[1].getYaw()) / 2.0f)),
		//		glm::radians(((ahrsFilter[0].getRoll() + ahrsFilter[1].getRoll()) / 2.0f))
		//	)
			/*glm::vec3(
				(angle[0].x + angle[1].x) / 2.0f,
				(angle[0].y + angle[1].y) / 2.0f,
				(angle[0].z + angle[1].z) / 2.0f
			)*/
		//);

		pose.qRotation = COpenPSVR_HMDDeviceDriver::HmdQuaternion_Init(filterPose.w, filterPose.x, filterPose.y, filterPose.z);
		pose.qWorldFromDriverRotation = COpenPSVR_HMDDeviceDriver::HmdQuaternion_Init(1, 0, 0, 0);
		pose.qDriverFromHeadRotation = COpenPSVR_HMDDeviceDriver::HmdQuaternion_Init(1, 0, 0, 0);
	//} else {
	//	pose.qWorldFromDriverRotation = COpenPSVR_HMDDeviceDriver::HmdQuaternion_Init(1, 0, 0, 0);
	//	pose.qDriverFromHeadRotation = COpenPSVR_HMDDeviceDriver::HmdQuaternion_Init(1, 0, 0, 0);
		//	pose.qWorldFromDriverRotation = HmdQuaternion_Init(quat.w, quat.x, quat.y, quat.z);
		//	pose.qDriverFromHeadRotation = HmdQuaternion_Init(quat.w, quat.x, quat.y, quat.z);
	//	pose.qRotation = COpenPSVR_HMDDeviceDriver::HmdQuaternion_Init(quat.w, quat.x, quat.y, quat.z);
	//}

	this->mp_sensorMutex->unlock();

	this->m_poseTimer.reset();

	return pose;
}

// Inherited via IVRDisplayComponent
void COpenPSVR_HMDDeviceDriver::GetWindowBounds(int32_t * pnX, int32_t * pnY, uint32_t * pnWidth, uint32_t * pnHeight) {
	*pnX = m_nWindowX;
	*pnY = m_nWindowY;
	*pnWidth = m_nWindowWidth;
	*pnHeight = m_nWindowHeight;

	DriverLog("WindowBounds: %d %d %d %d\n", m_nWindowX, m_nWindowY, m_nWindowWidth, m_nWindowHeight);
}

void COpenPSVR_HMDDeviceDriver::GetRecommendedRenderTargetSize(uint32_t * pnWidth, uint32_t * pnHeight) {
	*pnWidth = m_nRenderWidth;
	*pnHeight = m_nRenderHeight;
}

void COpenPSVR_HMDDeviceDriver::GetEyeOutputViewport(EVREye eEye, uint32_t * pnX, uint32_t * pnY, uint32_t * pnWidth, uint32_t * pnHeight) {
	*pnY = 0;
	*pnWidth = m_nWindowWidth / 2;
	*pnHeight = m_nWindowHeight;

	if (eEye == Eye_Left) {
		*pnX = 0;
	} else {
		*pnX = m_nWindowWidth / 2;
	}
}

void COpenPSVR_HMDDeviceDriver::GetProjectionRaw(EVREye eEye, float * pfLeft, float * pfRight, float * pfTop, float * pfBottom) {
	*pfLeft = -1.0;
	*pfRight = 1.0;
	*pfTop = -1.0;
	*pfBottom = 1.0;
}

DistortionCoordinates_t COpenPSVR_HMDDeviceDriver::ComputeDistortion(EVREye eEye, float fU, float fV) {
	DistortionCoordinates_t coordinates;
	coordinates.rfBlue[0] = fU;
	coordinates.rfBlue[1] = fV;
	coordinates.rfGreen[0] = fU;
	coordinates.rfGreen[1] = fV;
	coordinates.rfRed[0] = fU;
	coordinates.rfRed[1] = fV;
	return coordinates;
}

void COpenPSVR_HMDDeviceDriver::RunFrame() {
	// In a real driver, this should happen from some pose tracking thread.
	// The RunFrame interval is unspecified and can be very irregular if some other
	// driver blocks it for some periodic task.
	//if (m_unObjectId != vr::k_unTrackedDeviceIndexInvalid) {
	//	vr::VRServerDriverHost()->TrackedDevicePoseUpdated(m_unObjectId, GetPose(), sizeof(DriverPose_t));
	//}
}

void COpenPSVR_HMDDeviceDriver::sensorUpdate() {
	this->m_poseTimer.reset();
	while (this->m_brunning) {
		this->mp_sensorMutex->lock();
		psvr_read_sensor_sync(this->mp_psvr_ctx, (uint8_t *)&m_sensorFrame, sizeof(psvr_sensor_frame));
		this->mp_sensorMutex->unlock();

		//if ((0x01 & GetAsyncKeyState('A')) && !BMI055Integrator::calibrating) {
		//	BMI055Integrator::Recalibrate();
		//}

		//if ((0x01 & GetAsyncKeyState('S'))) {
		//	BMI055Integrator::Recenter();
		//}

		vr::VRServerDriverHost()->TrackedDevicePoseUpdated(m_unObjectId, GetPose(), sizeof(DriverPose_t));
		//vr::VRServerDriverHost()->ProximitySensorState(m_unObjectId, this->m_sensorFrame.s.status.worn);
		std::this_thread::yield();
	}
}

void COpenPSVR_HMDDeviceDriver::controlUpdate() {
	while (this->m_brunning) {
	}
}

void COpenPSVR_HMDDeviceDriver::FindHMDMonitor() {
#if defined( _WINDOWS )
	MonitorEnumParam param;
	param.caller = this;

	DISPLAY_DEVICE dd;
	dd.cb = sizeof(DISPLAY_DEVICE);

	for (DWORD i = 0; EnumDisplayDevices(NULL, i, &dd, 0); i++) { // query all display adaptors
		if (dd.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER)
			continue;

		if (dd.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP) { //look only for active displays
			/*DriverLog("Found Display Adapter:\n");

			std::wstring dNameW(dd.DeviceName);
			std::string dName(dNameW.begin(), dNameW.end());

			std::wstring dStringW(dd.DeviceString);
			std::string dString(dStringW.begin(), dStringW.end());

			std::wstring dIDW(dd.DeviceID);
			std::string dID(dIDW.begin(), dIDW.end());

			std::wstring dKeyW(dd.DeviceKey);
			std::string dKey(dKeyW.begin(), dKeyW.end());

			DriverLog("-DeviceName: %s\n", dName.c_str());
			DriverLog("-DeviceString: %s\n", dString.c_str());
			DriverLog("-DeviceID: %s\n", dID.c_str());
			DriverLog("-DeviceKey: %s\n", dKey.c_str());*/

			DISPLAY_DEVICE monInfo;
			monInfo.cb = sizeof(DISPLAY_DEVICE);

			for (DWORD j = 0; EnumDisplayDevices(dd.DeviceName, j, &monInfo, 0); j++) { //query all monitors on the adaptor
				/*DriverLog("Found Monitor:\n");
				std::wstring mNameW(monInfo.DeviceName);
				std::string mName(mNameW.begin(), mNameW.end());

				std::wstring mStringW(monInfo.DeviceString);
				std::string mString(mStringW.begin(), mStringW.end());

				std::wstring mIDW(monInfo.DeviceID);
				std::string mID(mIDW.begin(), mIDW.end());

				std::wstring mKeyW(monInfo.DeviceKey);
				std::string mKey(mKeyW.begin(), mKeyW.end());

				DriverLog("--DeviceName: %s\n", mName.c_str());
				DriverLog("--DeviceString: %s\n", mString.c_str());

				DriverLog("--DeviceID: %s\n", mID.c_str());
				DriverLog("--DeviceKey: %s\n", mKey.c_str());*/

				std::wstring mIDW(monInfo.DeviceID);
				std::string mID(mIDW.begin(), mIDW.end());

				if (mID.find("SNYB403") != std::string::npos) { //if the monitor has the string "SNYB403" in it, that's the psvr's display
					std::wstring dNameW(dd.DeviceName);
					std::string dName(dNameW.begin(), dNameW.end());
					param.display = dName;

					DriverLog("Found PSVR Display: %s\n", dName.c_str());
					break;
				}
			}
		}
	}

	if (param.display.size() != 0) {
		EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)&param); //get the information about that monitor.
	} else {
		DriverLog("Could not find PSVR Display.\n");
		this->SetDisplayInfo(0, 0);
	}
#else
	//Linux implementation to be added.
	this->SetDisplayInfo(0, 0);
#endif
}

void COpenPSVR_HMDDeviceDriver::SetDisplayInfo(int32_t x, int32_t y, int32_t w, int32_t h) {
	m_nWindowX = x;
	m_nWindowY = y;
	m_nWindowWidth = w;
	m_nWindowHeight = h;
}


void COpenPSVR_HMDDeviceDriver::Timer::reset() { beg_ = clock_::now(); }
double COpenPSVR_HMDDeviceDriver::Timer::elapsed() const {
	return std::chrono::duration_cast<millisecond_>
		(clock_::now() - beg_).count();
}

#if defined( _WINDOWS )
BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
	MonitorEnumParam* param = (MonitorEnumParam*)dwData;

	/*
	BOOL GetMonitorInfo(
	_In_  HMONITOR      hMonitor,
	_Out_ LPMONITORINFO lpmi
	);
	*/

	MONITORINFOEX info;
	info.cbSize = sizeof(MONITORINFOEX);

	if (GetMonitorInfo(hMonitor, &info)) { //for each monitor
		std::wstring deviceW(info.szDevice);
		std::string device(deviceW.begin(), deviceW.end());

		if (device.compare(param->display) == 0) { //if the monitor's name is the same as the monitor we are looking for.
			DriverLog("Found Monitor Info: %s\n", device.c_str());
			DriverLog("-left: %i\n", info.rcMonitor.left);
			DriverLog("-top: %i\n", info.rcMonitor.top);
			DriverLog("-right: %i\n", info.rcMonitor.right);
			DriverLog("-bottom: %i\n", info.rcMonitor.bottom);

			int32_t width = glm::abs(info.rcMonitor.left - info.rcMonitor.right);
			int32_t height = glm::abs(info.rcMonitor.top - info.rcMonitor.bottom);
			DriverLog("-x: %i\n", info.rcMonitor.left);
			DriverLog("-y: %i\n", info.rcMonitor.top);
			DriverLog("-w: %i\n", width);
			DriverLog("-h: %i\n", height);

			param->caller->SetDisplayInfo(info.rcMonitor.left, info.rcMonitor.top, width, height); //let the one who called this enumeration procedure know about the monitor.
			return false;
		}
	}

	return true;
}
#endif