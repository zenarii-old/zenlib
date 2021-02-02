@echo off
IF NOT EXIST build mkdir build
pushd build
set error_flags = "/W3 /WX /wd4018 /wd4267"

cl -Zi %error_flags% -DUSE_OPENGL ../zenlib/zencore/zencore_win32.c /link user32.lib gdi32.lib opengl32.lib winmm.lib 
popd