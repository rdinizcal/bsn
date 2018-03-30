all:	
	cxxtestgen --error-printer -o runner.cpp test_suite.h
	g++ -std=c++11 -c runner.cpp -o a.o
	g++ -o main SensorData.o a.o BodyHubModule.o -lopendavinci -lpthread	