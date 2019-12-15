@echo off 

set preprocessor_macros=""
set include_directories=-I"..\engine2d\include" -I"..\libEngine2d\include"
set libs=-lmingw32 -L"..\libEngine2d\lib" -lEngine2d
pushd ".\build"
    ::compilation
    g++ -Wall -ggdb -shared "..\dummyApp\src\dummyApp.cpp" %include_directories% %libs% -o "App.dll" 
popd