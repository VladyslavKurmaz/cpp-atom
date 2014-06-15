rem 
set PROJECT_HOME=D:\dev\rg\dev-2.0.0\src\
set CMAKE_EXE=d:\dev\cmake-2.8.12.2-win32-x86\bin\cmake.exe
set DEV_BOOST-1.55.0_INC=d:\dev\boost-1.55.0\
set DEV_BOOST-1.55.0_LIB=d:\dev\boost-1.55.0\stage\lib\
set DEV_LOKI-0.1.7_INC=D:\dev\loki-0.1.7\include\
set DEV_MSVC_10_VARS="%VS100COMNTOOLS%..\..\VC\vcvarsall.bat"

cd ..
mkdir builds
cd builds

call %DEV_MSVC_10_VARS%
call %CMAKE_EXE% -G "Visual Studio 10" %PROJECT_HOME%
call msbuild cpp-atom.sln

