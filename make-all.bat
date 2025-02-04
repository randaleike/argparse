REM Check the input
if [%1] == [Debug] goto startBuild
if [%1] == [Release] goto startBuild

echo "Invalid input!"
echo "Usage: make-all.bat <Debug|Release>"
goto end

:startBuild
REM Make libraries
cmake --build build --config %1

REM Make library unittests
cmake --build build --config %1 --target build-unittest

REM Run the library unittests
do (
    cd .\build\
    ctest --build-config %1 --exclude-regex samples
)

REM Make the samples
cmake --build build --config %1 --target samples

REM Make the samples unittest
cmake --build build --config %1 --target samples-unittest

REM Run the samples unittests
do(
    cd .\build\
    ctest --build-config %1 --tests-regex samples
)

:end