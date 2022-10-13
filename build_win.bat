git submodule update --init --recursive
cmake -S . -B build -A x64 -D PLATFORM_WINDOWS=ON

pause