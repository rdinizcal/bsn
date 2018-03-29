#!/bin/bash
cd libopenbasn
mkdir build
cd build
cmake ..
make
sudo make install
cd ..
cd ..
cd bodyhub
mkdir build
cd build
mkdir output
cmake ..
make
cd ..
cd ..
cd sensornode
mkdir build
cd build
mkdir output
cmake ..
make
cd ..
cd ..