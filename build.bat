::@echo off 

set preprocessor_macros="-DHOT_RELOAD=1"

::compile dummyApp first
call build_app.bat
pushd ".\build"
    ::compilation
    g++ -ggdb %preprocessor_macros%  "..\engine2d\src\mingw_engine2d.cpp" -I"..\vendor\SDL2\include" -lmingw32 -L"..\vendor\SDL2\x86\lib" -lSDL2main -lSDL2  -o "engine2d"
    ::copy the required dlls
    copy "..\vendor\SDL2\x86\bin\SDL2.dll" ".\"
popd