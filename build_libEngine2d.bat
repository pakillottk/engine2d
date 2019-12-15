@echo off 
set include_directories=-I"..\..\vendor\SDL2\include" -I"..\..\vendor\SDL2_Image\include" -I"..\..\engine2d\include"
pushd ".\libEngine2d\obj"
    ::compilation to .o
    g++ -ggdb %include_directories% -c "..\src\win32_libEngine2d.cpp"
popd

pushd ".\libEngine2d\lib"
    ::make the lib
    ar rvs libEngine2d.a "..\obj\win32_libEngine2d.o"
popd