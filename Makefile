all: compile-all test
compile:
	g++ -std=c++11 -I libbsn/include/entity -c libbsn/src/entity/BodyHubEntity.cpp -o libbsn/build/BodyHubEntity.o
	g++ -std=c++11 -I module/bodyhub/include -c module/bodyhub/src/BodyHubModule.cpp -o module/bodyhub/build/BodyHubModule.o
	g++ -std=c++11 -I libbsn/include/data -c libbsn/src/data/SensorData.cpp -o libbsn/build/SensorData.o
	g++ -std=c++11 -I libbsn/include/statistics -c libbsn/src/statistics/VitalStatistics.cpp -o libbsn/build/VitalStatistics.o
	g++ -std=c++11 -I simulation/communications/sockets/include -c simulation/communications/sockets/src/sockets.cpp -o simulation/communications/sockets/build/sockets.o
	g++ -std=c++11 -I simulation/communications/include -c simulation/communications/src/data_receiver.cpp -o simulation/communications/build/data_receiver.o
	g++ -std=c++11 -I simulation/communications/include -c simulation/communications/src/data_sender.cpp -o simulation/communications/build/data_sender.o
	g++ -std=c++11 -I libbsn/include/persistence -c libbsn/src/persistence/PersistenceModule.cpp -o libbsn/build/PersistenceModule.o
	g++ -std=c++11 -I module/sensornode/include -c module/sensornode/src/SensorNodeModule.cpp -o module/sensornode/build/SensorNodeModule.o
	g++ -std=c++11 -I libbsn/include/entity -c libbsn/src/entity/SensorNodeEntity.cpp -o libbsn/build/SensorNodeEntity.o

	g++ -std=c++11 -c module/sensornode/apps/sensornode.cpp -o module/sensornode/build/sensornode.o
	g++ -o module/sensornode/build/sensornode module/sensornode/build/sensornode.o simulation/communications/sockets/build/sockets.o libbsn/build/SensorData.o simulation/communications/build/data_receiver.o libbsn/build/PersistenceModule.o libbsn/build/VitalStatistics.o module/sensornode/build/SensorNodeModule.o libbsn/build/SensorNodeEntity.o -lopendavinci -lpthread
	g++ -std=c++11 -c module/bodyhub/apps/bodyhub.cpp -o module/bodyhub/build/bodyhub.o
	g++ -o module/bodyhub/build/bodyhub module/bodyhub/build/bodyhub.o libbsn/build/BodyHubEntity.o module/bodyhub/build/BodyHubModule.o libbsn/build/SensorData.o libbsn/build/VitalStatistics.o libbsn/build/PersistenceModule.o -lopendavinci -lpthread
	g++ -std=c++11 -c simulation/sensor/apps/sensor.cpp -o simulation/sensor/build/sensor.o
	g++ -o simulation/sensor/build/sensor simulation/sensor/build/sensor.o simulation/communications/build/data_sender.o libbsn/build/VitalStatistics.o simulation/communications/sockets/build/sockets.o -lopendavinci -lpthread

compile-com:
	$(info Compiling communications)
	mkdir -p simulation/communications/sockets/build 
	mkdir -p simulation/communications/build 
	mkdir -p sensor/build 
	g++  -c simulation/communications/sockets/src/sockets.cpp -o simulation/communications/sockets/build/sockets.o -I simulation/communications/sockets/include -std=c++11 -g3 -ggdb3
	g++  -I simulation/communications/include -c simulation/communications/src/data_sender.cpp -o simulation/communications/build/data_sender.o -std=c++11 -g3 -ggdb3
	g++  -I simulation/communications/include -c simulation/communications/src/data_receiver.cpp -o simulation/communications/build/data_receiver.o -std=c++11 -g3 -ggdb3	

compile-bh:
	mkdir -p module/bodyhub/build

	$(info Compiling bodyhub)
	g++ -std=c++11 -I module/bodyhub/include -c module/bodyhub/apps/bodyhub.cpp -o module/bodyhub/build/BodyHub.o
	g++ -std=c++11 -I module/bodyhub/include -c module/bodyhub/src/BodyHubModule.cpp -o module/bodyhub/build/BodyHubModule.o
	
	g++ -o module/bodyhub/build/bodyhub libbsn/build/VitalStatistics.o libbsn/build/BodyHubEntity.o libbsn/build/PersistenceModule.o libbsn/build/SensorData.o module/bodyhub/build/BodyHub.o module/bodyhub/build/BodyHubModule.o -lopendavinci -lpthread
compile-all:
	g++ -I libbsn/include/bsn/data/ -std=c++11 -c libbsn/src/data/SensorData.cpp -o libbsn/build/SensorData.o

	g++ -I libbsn/include/bsn/persistence/ -std=c++11 -c libbsn/src/persistence/PersistenceModule.cpp -o libbsn/build/PersistenceModule.o

	compile-com
	
	mkdir -p module/sensornode/build

	$(info Compiling sensornode)
	g++ -std=c++11 -g3 -ggdb3 -I module/sensornode/include -c module/sensornode/src/SensorNodeModule.cpp -o module/sensornode/build/SensorNodeModule.o
	g++ -std=c++11 -I module/sensornode/include -c module/sensornode/apps/sensornode.cpp -o module/sensornode/build/SensorNode.o
	#g++ -o module/sensornode/build/sensornode simulation/communications/sockets/build/sockets.o libbsn/build/libbsn.so simulation/communications/build/data_receiver.o module/sensornode/build/SensorNode.o module/sensornode/build/SensorNodeModule.o -lopendavinci -lpthread	
	
	$(info Compiling sensor)
	g++ -c simulation/sensor/apps/sensor.cpp -o simulation/sensor/build/sensor.o -std=c++11 -g3 -ggdb3
	g++ -o simulation/sensor/build/sensor simulation/communications/sockets/build/sockets.o libbsn/build/VitalStatistics.o simulation/communications/build/data_sender.o simulation/sensor/build/sensor.o

test:
	cxxtestgen --error-printer -o module/test/runner.cpp module/test/test_bodyhub.h module/test/test_sensorNode.h
	g++ -std=c++11 -c module/test/runner.cpp -o module/test/a.o
	g++ -o module/test/main.out module/test/a.o  /usr/local/lib/libbsn.so ../module/bodyhub/build/BodyHubModule.o ../module/sensornode/build/SensorNodeModule.o -lopendavinci -lpthread	
	./module/test/main.out

test-bodyhub:
	cxxtestgen --error-printer -o module/test/runner.cpp module/test/test_bodyhub.h
	g++ -std=c++11 -c module/test/runner.cpp -o module/test/a.o
	g++ -o module/test/main.out module/test/a.o libbsn/build/VitalStatistics.o libbsn/build/SensorData.o libbsn/build/PersistenceModule.o libbsn/build/BodyHubEntity.o module/bodyhub/build/BodyHubModule.o -lopendavinci -lpthread
	./module/test/main.out

test-sensornode:
	cxxtestgen --error-printer -o module/test/runner.cpp module/test/test_sensorNode.h
	g++ -std=c++11 -c module/test/runner.cpp -o module/test/a.o
	g++ -o module/test/main.out module/test/a.o module/sensornode/build/SensorNodeModule.o /usr/local/lib/libbsn.so  -lopendavinci -lpthread	
	./module/test/main.out

clean:
	$(info Clean all)
	rm -f *~ *.out *.o module/test/runner.cpp