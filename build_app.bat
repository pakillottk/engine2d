@echo off 
pushd ".\build"
    ::compilation
    g++ -Wall -ggdb -shared "..\dummyApp\src\dummyApp.cpp" -I"..\engine2d\include" -o "App.dll" 
popd