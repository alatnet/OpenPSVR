 [![Build status](https://ci.appveyor.com/api/projects/status/o78wh7jv4k5rim2k/branch/master?svg=true)](https://ci.appveyor.com/project/SuperEvenSteven/openpsvr) [![Build Status](https://travis-ci.org/SuperEvenSteven/OpenPSVR.svg?branch=master)](https://travis-ci.org/SuperEvenSteven/OpenPSVR) [![Snapshot download](https://img.shields.io/badge/download-snapshot-brightgreen.svg)](https://ci.appveyor.com/api/projects/SuperEvenSteven/openpsvr/artifacts/build/Win64/openpsvr/openpsvr-release.zip?branch=master&job=Environment%3A+arch%3DWin64%3B+Configuration%3A+Release)

# OpenPSVR
This project aims to create a unified Steam OpenVR driver for the PSVR bundle.
  
The following features will be added:
* PSVR HMD display, orientation and tracking support 
* PS Move/Navi/DS4 controller support
* Generic Stereoscopic camera support

## Building from Source
### Build Requirements
Install the following environment specific requirements.
#### Windows
- [Visual Studio 2017](https://www.visualstudio.com/vs/community/)
- [CMake](https://cmake.org/download/)
  
**Note**: You must have at least WinUSB or Libusb drivers installed on at least the PS VR Control device in the device manager for windows. The PS VR Sensors device must at least have HID drivers or the WinUSB/Libusb drivers. Libpsvr provides information on how to do this.

#### Linux
- [CMake](https://cmake.org/download/)
- [libudev-dev](https://packages.ubuntu.com/trusty/libudev-dev)

### Build System
#### Project Structure
```
OpenPSVR
    ├── cmake
    │     └── <build definition>
    ├── driver
    │     └── <driver resources & scripts>
    ├── installer
    │     └── <installer definition>
    ├── src
    │     └── <source files>
    ├── clean.(bat|sh)
    ├── generate.(bat|sh)
    ├── build.(bat|sh)
    └── CMakeLists.txt
```

The project's build process is scripted in OS specific shell/batch scripts that affect only the files and folders contained in the OpenPSVR project folder. This is done to keep the project self-contained and easily built by multi-platform continuous integration build servers (appveyor & travis).

#### Build System Scripts
The build scripts used to clean, generate and build the source code are:

* *clean* - cleans the project folder by deleting the deps, generated and build folder
* *generate* - downloads dependencies and generates project files
* *build* - compiles the driver for use in SteamVR

The generated folders are:

* *deps* - the place where dependencies are downloaded to
* *generated* - CMake creates the project files i.e. MSVS solution or GNU Make folder.
* *build* - the compiled driver binaries and driver resources are placed here

The process is defined below simply as:

1. Generate project files
2. Download dependencies (3rd-party libraries)
3. Build project binaries (the driver)

**Note**: the use of [Microsoft VCPKG](https://github.com/Microsoft/vcpkg) appears to be a similar simplified solution for future consideration. Using this would simplify defining the header libraries, and binary path definitions.

### Building the Source Code
#### Using CMake
For those comfortable with using a terminal or command prompt:
##### Windows
1. Clone repo `git clone https://github.com/alatnet/OpenPSVR.git`
2. Install [CMake](https://cmake.org/download/) and [Visual Studio 2017](https://www.visualstudio.com/vs/community/)
3. Clean and Generate MS Visual Studio project `clean.bat && generate.bat`
4. Build driver `build.bat`, you should get a `Build Successful` message. The driver is built and ready to install.
5. Deploy the driver locally into your local installation of SteamVR for testing `deploy.bat`

##### Linux
1. Clone repo `git clone https://github.com/alatnet/OpenPSVR.git`
2. Install libusbdev required to compile libusb dependency `sudo apt-get install libudev-deb`
3. Clean and Generate make files `./clean.sh && ./generate.sh`
4. Build driver `build.sh`, you should get a `Build Successful` message. The driver is built and ready to install.
5. Deploy the driver locally (TODO)

### Modifying the Source Code
Changes to source code should always be done from the `src` folder and not the `generated` source folder.
  
Run the `build` script to quickly compile your change. If changes are made to the CMake scripts you will need to `clean && generate && build`

### Installing the Driver in SteamVR
#### Automated Installer
In the installer folder of this project is an XML definition file for the BitRock installer project. This is a cross-platform installer that offer free licenses for opensource projects. This project's installer is built using that.

[![alt text][bitrock-logo]][bitrock-url]
#### Manually Installing
Either run the `deploy` script or manually copy the openpsvr folder, found in `build\<arch>\openpsvr`, to your SteamVR drivers folder `<Steam_installation>/steamapps/common/SteamVR/drivers/`.
### OpenVR Driver Structure
Once the project source is built you'll find the driver in the `build` folder. The openpsvr driver structure  within the SteamVR\drivers folder should look like this:
```
openpsvr
    ├── driver.vrdrivermanifest
    ├── register.bat
    ├── unregister.bat
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

## Status
The project can be built on both Windows and Ubuntu but has only been tested on Windows. Also the installer is currently only tested on Windows.

#### What works
- Connecting to the PSVR
- Turning the PSVR On/Off
- Setting the PSVR into VR Mode
- Reading Sensor Data
- Finding the monitor for the PSVR (windows only, currently)
- Moving the vr screen to the PSVR Monitor (be sure "activateMultipleDrivers" in your steam.vrsettings is set to true to have this work)

#### What sort of works
- Head tracking (generally converting the sensor data into head tracking)

[bitrock-logo]:https://github.com/alatnet/OpenPSVR/blob/master/installer/installer-logo.png "BitRock Installer"
[bitrock-url]:http://bitrock.com
