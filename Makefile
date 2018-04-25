all:
	@mkdir -p libopenbasn/build
	@mkdir -p libopenbasn/build/output

	$(info Compiling sensordata)
	g++ -std=c++11 -I libopenbasn/include/openbasn/data -c libopenbasn/src/data/SensorData.cpp -o libopenbasn/build/SensorData.o

	@mkdir -p bodyhub/build
	@mkdir -p bodyhub/build/output

	$(info Compiling bodyhub)
	g++ -std=c++11 -I bodyhub/include -c bodyhub/src/BodyHubModule.cpp -o bodyhub/build/BodyHubModule.o
	g++ -std=c++11 -I bodyhub/include -c bodyhub/apps/bodyhub.cpp -o bodyhub/build/BodyHub.o
	g++ -o bodyhub/build/bodyhub VitalStatistics/build/VitalStatistics.o libopenbasn/build/SensorData.o bodyhub/build/BodyHub.o bodyhub/build/BodyHubModule.o -lopendavinci -lpthread

	@mkdir -p sensornode/build
	@mkdir -p sensornode/build/output

	$(info Compiling sensornode)
	g++ -std=c++11 -I sensornode/include -c sensornode/src/SensorNodeModule.cpp -o sensornode/build/SensorNodeModule.o
	g++ -std=c++11 -I sensornode/include -c sensornode/apps/sensornode.cpp -o sensornode/build/SensorNode.o
	g++ -o sensornode/build/sensornode VitalStatistics/build/VitalStatistics.o libopenbasn/build/SensorData.o sensornode/build/SensorNode.o sensornode/build/SensorNodeModule.o -lopendavinci -lpthread

	$(info Compiling vital statistics)
	@mkdir -p VitalStatistics/build
	g++ -std=c++11 -I VitalStatistics/include/ -c VitalStatistics/src/VitalStatistics.cpp -o VitalStatistics/build/VitalStatistics.o	
