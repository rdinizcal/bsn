all:	
	@cxxtestgen --error-printer -o runner.cpp test_suite.h
	@g++ -std=c++11 -c runner.cpp -o a.o
	@g++ -o main ../libopenbasn/build/SensorData.o a.o ../bodyhub/build/BodyHubModule.o ../sensornode/build/SensorNodeModule.o -lopendavinci -lpthread	
	./main