@echo off
call gettf2loc.cmd

del /Q "%TF2LOC%\tf\scripts\mod_textures.txt"
HLExtract -p "%TF2LOC%\..\..\team fortress 2 content.gcf" -e "root\tf\scripts\mod_textures.txt" -d "%TF2LOC%\tf\scripts"
ren "%TF2LOC%\tf\scripts\mod_textures.txt" mod_textures.pure
allbut 3 < "%TF2LOC%\tf\scripts\mod_textures.pure" > "%TF2LOC%\tf\scripts\mod_textures.txt"
type "%TF2LOC%\tf\scripts\mod_textures.patch" >> "%TF2LOC%\tf\scripts\mod_textures.txt"
del "%TF2LOC%\tf\scripts\mod_textures.pure"
echo mod_textures.txt patched.
