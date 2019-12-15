@echo off 

set preprocessor_macros="-DHOT_RELOAD=1"
set include_directories=-I"..\vendor\SDL2\include" -I"..\vendor\SDL2\include\SDL2" -I"..\vendor\SDL2_TTF\include"
set libs=-lmingw32 -L"..\vendor\SDL2\x86\lib" -lSDL2main -lSDL2 -L"..\vendor\SDL2_TTF\x86\lib" -lSDL2_ttf
::compile dummyApp first
call build_app.bat
pushd ".\build"
    ::compilation
    g++ -ggdb -mwindows %preprocessor_macros% %include_directories% "..\engine2d\src\mingw_engine2d.cpp" %libs%  -o "engine2d"
    ::copy the required dlls
    copy "..\vendor\SDL2\x86\bin\SDL2.dll" ".\"
    copy "..\vendor\SDL2_TTF\x86\bin\*.dll" ".\"
popd