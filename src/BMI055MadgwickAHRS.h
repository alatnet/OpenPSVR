#include <glm.hpp>
#include <gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/euler_angles.hpp>

class BMI055MadgwickAHRS {
public:
	BMI055MadgwickAHRS(glm::quat InitialPose);

public:
	void Update(float gx, float gy, float gz, float ax, float ay, float az, float Beta, float SamplePeriod);

public:
	glm::quat Quat;
};