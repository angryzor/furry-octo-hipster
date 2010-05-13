@echo off
echo Building textures...
VTFCmd.exe -folder "pngs\*.png" -output "tf\materials\vgui\logos\visual_improve_hud" -format "BGRA8888" -nomipmaps
echo Done.
pause
