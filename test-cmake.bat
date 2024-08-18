REM Test windows cmake

REM Remove log file if it exists
if exist test-cmake-win.log del test-cmake-win.log

REM File header
echo "Windows cmake/unittest log file\n" > test-cmake-win.log

REM Test release build
echo "Windows cl cmake Release started"
echo "Windows cl cmake Release\n:" >> test-cmake-win.log
cmake -B build-cl-release -DCMAKE_CXX_COMPILER=cl -DCMAKE_C_COMPILER=cl -DCMAKE_BUILD_TYPE=Release -S .\ >> test-cmake-win.log
if %ERRORLEVEL% NEQ 0 goto Error_Cmake_Release

echo "Windows cl build Release started"
echo "Windows cl build Release\n:" >> test-cmake-win.log
cmake --build .\build-cl-release --config Release >> test-cmake-win.log
if %ERRORLEVEL% NEQ 0 goto Error_Build_Release

REM Test Release Unittest
echo "Windows cl Release unittest started"
echo "Windows cl Release unittest\n:" >> test-cmake-win.log
make -C .\build-cl-release unittest >> test-cmake-win.log
if %ERRORLEVEL% NEQ 0 goto Error_Unittest_Release

REM Test Release Sample 
echo "Windows cl Release sample unittest started"
echo "Windows cl Release sample unittest\n:" >> test-cmake-win.log
pushd .\build-cl-release
.\sample-unittest >> ../test-cmake-win.log
if %ERRORLEVEL% NEQ 0 goto Error_Sample_Unittest_Release
popd

REM Test debug build
echo "Windows cl cmake Debug started"
echo "Windows cl cmake Debug\n:" >> test-cmake-win.log
cmake -B build-cl-debug -DCMAKE_CXX_COMPILER=cl -DCMAKE_C_COMPILER=cl -DCMAKE_BUILD_TYPE=Debug -S .\ >> test-cmake-win.log
if %ERRORLEVEL% NEQ 0 goto Error_Cmake_Debug

echo "Windows cl build Debug started"
echo "Windows cl build Debug\n:" >> test-cmake-win.log
cmake --build .\build-cl-debug --config Debug >> test-cmake-win.log
if %ERRORLEVEL% NEQ 0 goto Error_Build_Debug

REM Test Debug Unittest
echo "Windows cl Debug unittest started"
echo "Windows cl Debug unittest\n:" >> test-cmake-win.log
make -C .\build-cl-debug unittest >> test-cmake-win.log
if %ERRORLEVEL% NEQ 0 goto Error_Unittest_Debug

REM Test Debug Sample 
echo "Windows cl Debug sample unittest started"
echo "Windows cl Debug sample unittest\n:" >> test-cmake-win.log
pushd .\build-cl-debug
.\sample-unittest >> ../test-cmake-win.log
if %ERRORLEVEL% NEQ 0 goto Error_Sample_Unittest_Debug
popd

echo "Windows cmake/unittest complete, no errors"
goto End

:Error_Cmake_Release
echo "Windows cl cmake Release failed"
goto End

:Error_Build_Release
echo "Windows cl build Release failed"
goto End

:Error_Unittest_Release
echo "Windows cl Release unittest failed"
goto End

:Error_Sample_Unittest_Release
popd
echo "Windows cl Release sample unittest failed"
goto End

:Error_Cmake_Debug
echo "Windows cl cmake Debug failed"
goto End

:Error_Build_Debug
echo "Windows cl build Debug failed"
goto End

:Error_Unittest_Debug
echo "Windows cl Debug unittest failed"
goto End

:Error_Sample_Unittest_Debug
popd
echo "Windows cl Debug sample unittest failed"
goto End

:End
