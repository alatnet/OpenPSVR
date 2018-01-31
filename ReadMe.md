# OpenPSVR
## Build Requirements
- [libpsvr](https://github.com/alatnet/libpsvr)
- [libusb](https://github.com/libusb/libusb)
- [glm](https://glm.g-truc.net)
- [openvr](https://github.com/ValveSoftware/openvr/)

##### Windows
- Visual Studio

##### Linux
- TBD

## Building OpenPSVR
Place the libraries listed above in the 3rdParty folder.  
Read the readme in that folder on how to place the files themselves.

#### Windows
Open the OpenPSVR.sln file in Visual Studio.  
Modify the paths macro in the property manager (View > Property Manager \[towards the bottom\]\{VS2017\}).  
Batch build both the win32/x86 and x64 versions if you are running the driver on a 64 bit system, otherwise just build the win32/x86 version.  
The driver should automatically install itself in the steam vr driver folder if you modified the path macros.  
If you do not want it to automatically install, delete the command line in the project property's post-build event.

#### Linux
TBD

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
