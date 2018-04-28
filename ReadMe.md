# OpenPSVR
This project aims to create a unified Steam OpenVR driver for the PSVR bundle allowing opensource VR developers to utilize their existing PSVR hardware for PC VR development.
## Build Status
Ubunut 14.04 -  Linux64: [![Build Status](https://travis-ci.org/SuperEvenSteven/OpenPSVR.svg?branch=convert-to-cmake)](https://travis-ci.org/SuperEvenSteven/OpenPSVR)  
Windows 10 VS2017 - Win64: [![Build status](https://ci.appveyor.com/api/projects/status/9rqyc5c3jarsqc0w?svg=true)](https://ci.appveyor.com/project/SuperEvenSteven/openpsvr)   
## Snapshot Downloads
Last successful build from latest commit: [![Snapshot download](https://img.shields.io/badge/download-snapshot-brightgreen.svg)](https://ci.appveyor.com/api/projects/SuperEvenSteven/openpsvr/artifacts/build/Win64/openpsvr/openpsvr-release.zip?branch=convert-to-cmake&job=Environment%3A+arch%3DWin64%3B+Configuration%3A+Release) 
## Build Requirements
##### Windows
Install the following first:
- [Visual Studio](https://www.visualstudio.com/vs/community/)
- [CMake](https://cmake.org/download/) 

##### Linux
Install the following first:
- [CMake](https://cmake.org/download/)
- [libudev-dev](https://packages.ubuntu.com/trusty/libudev-dev)

## Building OpenPSVR
#### Windows
1. Clone repo
2. Install cmake
3. Clean and Generate MS Visual Studio project `clean.bat && generate.bat`
4. Build driver `build.bat`, you should get a 'Build Successful' message. The driver is built and ready to install.
5. Deploy the driver locally into your local installation of SteamVR for testing `deploy.bat`
 
#### Linux
1. Clone repo
2. Install libusbdev required to compile libusb dependency `sudo apt-get install libusbdev-deb`
3. Clean and Generate make files `./clean.sh && ./generate.sh`
4. Build driver `build.sh`
5. Deploy the driver locally (TODO)


### Manual Installation
#### Windows
If you are building the driver from the solution, it should automatically install the driver into the driver folder if you configured the macro's correctly.  
Otherwise, copy/extract the openpsvr folder to your SteamVR\drivers folder.  
The openpsvr within the SteamVR\drivers folder should look like this:
```
openpsvr
    ├── driver.vrdrivermanifest
    ├── bin
    │   ├── win32
    │   │   └── driver_OpenPSVR.dll (be sure to have the "driver_" prefix)
    │   └── win64
    │       └── driver_OpenPSVR.dll (be sure to have the "driver_" prefix)
    └── resources
        ├── driver.vrresources
        ├── icons
        │   ├── headset_sample_status_ready_alert.png (to be changed)
        │   ├── headset_sample_status_ready_low.png (to be changed)
        │   ├── headset_sample_status_searching_alert.gif (to be changed)
        │   ├── headset_sample_status_searching.gif (to be changed)
        │   ├── psvr_error.png
        │   ├── psvr_off.png
        │   ├── psvr_ready.png
        │   └── psvr_standby.png
        └── settings
            └── default.vrsettings
```
Note! You must have at least WinUSB or Libusb drivers installed on at least the PS VR Control device in the device manager for windows.  
The PS VR Sensors device must at least have HID drivers or the WinUSB/Libusb drivers.    
libpsvr provides information on how to do this.

#### Linux
TBD  
(generally the same as the windows installation...)

## Status
#### What works
- Connecting to the PSVR
- Turning the PSVR On/Off
- Setting the PSVR into VR Mode
- Reading Sensor Data
- Finding the monitor for the PSVR (windows only, currently)
- Moving the vr screen to the PSVR Monitor (be sure "activateMultipleDrivers" in your steam.vrsettings is set to true to have this work)

#### What sort of works
- Head tracking (generally converting the sensor data into head tracking)
