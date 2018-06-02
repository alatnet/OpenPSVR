#pragma once

#include <vector>

#include "OpenPSVR_Common.h"

//-----------------------------------------------

#define CAMERA_UUID void * //placeholder, to change
#define CAMERA_FORMAT void * //placeholder, to change, use enum?

//-----------------------------------------------

struct OpenPSVR_CameraImage {
	union {
		struct {
			void * left;
			void * right;
		} stereo;
		void * image;
	};
};

struct OpenPSVR_CameraSettings {
	int width, height;
	int framerate;
	CAMERA_FORMAT format;
	bool stereo;
};

struct OpenPSVR_CameraInfo {
	CAMERA_UUID uuid;
	std::vector<OpenPSVR_CameraSettings> * settings;
};

//-----------------------------------------------

class OpenPSVR_Camera {
public:
	virtual void Init(OpenPSVR_CameraSettings settings) = 0;
	virtual void Shutdown() = 0;
public:
	virtual OpenPSVR_CameraImage * getFrame() = 0;
	virtual OpenPSVR_CameraSettings getSettings() = 0;
};

class OpenPSVR_CameraFactory {
public:
	virtual void Init() = 0;
	virtual void Shutdown() = 0;
public:
	virtual std::vector<OpenPSVR_CameraInfo> getCameras() = 0;
	virtual OpenPSVR_Camera * newCamera(CAMERA_UUID uuid) = 0;
};

//-----------------------------------------------

#define OpenPSVR_CameraPluginEntry newCameraFactory
#define OpenPSVR_CameraPluginEntryStr "newCameraFactory"

#define OpenPSVR_CameraPlugin(classname) \
OPENPSVR_DLL_DECLARE OpenPSVR_CameraFactory * OpenPSVR_CameraPluginEntry() { \
	return new classname; \
}