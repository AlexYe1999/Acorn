git submodule update --init --recursive
cmake -S . -B build -A x64 -DPLATFORM_WINDOWS=ON -D PLATFORM_WINDOWS=ON
cmake --build build --config Release
pause