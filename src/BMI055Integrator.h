#include "BMI055MadgwickAHRS.h"

#include <glm.hpp>
#include <gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/euler_angles.hpp>

constexpr float GFS_125DPS = (float)(0.00381f * (glm::pi<float>() / 180.0f));
constexpr float GFS_250DPS = (float)(0.007622f * (glm::pi<float>() / 180.0f));
constexpr float GFS_500DPS = (float)(0.01524f * (glm::pi<float>() / 180.0f));
constexpr float GFS_1000DPS = (float)(0.03048f * (glm::pi<float>() / 180.0f));
constexpr float GFS_2000DPS = (float)(0.06097f * (glm::pi<float>() / 180.0f));

constexpr float AFS_2G = 2.0f / 2048.0f;
constexpr float AFS_4G = 4.0f / 2048.0f;
constexpr float AFS_8G = 8.0f / 2048.0f;
constexpr float AFS_16G = 16.0f / 2048.0f;

class BMI055Integrator {
public:
	BMI055Integrator(float AccelerometerScale, float GyroscopeScale);
	~BMI055Integrator();

public:
	void Recalibrate();
	void Recenter();

	glm::quat Parse(glm::vec3 *gyro, glm::vec3 *accel, uint32_t *timestamp); //requires 2 data sets
	glm::vec3 ParseToEuler(glm::vec3 *gyro, glm::vec3 *accel, uint32_t *timestamp) { return toEuler(this->Parse(gyro, accel, timestamp)); } //requires 2 data sets

	static glm::vec3 toEuler(glm::quat Q);

public:
	bool isCalibrating() { return this->calibrating; }

private:
	glm::quat Integrate(glm::vec3 linearAccel, glm::vec3 angularAccel, uint32_t timestamp);

private:
	float aRes;
	float gRes;

	glm::vec3 gravityVec;
	glm::vec3 accelOffset;
	glm::vec3 gyroOffset;

	int samplesLeft;

	uint32_t prevTimestamp;

	BMI055MadgwickAHRS *fusion;
	glm::quat ZeroPose;

	bool recalibrate;
	bool recenter;
	bool calibrating;
};