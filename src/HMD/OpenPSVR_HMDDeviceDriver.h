#pragma once

#include <openvr_driver.h>
#include "../driverlog.h"

#include <psvr.h>

#include <thread>
#include <mutex>
#include <chrono>

#include <glm.hpp>
#include <gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/euler_angles.hpp>

#include "../Algorithms/MahonyAHRS.h"
#include "../Algorithms/MadgwickAHRS.h"
#include "../Algorithms/BMI055Integrator.h"

using namespace vr;

class COpenPSVR_HMDDeviceDriver : public vr::ITrackedDeviceServerDriver, public vr::IVRDisplayComponent {
public:
	COpenPSVR_HMDDeviceDriver(psvr_context* psvr_ctx);
	virtual ~COpenPSVR_HMDDeviceDriver();

public:
	// Inherited via ITrackedDeviceServerDriver
	virtual EVRInitError Activate(uint32_t unObjectId) override;
	virtual void Deactivate() override;
	virtual void EnterStandby() override;
	virtual void * GetComponent(const char * pchComponentNameAndVersion) override;
	virtual void DebugRequest(const char * pchRequest, char * pchResponseBuffer, uint32_t unResponseBufferSize) override;
	virtual DriverPose_t GetPose() override;

public:
	// Inherited via IVRDisplayComponent
	virtual void GetWindowBounds(int32_t * pnX, int32_t * pnY, uint32_t * pnWidth, uint32_t * pnHeight) override;
	virtual bool IsDisplayOnDesktop() override { return true; }
	virtual bool IsDisplayRealDisplay() override { return true; }
	virtual void GetRecommendedRenderTargetSize(uint32_t * pnWidth, uint32_t * pnHeight) override;
	virtual void GetEyeOutputViewport(EVREye eEye, uint32_t * pnX, uint32_t * pnY, uint32_t * pnWidth, uint32_t * pnHeight) override;
	virtual void GetProjectionRaw(EVREye eEye, float * pfLeft, float * pfRight, float * pfTop, float * pfBottom) override;
	virtual DistortionCoordinates_t ComputeDistortion(EVREye eEye, float fU, float fV) override;

public:
	void RunFrame();
	std::string GetSerialNumber() const { return m_sSerialNumber; }

private:
	void sensorUpdate();
	void controlUpdate();

	std::thread *mp_sensorThread;
	std::mutex *mp_sensorMutex;
	//std::thread *mp_controlThread;
	bool m_brunning;

private:
	void FindHMDMonitor();

public:
	void SetDisplayInfo(int32_t x, int32_t y, int32_t w = 1920, int32_t h = 1080);

private:
	vr::TrackedDeviceIndex_t m_unObjectId;
	vr::PropertyContainerHandle_t m_ulPropertyContainer;

	std::string m_sSerialNumber;
	std::string m_sModelNumber;

	int32_t m_nWindowX;
	int32_t m_nWindowY;
	int32_t m_nWindowWidth;
	int32_t m_nWindowHeight;
	int32_t m_nRenderWidth;
	int32_t m_nRenderHeight;
	float m_flSecondsFromVsyncToPhotons;
	float m_flDisplayFrequency;
	float m_flIPD;

private:
	psvr_context * mp_psvr_ctx;
	psvr_sensor_frame m_sensorFrame;
private:
	class Timer {
	public:
		Timer() : beg_(clock_::now()) {}
		void reset();
		double elapsed() const;

	private:
		typedef std::chrono::high_resolution_clock clock_;
		typedef std::chrono::duration<double, std::ratio<1, 1000>> millisecond_;
		std::chrono::time_point<clock_> beg_;
	};

	Timer m_poseTimer;
	//Mahony ahrsFilter[2];
	//Madgwick ahrsFilter[2];

	//glm::vec3 angle[2];

	BMI055Integrator *integrator;

private:
	inline static const glm::quat fixQuat(glm::quat quat) {
		//cant figure out the math to swap x and y axis in quaternion, doing with euler angles
		auto tmp = glm::eulerAngles(quat);
		return glm::normalize(glm::quat_cast(glm::eulerAngleYXZ(-tmp.x, -tmp.y, -tmp.z)));
	}

	static inline HmdQuaternion_t HmdQuaternion_Init(double w, double x, double y, double z) {
		HmdQuaternion_t quat;
		quat.w = w;
		quat.x = x;
		quat.y = y;
		quat.z = z;
		return quat;
	}

	static inline void HmdMatrix_SetIdentity(HmdMatrix34_t *pMatrix) {
		pMatrix->m[0][0] = 1.f;
		pMatrix->m[0][1] = 0.f;
		pMatrix->m[0][2] = 0.f;
		pMatrix->m[0][3] = 0.f;
		pMatrix->m[1][0] = 0.f;
		pMatrix->m[1][1] = 1.f;
		pMatrix->m[1][2] = 0.f;
		pMatrix->m[1][3] = 0.f;
		pMatrix->m[2][0] = 0.f;
		pMatrix->m[2][1] = 0.f;
		pMatrix->m[2][2] = 1.f;
		pMatrix->m[2][3] = 0.f;
	}
};