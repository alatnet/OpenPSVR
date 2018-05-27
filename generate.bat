@echo off

::Initialise
set PROJECT_ROOT=%cd%

::Generate the project files for PSMoveService
call :generateProjectFiles || goto handleError

::Exit batch script
goto exit

::Function generates project files for the configured ide
:generateProjectFiles
@echo off
IF NOT EXIST %PROJECT_ROOT%\generated mkdir %PROJECT_ROOT%\generated
pushd %PROJECT_ROOT%\generated
echo "Building project files..."
echo "Running cmake in %PROJECT_ROOT%"
cmake .. -G "Visual Studio 15 2017 Win64"
popd
goto:eof

:handleError
echo "GENERATE FAILED"
exit /b 1
goto:eof

:exit
echo "GENERATE SUCCESSFUL"
exit /b 0
goto:eof

