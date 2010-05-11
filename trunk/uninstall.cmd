@echo off
pushd installtools
call gettf2loc.cmd
popd

echo Uninstalling mod...

rd /S /Q "%TF2LOC%\tf\materials\vgui\logos\visual_improve_hud"
del /Q "%TF2LOC%\tf\resource\ui\hud_obj_dispenser.res"
del /Q "%TF2LOC%\tf\resource\ui\hud_obj_sentrygun.res"
del /Q "%TF2LOC%\tf\resource\ui\hud_obj_tele_entrance.res"
del /Q "%TF2LOC%\tf\resource\ui\hud_obj_tele_exit.res"
del /Q "%TF2LOC%\tf\scripts\mod_textures.txt"
del /Q "%TF2LOC%\tf\scripts\mod_textures.patch"
del /Q "%TF2LOC%\tf\addons\Cryan HUD.dll"
del /Q "%TF2LOC%\tf\addons\Cryan HUD.vdf"

echo Done.
pause
