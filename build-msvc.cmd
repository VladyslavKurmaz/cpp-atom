echo off

set BUILD_TOOLSET=%1
set BUILD_CONFIGURATION=%2
set BUILD_DEST=build-%BUILD_TOOLSET%

if "%BUILD_TOOLSET%"=="msvc11" (
	set BUILD_ENV="%VS110COMNTOOLS%..\..\VC\vcvarsall.bat"
	set BUILD_CMAKE_CODE="Visual Studio 11"
)
if "%BUILD_TOOLSET%"=="msvc10" (
	set BUILD_ENV="%VS100COMNTOOLS%..\..\VC\vcvarsall.bat"
	set BUILD_CMAKE_CODE="Visual Studio 10"
)

set DEV_BOOST-1.57.0_INC=%DEV_HOME%\boost-1.57.0
set DEV_BOOST-1.57.0_LIB=%DEV_HOME%\boost-1.57.0\stage\lib
set DEV_LOKI-0.1.7_INC=%DEV_HOME%\loki-0.1.7\include
set DEV_CMAKE-2.8.12.2-WIN32-X86=%DEV_HOME%\cmake-2.8.12.2-win32-x86\bin\cmake.exe

set BUILD_FOLDER=%BUILD_DEST%-%BUILD_CONFIGURATION%

call %BUILD_ENV%

cd ..
mkdir %BUILD_FOLDER%
cd %BUILD_FOLDER%
call %DEV_CMAKE-2.8.12.2-WIN32-X86% -G %BUILD_CMAKE_CODE% .\..\src
call msbuild cpp-atom.sln /p:Configuration=%BUILD_CONFIGURATION%
