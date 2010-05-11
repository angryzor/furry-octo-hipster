@echo off
:start
echo Trying to find TF2 location...
if not "%TF2LOC%"=="" (
	echo TF2 location already known: "%TF2LOC%"
	exit /B
)

echo Reading registry for Steam location.
for /F "tokens=1,2,*" %%i in ('reg query HKCU\Software\Valve\Steam /v SteamPath') do if "%%i"=="SteamPath" set STEAMLOC=%%k

if "%STEAMLOC%"=="" (
	echo Steam location could not be deduced from registry!
	set /P "STEAMLOC=> Please enter location of steam folder manually: "
)

rem Registry weirdly keeps path with slashes instead of backslashes
rem this sometimes gives problems so we get rid of this with a trick
rem It also makes sure we don't have any trailing backslashes

pushd "%STEAMLOC%"
set STEAMLOC=%CD%
popd

echo Steam folder is "%STEAMLOC%"



set /P "STEAMUSER=> Please enter your Steam login username (NOT screen name): "

set TF2LOC=%STEAMLOC%\steamapps\%STEAMUSER%\team fortress 2

echo TF2 is at location "%TF2LOC%"

if not exist "%TF2LOC%" (
	echo ----------------------------------------------------------------------------------
	echo The folder "%TF2LOC%" cannot be found!!! Retrying TF2 folder finding routine...  
	echo To abort install instead of retrying, press Ctrl-C and choose Y.
	echo ----------------------------------------------------------------------------------
	set STEAMLOC=
	set TF2LOC=
	goto start
)
