#!/bin/bash
cd libbsn
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
g++ -std=c++11 -I ../include -c ../src/BodyHubModule.cpp -o BodyHubModule.o
cd ..
cd ..
cd sensornode
mkdir build
cd build
mkdir output
cmake ..
make
g++ -std=c++11 -I ../include -c ../src/SensorNodeModule.cpp -o SensorNodeModule.o
cd ..
cd ..