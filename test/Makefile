all: 
	@cxxtestgen --error-printer -o runner.cpp test_bodyhub.h test_sensorNode.h
	@g++ -std=c++11 -c runner.cpp -o a.o
	@g++ -o main ../libopenbasn/build/SensorData.o a.o ../bodyhub/build/BodyHubModule.o ../sensornode/build/SensorNodeModule.o -lopendavinci -lpthread	
	./main

bodyhub:	
	@cxxtestgen --error-printer -o runner.cpp test_bodyhub.h
	@g++ -std=c++11 -c runner.cpp -o a.o
	@g++ -o main ../libopenbasn/build/SensorData.o a.o ../bodyhub/build/BodyHubModule.o  -lopendavinci -lpthread	
	./main

sensornode:	
	@cxxtestgen --error-printer -o runner.cpp test_sensorNode.h
	@g++ -std=c++11 -c runner.cpp -o a.o
	@g++ -o main ../libopenbasn/build/SensorData.o a.o  ../sensornode/build/SensorNodeModule.o -lopendavinci -lpthread	
	./main