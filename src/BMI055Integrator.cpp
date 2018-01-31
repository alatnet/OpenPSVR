#include "BMI055Integrator.h"

BMI055Integrator::BMI055Integrator(float AccelerometerScale, float GyroscopeScale) {
	aRes = AccelerometerScale;
	gRes = GyroscopeScale;

	calibrating = true;
	recalibrate = false;
	recenter = false;

	gravityVec = glm::vec3(0.0f, 0.0f, 0.0f);
	accelOffset = glm::vec3(0.0f, 0.0f, 0.0f);
	gyroOffset = glm::vec3(0.0f, 0.0f, 0.0f);

	samplesLeft = 2000;

	prevTimestamp = 0;

	fusion = new BMI055MadgwickAHRS(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	ZeroPose = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
}

BMI055Integrator::~BMI055Integrator() {
	if (fusion) delete fusion;
}

void BMI055Integrator::Recalibrate() {
	calibrating = true;
	recalibrate = true;
}

void BMI055Integrator::Recenter() {
	recenter = true;
}

glm::quat BMI055Integrator::Parse(glm::vec3 *gyro, glm::vec3 *accel, uint32_t *timestamp) {
	if (recalibrate) {
		samplesLeft = 2000;
		accelOffset = glm::vec3(0.0f, 0.0f, 0.0f);
		gyroOffset = glm::vec3(0.0f, 0.0f, 0.0f);
		recalibrate = false;
	}

	this->Integrate(accel[0] * aRes, gyro[0] * gRes, timestamp[0]);
	return this->Integrate(accel[1] * aRes, gyro[1] * gRes, timestamp[1]);
}

glm::quat BMI055Integrator::Integrate(glm::vec3 linearAccel, glm::vec3 angularAccel, uint32_t timestamp) {
	if (samplesLeft > 0) {
		samplesLeft--;
		accelOffset += linearAccel;
		gyroOffset += angularAccel;
		return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	} else if (samplesLeft == 0) {
		samplesLeft--;
		accelOffset /= 2000;
		gyroOffset /= 2000;
		gravityVec = glm::normalize(accelOffset);
		accelOffset -= gravityVec;
		prevTimestamp = timestamp;

		if (fusion) delete fusion;
		fusion = new BMI055MadgwickAHRS(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
		return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	} else if (samplesLeft > -1500) {
		samplesLeft--;

		linearAccel = glm::normalize(linearAccel - accelOffset);
		angularAccel -= gyroOffset;

		float interval = 0;

		if (prevTimestamp > timestamp)
			interval = (timestamp + (0xFFFFFF - prevTimestamp)) / 1000000.0f;
		else
			interval = (timestamp - prevTimestamp) / 1000000.0f;

		fusion->Update(angularAccel.x, angularAccel.y, angularAccel.z, linearAccel.x, linearAccel.y, linearAccel.z, 1.5f, interval);
		prevTimestamp = timestamp;
		return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	} else if (samplesLeft > -2000) {
		samplesLeft--;

		linearAccel = glm::normalize(linearAccel - accelOffset);
		angularAccel -= gyroOffset;

		float interval = 0;

		if (prevTimestamp > timestamp)
			interval = (timestamp + (0xFFFFFF - prevTimestamp)) / 1000000.0f;
		else
			interval = (timestamp - prevTimestamp) / 1000000.0f;

		fusion->Update(angularAccel.x, angularAccel.y, angularAccel.z, linearAccel.x, linearAccel.y, linearAccel.z, 0.05f, interval);
		prevTimestamp = timestamp;
		return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	} else if (samplesLeft == -2000) {
		samplesLeft--;

		linearAccel = glm::normalize(linearAccel - accelOffset);
		angularAccel -= gyroOffset;

		float interval = 0;

		if (prevTimestamp > timestamp)
			interval = (timestamp + (0xFFFFFF - prevTimestamp)) / 1000000.0f;
		else
			interval = (timestamp - prevTimestamp) / 1000000.0f;

		fusion->Update(angularAccel.x, angularAccel.y, angularAccel.z, linearAccel.x, linearAccel.y, linearAccel.z, 0.035f, interval);
		prevTimestamp = timestamp;

		ZeroPose = glm::quat(1.0f, 0.0f, 0.0f, 0.0f) * glm::inverse(fusion->Quat);

		calibrating = false;

		return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	} else {
		linearAccel = glm::normalize(linearAccel - accelOffset);
		angularAccel -= gyroOffset;

		float interval = 0;

		if (prevTimestamp > timestamp)
			interval = (timestamp + (0xFFFFFF - prevTimestamp)) / 1000000.0f;
		else
			interval = (timestamp - prevTimestamp) / 1000000.0f;

		fusion->Update(angularAccel.x, angularAccel.y, angularAccel.z, linearAccel.x, linearAccel.y, linearAccel.z, 0.035f, interval);
		prevTimestamp = timestamp;

		if (recenter) {
			ZeroPose = glm::quat(1.0f, 0.0f, 0.0f, 0.0f) * glm::inverse(fusion->Quat);
			recenter = false;
		}

		return glm::inverse(ZeroPose * fusion->Quat);
	}
}

glm::vec3 BMI055Integrator::toEuler(glm::quat Q) {
	glm::vec3 pitchYawRoll = glm::vec3(0.0f, 0.0f, 0.0f);

	float sqw = Q.w * Q.w;
	float sqx = Q.x * Q.x;
	float sqy = Q.y * Q.y;
	float sqz = Q.z * Q.z;

	pitchYawRoll.x = (float)atan2((2.0f * Q.x * Q.w + 2.0f * Q.y * Q.z), (1.0f - 2.0f * (sqz + sqw))); //Yaw
	pitchYawRoll.y = -(float)asin(2.0f * (Q.x * Q.z - Q.w * Q.y)); // Pitch
	pitchYawRoll.z = (float)atan2(2.0f * Q.x * Q.y + 2.0f * Q.z * Q.w, 1.0f - 2.0f * (sqy + sqz)); // Roll

	return pitchYawRoll;
}