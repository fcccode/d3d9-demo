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
) else if defined VS90COMNTOOLS (
    echo Visual Studio 2008
    call "%VS90COMNTOOLS%vsvars32.bat"
) else if defined VS80COMNTOOLS (
    echo Visual Studio 2005
    call "%VS80COMNTOOLS%vsvars32.bat"
) else (
    echo Visual Studio None
    goto :pause
)

set D3DX9=%~dp0d3dx9\

set OPTS=/Fesnowman /I"%D3DX9%include" /DDIRECTINPUT_VERSION=0x0800 /EHsc /nologo
set LINK=/LIBPATH:"%D3DX9%lib\x86"

set SRCS=framework.cpp application.cpp camera.cpp crate.cpp effect.cpp error.cpp input.cpp light.cpp skybox.cpp snowman.cpp terrain.cpp vertex.cpp
set LIBS=d3dx9.lib d3d9.lib dinput8.lib dxguid.lib kernel32.lib user32.lib gdi32.lib

cl %OPTS% %SRCS% %LIBS% /link %LINK%
del *.obj

:pause
pause
