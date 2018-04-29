@echo off

::Build driver
call :unregisterDriver || goto handleError

::Exit batch script
goto exit

::---------------------------
::|Function definitions below
::---------------------------

::Function unregisters the driver with SteamVR
:unregisterDriver
echo "Un-registering SteamVR Win64 driver..."
echo "Driver directory:%CD%"
"..\..\bin\win64\vrpathreg" removedriver "%CD%"
goto:eof

:handleError
echo "Failed to unregister driver"
exit /b 1
goto:eof

:exit
echo "Driver successfully unregistered"
exit /b 0
goto:eof