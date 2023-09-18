git submodule update --init --recursive
cd vendor

cd fmt
mkdir build
cd build
cmake -DFMT_TESTS=OFF ..
make install
cd ../..

cd CppLinuxSerial
mkdir build
cd build
cmake ..
make install
cd ../..

cd ..

mkdir build
cd build
make install
