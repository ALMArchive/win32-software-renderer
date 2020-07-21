@echo off

pushd ..\build
cl -FC -Zi ..\src\win32_software_renderer.cpp ..\src\bitmap.cpp ..\src\utils.cpp ..\src\stars_3d.cpp ..\src\weird_gradient.cpp user32.lib gdi32.lib
popd
