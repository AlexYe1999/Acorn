git submodule update --init --recursive

rmdir /S /Q build
mkdir build
cd build
cmake -A x64 -DPLATFORM_WINDOWS=ON ..
AcornEngine.sln
cd ..
pause