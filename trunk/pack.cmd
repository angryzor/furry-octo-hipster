@echo off

echo Packing release...
del release.zip

md packing
md packing\tf
xcopy "tf" "packing\tf" /S /E /Y

winrar a release.zip -m5 installtools install.cmd uninstall.cmd "README IM SERIOUS.txt" glitches.txt

pushd packing
winrar a ..\release.zip -m5 tf
popd

rd /S /Q "packing"

echo Done.
pause
