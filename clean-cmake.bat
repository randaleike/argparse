REM Cleanup build remains

REM check for build directories
if exist .\build-release\ (
    rmdir /S /Q .\build-release
)
if exist .\build-debug\ (
    rmdir /S /Q .\build-debug
)
if exist .\build\ (
    rmdir /S /Q .\build
)
if exist .\build-cl-release\ (
    rmdir /S /Q .\build-gcc-release
)
if exist .\build-cl-debug\ (
    rmdir /S /Q .\build-gcc-debug
)

REM Remove log file if it exists
if exist test-cmake-win.log (
    del test-cmake-win.log
)

REM Remove cache if it exists
if exist .\.cache\ (
    rmdir /S /Q .\.cache
)
