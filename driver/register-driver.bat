@echo off

::Build driver
call :registerDriver || goto handleError

::Exit batch script
goto exit

::---------------------------
::|Function definitions below
::---------------------------

::Function registers the driver with SteamVR
:registerDriver
echo "Registering SteamVR Win64 driver..."
echo "Driver directory:%CD%"
"..\..\bin\win64\vrpathreg" adddriver "%CD%"
goto:eof

:handleError
echo "Failed to register driver"
exit /b 1
goto:eof

:exit
echo "Driver successfully registered"
exit /b 0
goto:eof