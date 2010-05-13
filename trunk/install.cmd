@echo off
pushd installtools
call gettf2loc.cmd
popd

echo Installing mod...

rd /S /Q "%TF2LOC%\tf\materials\vgui\logos\visual_improve_hud"

xcopy /S /E /Y "tf" "%TF2LOC%\tf"

pushd installtools
call modtexfix.cmd
popd

echo Done.
pause
