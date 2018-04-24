@echo off
setlocal

::Initialise
set PROJECT_ROOT=%cd%

::Clean
call :cleanProjectFolder || goto handleError
call :cleanBuildFolder || goto handleError

::Exit batch script
goto exit

::---------------------------
::|Function definitions below
::---------------------------

::Clean up the old generated project folder
:cleanProjectFolder
IF EXIST %PROJECT_ROOT%\generated (
echo "Cleaning old generated folder..."
del /f /s /q %PROJECT_ROOT%\generated > nul
rmdir /s /q %PROJECT_ROOT%\generated
)
goto:eof

::Clean up the old build project folder
:cleanBuildFolder
IF EXIST %PROJECT_ROOT%\build (
echo "Cleaning old build folder..."
del /f /s /q %PROJECT_ROOT%\build > nul
rmdir /s /q %PROJECT_ROOT%\build
)
goto:eof

:handleError
echo "CLEAN FAILED"
endlocal
exit /b 1
goto:eof

:exit
echo "CLEAN SUCCESSFUL"
endlocal
exit /b 0
goto:eof
