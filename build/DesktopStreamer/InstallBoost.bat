@ECHO OFF

rem 2020/12/08 sjyoon

set PLATFORM_TOOLSET=%~1
set LIBRARY_PATH=%~2

if  %PLATFORM_TOOLSET% EQU v142 (
  set TOOLSET=14.2
) else if %PLATFORM_TOOLSET% EQU v141 (
  set TOOLSET=14.1
) else if %PLATFORM_TOOLSET% EQU v140 (
  set TOOLSET=14.0
) else if %PLATFORM_TOOLSET% EQU v130 (
  set TOOLSET=13.0
) else if %PLATFORM_TOOLSET% EQU v120 (
  set TOOLSET=12.0
) else if %PLATFORM_TOOLSET% EQU v110 (
  set TOOLSET=11.0
)

cd "%LIBRARY_PATH%"

echo boost build checking...
set BUILD_ARGUMENT=toolset=msvc-%TOOLSET% variant=debug,release link=static threading=multi runtime-link=static address-model=32,64 --no-cmake-config

if not exist %LIBRARY_PATH%stage\ (

  echo ==================Boost Build==================
  echo Toolset        : %TOOLSET%
  echo Library Path   : %LIBRARY_PATH%

  echo Build boost. It takes quite a while, so please wait without closing.

  echo Creating b2.exe...
  call bootstrap.bat

  echo .\b2.exe %BUILD_ARGUMENT%
  .\b2.exe %BUILD_ARGUMENT%

  echo ================boost Build End================
)

