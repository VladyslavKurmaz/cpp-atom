echo off

set BUILD_TOOLSET=%1
set BUILD_CONFIGURATION=%2
set BUILD_DEST=build-%BUILD_TOOLSET%
set BUILD_FOLDER=%BUILD_DEST%-%BUILD_CONFIGURATION%

if "%BUILD_TOOLSET%"=="msvc14" (
	set BUILD_ENV="%VS140COMNTOOLS%..\..\VC\vcvarsall.bat"
	set BUILD_CMAKE_CODE="Visual Studio 14 2015"
	set BUILD_BOOST_TOOLSET="msvc-14.0"
)
if "%BUILD_TOOLSET%"=="msvc12" (
	set BUILD_ENV="%VS120COMNTOOLS%..\..\VC\vcvarsall.bat"
	set BUILD_CMAKE_CODE="Visual Studio 12"
	set BUILD_BOOST_TOOLSET="msvc-12.0"
)
if "%BUILD_TOOLSET%"=="msvc11" (
	set BUILD_ENV="%VS110COMNTOOLS%..\..\VC\vcvarsall.bat"
	set BUILD_CMAKE_CODE="Visual Studio 11"
	set BUILD_BOOST_TOOLSET="msvc-11.0"
)
if "%BUILD_TOOLSET%"=="msvc10" (
	set BUILD_ENV="%VS100COMNTOOLS%..\..\VC\vcvarsall.bat"
	set BUILD_CMAKE_CODE="Visual Studio 10 2010"
	set BUILD_BOOST_TOOLSET="msvc-10.0"
)

set DEV_BOOST-1.58.0_INC=%DEV_HOME%\boost-1.58.0
set DEV_BOOST-1.58.0_LIB=%DEV_HOME%\boost-1.58.0\stage\lib
set DEV_LOKI-0.1.7_INC=%DEV_HOME%\loki-0.1.7\include

set DEV_CMAKE-3.2.3-WIN32-X86=%DEV_HOME%\cmake-3.2.3-win32-x86\bin\cmake.exe
set DEV_CMAKE=%DEV_CMAKE-3.2.3-WIN32-X86%

call %BUILD_ENV%

cd ..
mkdir %BUILD_FOLDER%
cd %BUILD_FOLDER%
call %DEV_CMAKE% -G %BUILD_CMAKE_CODE% .\..\src
call msbuild cpp-atom.sln /p:Configuration=%BUILD_CONFIGURATION%
