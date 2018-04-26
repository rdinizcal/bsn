all: 
	@cxxtestgen --error-printer -o runner.cpp test_bodyhub.h test_sensorNode.h test_vital_statistics.h
	@g++ -std=c++11 -c runner.cpp -o a.o
	@g++ -o main.out ../persistence/build/persistence.o ../VitalStatistics/build/VitalStatistics.o ../libopenbasn/build/SensorData.o a.o ../bodyhub/build/BodyHubModule.o ../sensornode/build/SensorNodeModule.o -lopendavinci -lpthread	
	./main.out

bodyhub:	
	@cxxtestgen --error-printer -o runner.cpp test_bodyhub.h
	@g++ -std=c++11 -I VitalStatistics/include -c runner.cpp -o a.o
	@g++ -o main.out ../persistence/build/persistence.o ../VitalStatistics/build/VitalStatistics.o ../libopenbasn/build/SensorData.o a.o ../bodyhub/build/BodyHubModule.o  -lopendavinci -lpthread	
	./main.out

sensornode:	
	@cxxtestgen --error-printer -o runner.cpp test_sensorNode.h
	@g++ -std=c++11 -c runner.cpp -o a.o
	@g++ -o main.out ../persistence/build/persistence.o ../libopenbasn/build/SensorData.o a.o  ../sensornode/build/SensorNodeModule.o -lopendavinci -lpthread	
	./main.out

vitalstatistics:
	@cxxtestgen --error-printer -o runner.cpp test_vital_statistics.h 
	@g++ -std=c++11 -c runner.cpp -o a.o
	@g++ -o main.out ../VitalStatistics/build/VitalStatistics.o a.o -lopendavinci -lpthread	
	./main.out	