REM Cleanup build remains

REM check for build directories
if exist .\build\ (
    rmdir /S /Q .\build
)

REM Check the input
if [%1] == [Debug] goto startBuild
if [%1] == [Release] goto startBuild

echo "Invalid input!"
echo "Usage: make-clean.bat <Debug|Release>"
goto end

:startBuild
REM Make the make files
cmake -B build -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc -DCMAKE_BUILD_TYPE=%1 -S .

REM call make all
call .\make-all.bat "%1"
:end