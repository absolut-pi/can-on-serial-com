git submodule update --init --recursive
cd vendor

cd fmt
mkdir build
cd build
cmake -DFMT_TEST=OFF ..
make install
cd ../..

cd CppLinuxSerial
mkdir build
cd build
cmake -DBUILD_TESTS=OFF ..
make install
cd ../..

cd ..

mkdir build
cd build
make install
