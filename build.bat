@echo off

if defined VS150COMNTOOLS (
    echo Visual Studio 2017
    call "%VS150COMNTOOLS%vsvars32.bat"
) else if defined VS140COMNTOOLS (
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

set OPTS=/I"%D3DX9%include" /DDIRECTINPUT_VERSION=0x0800 /nologo /EHsc
set LINK=/LIBPATH:"%D3DX9%lib\x86"

set SRCS=application.cpp framework.cpp input.cpp camera.cpp terrain.cpp error.cpp vertex.cpp skybox.cpp crate.cpp
set LIBS=d3d9.lib d3dx9d.lib dinput8.lib dxguid.lib kernel32.lib user32.lib gdi32.lib

cl %OPTS% %SRCS% %LIBS% /link %LINK%

:pause
pause
