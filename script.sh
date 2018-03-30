#!/bin/bash
cd libopenbasn
mkdir build
cd build
cmake ..
make
sudo make install
cd ..
g++ -std=c++11 -c src/data/SensorData.cpp -o build/SensorData.o
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