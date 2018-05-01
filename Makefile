all: compile test

compile:
	$(info Compiling communications)
	mkdir -p simulation/communications/sockets/build 
	mkdir -p simulation/communications/build 
	mkdir -p sensor/build 
	g++ -c simulation/communications/sockets/src/sockets.cpp -o simulation/communications/sockets/build/sockets.o -I simulation/communications/sockets/include -std=c++11 -g3 -ggdb3
	g++  -I simulation/communications/include -c simulation/communications/src/data_sender.cpp -o simulation/communications/build/data_sender.o -std=c++11 -g3 -ggdb3
	g++  -I simulation/communications/include -c simulation/communications/src/data_receiver.cpp -o simulation/communications/build/data_receiver.o -std=c++11 -g3 -ggdb3	
	
	mkdir -p module/bodyhub/build

	$(info Compiling bodyhub)
	g++ -std=c++11 -I module/bodyhub/include -c module/bodyhub/src/BodyHubModule.cpp -o module/bodyhub/build/BodyHubModule.o
	g++ -std=c++11 -I module/bodyhub/include -c module/bodyhub/apps/bodyhub.cpp -o module/bodyhub/build/BodyHub.o
	#g++ -o module/bodyhub/build/bodyhub /usr/local/lib/libbsn.so module/bodyhub/build/BodyHub.o module/bodyhub/build/BodyHubModule.o -lopendavinci -lpthread

	mkdir -p module/sensornode/build

	$(info Compiling sensornode)
	g++ -std=c++11 -g3 -ggdb3 -I module/sensornode/include -c module/sensornode/src/SensorNodeModule.cpp -o module/sensornode/build/SensorNodeModule.o
	g++ -std=c++11 -I module/sensornode/include -c module/sensornode/apps/sensornode.cpp -o module/sensornode/build/SensorNode.o
	#g++ -o module/sensornode/build/sensornode simulation/communications/sockets/build/sockets.o libbsn/build/libbsn.so simulation/communications/build/data_receiver.o module/sensornode/build/SensorNode.o module/sensornode/build/SensorNodeModule.o -lopendavinci -lpthread	
	
	$(info Compiling sensor)
	g++ -c sensor/sensor.cpp -o sensor/build/sensor.o -std=c++11 -g3 -ggdb3
	g++ -o sensor/build/sensor simulation/communications/sockets/build/sockets.o VitalStatistics/build/VitalStatistics.o simulation/communications/build/data_sender.o sensor/build/sensor.o

test:
	cxxtestgen --error-printer -o module/test/runner.cpp module/test/test_bodyhub.h module/test/test_sensorNode.h
	g++ -std=c++11 -c module/test/runner.cpp -o module/test/a.o
	g++ -o module/test/main.out module/test/a.o  /usr/local/lib/libbsn.so ../module/bodyhub/build/BodyHubModule.o ../module/sensornode/build/SensorNodeModule.o -lopendavinci -lpthread	
	./module/test/main.out

test-bodyhub:
	cxxtestgen --error-printer -o module/test/runner.cpp module/test/test_bodyhub.h
	g++ -std=c++11 -c module/test/runner.cpp -o module/test/a.o
	g++ -o module/test/main.out module/test/a.o module/bodyhub/build/BodyHubModule.o  /usr/local/lib/libbsn.so -lopendavinci -lpthread
	./module/test/main.out

test-sensornode:
	cxxtestgen --error-printer -o module/test/runner.cpp module/test/test_sensorNode.h
	g++ -std=c++11 -c module/test/runner.cpp -o module/test/a.o
	g++ -o module/test/main.out module/test/a.o module/sensornode/build/SensorNodeModule.o /usr/local/lib/libbsn.so  -lopendavinci -lpthread	
	./module/test/main.out

clean:
	$(info Clean all)
	rm -f *~ *.out *.o module/test/runner.cpp