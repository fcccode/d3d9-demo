@echo off

if defined VS140COMNTOOLS (
    echo Visual Studio 2015
    call "%VS140COMNTOOLS%vsvars32.bat"
) else if defined VS120COMNTOOLS (
    echo Visual Studio 2013
    call "%VS120COMNTOOLS%vsvars32.bat"
) else if defined VS110COMNTOOLS (
    echo Visual Studio 2012
    call "%VS110COMNTOOLS%vsvars32.bat"
) else if defined VS100COMNTOOLS (
    echo Visual Studio 2010
    call "%VS100COMNTOOLS%vsvars32.bat"
) else (
    echo Visual Studio None
    goto :pause
)

set D3DX9_INC=..\d3dx9\include
set D3DX9_LIB=..\d3dx9\lib\x86

set OPTS=/I"%D3DX9_INC%" /Fe..\Snowman /nologo /EHsc /W4 /D_CRT_SECURE_NO_WARNINGS /DDIRECTINPUT_VERSION=0x0800
set LINK=/LIBPATH:"%D3DX9_LIB%"

set SRCS=application.cpp framework.cpp camera.cpp crate.cpp effect.cpp input.cpp light.cpp skybox.cpp snowman.cpp terrain.cpp vertex.cpp
set LIBS=user32.lib gdi32.lib d3d9.lib d3dx9.lib dinput8.lib dxguid.lib

cd sources
cl %OPTS% %SRCS% %LIBS% /link %LINK%
del *.obj
cd ..

:pause
pause
