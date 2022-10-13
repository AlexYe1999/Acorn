git submodule update --init --recursive

mkdir build
cd build 
cmake .. -D PLATFORM_LINUX=ON
make
cd ..