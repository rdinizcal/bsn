all:
	$(info Compiling persistence module)
	@mkdir -p persistence/build
	@mkdir -p persistence/output
	g++ -std=c++11 -I persistence/include/ -c persistence/src/persistenceModule.cpp -o persistence/build/persistence.o

	$(info Compiling vital statistics)
	@mkdir -p VitalStatistics/build
	g++ -std=c++11 -I VitalStatistics/include/ -c VitalStatistics/src/VitalStatistics.cpp -o VitalStatistics/build/VitalStatistics.o	
	
	@mkdir -p libopenbasn/build

	$(info Compiling sensordata)
	g++ -std=c++11 -I libopenbasn/include/openbasn/data -c libopenbasn/src/data/SensorData.cpp -o libopenbasn/build/SensorData.o

	@mkdir -p bodyhub/build

	$(info Compiling bodyhub)
	g++ -std=c++11 -I bodyhub/include -c bodyhub/src/BodyHubModule.cpp -o bodyhub/build/BodyHubModule.o
	g++ -std=c++11 -I bodyhub/include -c bodyhub/apps/bodyhub.cpp -o bodyhub/build/BodyHub.o
	g++ -o bodyhub/build/bodyhub persistence/build/persistence.o VitalStatistics/build/VitalStatistics.o libopenbasn/build/SensorData.o bodyhub/build/BodyHub.o bodyhub/build/BodyHubModule.o -lopendavinci -lpthread

	@mkdir -p sensornode/build

	$(info Compiling sensornode)
	g++ -std=c++11 -I sensornode/include -c sensornode/src/SensorNodeModule.cpp -o sensornode/build/SensorNodeModule.o
	g++ -std=c++11 -I sensornode/include -c sensornode/apps/sensornode.cpp -o sensornode/build/SensorNode.o
	g++ -o sensornode/build/sensornode persistence/build/persistence.o VitalStatistics/build/VitalStatistics.o libopenbasn/build/SensorData.o sensornode/build/SensorNode.o sensornode/build/SensorNodeModule.o -lopendavinci -lpthread
	
com:
	mkdir -p sockets/build 
	mkdir -p communications/build 
	mkdir -p sensor/build 
	g++ -c sockets/src/sockets.cpp -o sockets/build/sockets.o -I sockets/include -std=c++11 -g3 -ggdb3
	g++  -I communications/include -c communications/src/data_sender.cpp -o communications/build/data_sender.o -std=c++11 -g3 -ggdb3
	g++  -I communications/include -c communications/src/data_receiver.cpp -o communications/build/data_receiver.o -std=c++11 -g3 -ggdb3	

sensor:
	g++ -c sensor/sensor.cpp -o sensor/build/sensor.o -std=c++11 -g3 -ggdb3
	g++ -o sensor/build/sensor sockets/build/sockets.o communications/build/data_sender.o sensor/build/sensor.o