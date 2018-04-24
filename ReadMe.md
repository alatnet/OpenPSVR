# OpenPSVR
## Build Requirements
##### Windows
- Visual Studio

##### Linux
- CMake

## Building OpenPSVR
#### Windows
3rd Party libraries should be automatically downloaded into the 3rdParty folder via git submodule support.  
Make sure that libusb is built and symlinked (prefered) or copied into the 3rdParty folder of libpsvr.  
Also make sure that libpsvr is build after libusb is symlinked/copied and built.  

Open the OpenPSVR.sln file in Visual Studio.  
Modify the paths macro in the property manager (View > Property Manager \[towards the bottom\]\{VS2017\}).  
Batch build both the win32/x86 and x64 versions if you are running the driver on a 64 bit system, otherwise just build the win32/x86 version.  
The driver should automatically install itself in the steam vr driver folder if you modified the path macros.  
If you do not want it to automatically install, delete the command line in the project property's post-build event.

#### Linux
Note: this is still a WIP as some Win32 specific parts need a linux equivalent coded. e.g. stricmp vs strcasecmp etc..
1. Clone repo
2. Initialise git submodules `git submodule update --init`
3. Generate make files `generate.sh`
4. Build driver `build.sh`


### Installation
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
