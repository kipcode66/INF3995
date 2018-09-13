# Simple Web server

## Updating submodules (Pistache)
git submodule update --init --recursive

## Compiling Pistache
go to `lib/pistache`
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j8 # start make with 8 threads
