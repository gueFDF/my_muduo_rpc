#! /bin/zsh
   cd build
   make clean
   cmake ..
   make -j16
   cd ..
   cd bin