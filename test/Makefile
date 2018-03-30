all:	
	cxxtestgen --error-printer -o runner.cpp test_suite.h
	g++ -std=c++11 -c runner.cpp -o build/a.o
	g++ -o main build/SensorData.o build/a.o build/BodyHubModule.o -lopendavinci -lpthread	