all:	
	cxxtestgen --error-printer -o runner.cpp test_suite.h
	g++ -std=c++11 -c runner.cpp -o aux/a.o
	g++ -o main aux/SensorData.o aux/a.o aux/BodyHubModule.o -lopendavinci -lpthread	