git submodule update --init --recursive
cd vendor

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
