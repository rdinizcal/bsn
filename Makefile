all: install compile-libbsn compile-simulation compile-module compile-main
run_tests: test_BodyHubEntity test_vitalstatistics test_sensorNode test_bodyhub
compile-libbsn:
	g++ -std=c++11 -I libbsn/include/entity -c libbsn/src/entity/SensorNodeEntity.cpp -o libbsn/build/SensorNodeEntity.o
	g++ -std=c++11 -I libbsn/include/entity -c libbsn/src/entity/BodyHubEntity.cpp -o libbsn/build/BodyHubEntity.o
	g++ -std=c++11 -I libbsn/include/persistence -c libbsn/src/persistence/PersistenceModule.cpp -o libbsn/build/PersistenceModule.o
	g++ -std=c++11 -I libbsn/include/data -c libbsn/src/data/SensorData.cpp -o libbsn/build/SensorData.o
	g++ -std=c++11 -I libbsn/include/statistics -c libbsn/src/statistics/VitalStatistics.cpp -o libbsn/build/VitalStatistics.o
compile-simulation:
	g++ -std=c++11 -I simulation/communications/include -c simulation/communications/src/data_sender.cpp -o simulation/communications/build/data_sender.o
	g++ -std=c++11 -I simulation/communications/include -c simulation/communications/src/data_receiver.cpp -o simulation/communications/build/data_receiver.o
	g++ -std=c++11 -I simulation/communications/sockets/include -c simulation/communications/sockets/src/sockets.cpp -o simulation/communications/sockets/build/sockets.o
compile-module:
	g++ -std=c++11 -I module/bodyhub/include -c module/bodyhub/src/BodyHubModule.cpp -o module/bodyhub/build/BodyHubModule.o
	g++ -std=c++11 -I module/sensornode/include -c module/sensornode/src/SensorNodeModule.cpp -o module/sensornode/build/SensorNodeModule.o
compile-main:
	g++ -std=c++11 -c simulation/sensor/apps/sensor.cpp -o simulation/sensor/build/sensor.o
	g++ -o simulation/sensor/build/sensor simulation/sensor/build/sensor.o simulation/communications/build/data_sender.o libbsn/build/VitalStatistics.o simulation/communications/sockets/build/sockets.o -lopendavinci -lpthread
	g++ -std=c++11 -c module/sensornode/apps/sensornode.cpp -o module/sensornode/build/sensornode.o
	g++ -o module/sensornode/build/sensornode module/sensornode/build/sensornode.o libbsn/build/VitalStatistics.o libbsn/build/PersistenceModule.o module/sensornode/build/SensorNodeModule.o libbsn/build/SensorData.o libbsn/build/SensorNodeEntity.o simulation/communications/build/data_receiver.o simulation/communications/sockets/build/sockets.o -lopendavinci -lpthread
	g++ -std=c++11 -c module/bodyhub/apps/bodyhub.cpp -o module/bodyhub/build/bodyhub.o
	g++ -o module/bodyhub/build/bodyhub module/bodyhub/build/bodyhub.o libbsn/build/PersistenceModule.o module/bodyhub/build/BodyHubModule.o libbsn/build/VitalStatistics.o libbsn/build/BodyHubEntity.o libbsn/build/SensorData.o -lopendavinci -lpthread
test_BodyHubEntity:
	@cxxtestgen --error-printer -o libbsn/test/runner.cpp libbsn/test/test_BodyHubEntity.h
	@g++ -std=c++11 -c libbsn/test/runner.cpp -o libbsn/test/a.o 
	@g++ -o libbsn/test/main.out libbsn/test/a.o libbsn/build/PersistenceModule.o libbsn/build/VitalStatistics.o libbsn/build/BodyHubEntity.o libbsn/build/SensorData.o -lopendavinci -lpthread
	./libbsn/test/main.out
test_vitalstatistics:
	@cxxtestgen --error-printer -o libbsn/test/runner.cpp libbsn/test/test_vitalstatistics.h
	@g++ -std=c++11 -c libbsn/test/runner.cpp -o libbsn/test/a.o 
	@g++ -o libbsn/test/main.out libbsn/test/a.o libbsn/build/VitalStatistics.o -lopendavinci -lpthread
	./libbsn/test/main.out
test_sensorNode:
	@cxxtestgen --error-printer -o module/test/runner.cpp module/test/test_sensorNode.h
	@g++ -std=c++11 -c module/test/runner.cpp -o module/test/a.o 
	@g++ -o module/test/main.out module/test/a.o libbsn/build/VitalStatistics.o libbsn/build/PersistenceModule.o module/sensornode/build/SensorNodeModule.o libbsn/build/SensorData.o libbsn/build/SensorNodeEntity.o simulation/communications/build/data_receiver.o simulation/communications/sockets/build/sockets.o -lopendavinci -lpthread
	./module/test/main.out
test_bodyhub:
	@cxxtestgen --error-printer -o module/test/runner.cpp module/test/test_bodyhub.h
	@g++ -std=c++11 -c module/test/runner.cpp -o module/test/a.o 
	@g++ -o module/test/main.out module/test/a.o libbsn/build/PersistenceModule.o module/bodyhub/build/BodyHubModule.o libbsn/build/VitalStatistics.o libbsn/build/BodyHubEntity.o libbsn/build/SensorData.o -lopendavinci -lpthread
	./module/test/main.out
install:
	sudo mkdir -p /usr/local/include/bsn/
	sudo mkdir -p  /usr/local/include/bsn/persistence/
	sudo cp libbsn/include/persistence/PersistenceModule.h /usr/local/include/bsn/persistence/
	sudo mkdir -p  /usr/local/include/bsn/statistics/
	sudo cp libbsn/include/statistics/VitalStatistics.h /usr/local/include/bsn/statistics/
	sudo mkdir -p  /usr/local/include/bsn/data/
	sudo cp libbsn/include/data/SensorData.h /usr/local/include/bsn/data/
	sudo mkdir -p  /usr/local/include/bsn/entity/
	sudo cp libbsn/include/entity/SensorNodeEntity.h /usr/local/include/bsn/entity/
	sudo mkdir -p  /usr/local/include/bsn/entity/
	sudo cp libbsn/include/entity/BodyHubEntity.h /usr/local/include/bsn/entity/
	sudo mkdir -p /usr/local/include/bsn/
	sudo cp module/sensornode/include/SensorNodeModule.h /usr/local/include/bsn/SensorNodeModule.h
	sudo cp module/bodyhub/include/BodyHubModule.h /usr/local/include/bsn/BodyHubModule.h
uninstall:
	sudo rm -rf  /usr/local/include/bsn/