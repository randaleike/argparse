REM Cleanup build remains

REM check for build directories
if exist .\build\ (
    rmdir /S /Q .\build
)
REM Remove cache if it exists
if exist .\.cache\ (
    rmdir /S /Q .\.cache
)
REM Remove Testing
if exist .\Testing\ (
    rmdir /S /Q .\Testing
)

REM Check the input
if [%1] == [Debug] goto startBuild
if [%1] == [Release] goto startBuild

echo "Invalid input!"
echo "Usage: make-clean.bat <Debug|Release>"
goto end

:startBuild
REM Make the make files
cmake -B build -DCMAKE_CXX_COMPILER=cl -DCMAKE_C_COMPILER=cl -DCMAKE_BUILD_TYPE=%1 -S .

REM Make libraries
cmake --build build --config %1

REM Make library unittests
cmake --build build --config %1 --target build-unittest

REM Make the samples
cmake --build build --config %1 --target samples

REM Run the library unittests
do (
    cd .\build\
    ctest --build-config %1 --exclude-regex samples
)

:end