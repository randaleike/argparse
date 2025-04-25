REM Cleanup build remains

REM Remove cache if it exists
if exist .\.cache\ (
    rmdir .\.cache
)
REM Remove Testing
if exist .\Testing\ (
    rmdir .\Testing
)

REM Check the input
if [%1] == [Debug] goto checkBuild
if [%1] == [Release] goto checkBuild

echo "Invalid input!"
echo "Usage: make-clean.bat <Debug|Release>"
goto end

:checkBuild
SET BUILD_DIR=build
if NOT [%2] == [] SET BUILD_DIR=%2

REM check for build directories
if exist .\%BUILD_DIR%\ (
    rmdir .\%BUILD_DIR%
)

:startBuild
REM Make the make files
cmake -B %BUILD_DIR% -DCMAKE_CXX_COMPILER=cl -DCMAKE_C_COMPILER=cl -DCMAKE_BUILD_TYPE=%1 -S .

REM Make libraries
cmake --build %BUILD_DIR% --config %1

REM Make the samples
cmake --build %BUILD_DIR% --config %1 --target samples

REM Make the samples unittest
cmake --build %BUILD_DIR% --config %1 --target samples-unittest

set /p runctest=Run ctest? [y/n]:
if [%runctest%] == [n] goto end
if [%runctest%] == [N] goto end
if [%runctest%] == [no] goto end
if [%runctest%] == [No] goto end
if [%runctest%] == [NO] goto end

REM Run the library unittests
do (
    cd .\%BUILD_DIR%\
    copy .\bin\%1\*.* .\%1
    ctest --build-config %1 --exclude-regex sample
)

:end