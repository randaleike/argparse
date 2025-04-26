REM Check the input
if [%1] == [Debug] goto checkBuild
if [%1] == [Release] goto checkBuild

echo "Invalid input!"
echo "Usage: make-all.bat <Debug|Release>"
goto end

:checkBuild
SET BUILD_DIR=build
if NOT [%2] == [] SET BUILD_DIR=%2

:startBuild
REM Make libraries
cmake --build %BUILD_DIR% --config %1

REM Make the samples
cmake --build %BUILD_DIR% --config %1 --target samples

REM Make the samples unittest
cmake --build %BUILD_DIR% --config %1 --target samples-unittest

REM Run the library unittests
do (
    cd .\%BUILD_DIR%\
    copy .\bin\%1\*.* .\%1
    ctest --build-config %1 --exclude-regex sample
)

:end