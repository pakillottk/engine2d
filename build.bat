@echo off 

::compile libEngine2D
call build_libEngine2d.bat
::compile dummyApp first
call build_app.bat

set preprocessor_macros="-DHOT_RELOAD=1"
set include_directories=-I"..\vendor\SDL2\include" -I"..\vendor\SDL2\include\SDL2"
set libs=-lmingw32 -L"..\vendor\SDL2\x86\lib" -lSDL2main -lSDL2
pushd ".\build"
    ::compilation
    g++ -ggdb -mwindows %preprocessor_macros% %include_directories% "..\engine2d\src\win32_engine2d.cpp" %libs%  -o "engine2d"
    ::copy the required dlls
    copy "..\vendor\SDL2\x86\bin\SDL2.dll" ".\"
popd