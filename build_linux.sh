git submodule update --init --recursive

mkdir build
cd build 
cmake .. -DPLATFORM_LINUX=ON
make
cd ..