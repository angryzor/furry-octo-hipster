@echo off
echo Building textures...
VTFCmd.exe -folder "pngs\*.png" -output "tf\materials\vgui\logos\cryan_custom_hud" -format "BGRA8888"
echo Done.
pause
