all: install compile-odv compile-libbsn compile-main
run_tests: test_comunications test_BodyHubEntity test_vitalstatistics
compile-odv:
	g++ -std=c++11 -I odv/sensornode/handler/include -c odv/sensornode/handler/src/DataHandler.cpp -o odv/sensornode/handler/build/DataHandler.o
	g++ -std=c++11 -I odv/sensornode/data/include -c odv/sensornode/data/src/DataCollector.cpp -o odv/sensornode/data/build/DataCollector.o
	g++ -std=c++11 -I odv/communication/include -c odv/communication/src/TCPReceiveBytes.cpp -o odv/communication/build/TCPReceiveBytes.o
	g++ -std=c++11 -I odv/communication/include -c odv/communication/src/TCPSendBytes.cpp -o odv/communication/build/TCPSendBytes.o
	g++ -std=c++11 -I odv/communication/include -c odv/communication/src/consumer.cpp -o odv/communication/build/consumer.o
compile-libbsn:
	g++ -std=c++11 -I libbsn/include/data -c libbsn/src/data/SensorData.cpp -o libbsn/build/SensorData.o
	g++ -std=c++11 -I libbsn/include/entity -c libbsn/src/entity/SensorNodeEntity.cpp -o libbsn/build/SensorNodeEntity.o
	g++ -std=c++11 -I libbsn/include/persistence -c libbsn/src/persistence/PersistenceModule.cpp -o libbsn/build/PersistenceModule.o
	g++ -std=c++11 -I libbsn/include/statistics -c libbsn/src/statistics/VitalStatistics.cpp -o libbsn/build/VitalStatistics.o
	g++ -std=c++11 -I libbsn/include/data -c libbsn/src/data/RawData.cpp -o libbsn/build/RawData.o
	g++ -std=c++11 -I libbsn/include/entity -c libbsn/src/entity/BodyHubEntity.cpp -o libbsn/build/BodyHubEntity.o
compile-main:
	g++ -std=c++11 -c odv/communication/apps/main.cpp -o odv/communication/build/main.o
	g++ -o odv/communication/build/main odv/communication/build/main.o odv/communication/build/TCPReceiveBytes.o odv/communication/build/TCPSendBytes.o -lopendavinci -lpthread
	g++ -std=c++11 -c odv/sensornode/data/apps/DataCollectorApp.cpp -o odv/sensornode/data/build/DataCollectorApp.o
	g++ -o odv/sensornode/data/build/DataCollectorApp odv/sensornode/data/build/DataCollectorApp.o odv/sensornode/data/build/DataCollector.o libbsn/build/RawData.o -lopendavinci -lpthread
	g++ -std=c++11 -c odv/sensornode/handler/apps/DataHandlerApp.cpp -o odv/sensornode/handler/build/DataHandlerApp.o
	g++ -o odv/sensornode/handler/build/DataHandlerApp odv/sensornode/handler/build/DataHandlerApp.o odv/sensornode/handler/build/DataHandler.o libbsn/build/RawData.o -lopendavinci -lpthread
test_comunications:
	@cxxtestgen --error-printer -o odv/communication/test/runner.cpp odv/communication/test/test_comunications.h
	@g++ -std=c++11 -c odv/communication/test/runner.cpp -o odv/communication/test/a.o 
	@g++ -o odv/communication/test/main.out odv/communication/test/a.o odv/communication/build/TCPReceiveBytes.o odv/communication/build/TCPSendBytes.o odv/communication/build/consumer.o -lopendavinci -lpthread
	./odv/communication/test/main.out
test_BodyHubEntity:
	@cxxtestgen --error-printer -o libbsn/test/runner.cpp libbsn/test/test_BodyHubEntity.h
	@g++ -std=c++11 -c libbsn/test/runner.cpp -o libbsn/test/a.o 
	@g++ -o libbsn/test/main.out libbsn/test/a.o libbsn/build/SensorData.o libbsn/build/VitalStatistics.o libbsn/build/PersistenceModule.o libbsn/build/BodyHubEntity.o -lopendavinci -lpthread
	./libbsn/test/main.out
test_vitalstatistics:
	@cxxtestgen --error-printer -o libbsn/test/runner.cpp libbsn/test/test_vitalstatistics.h
	@g++ -std=c++11 -c libbsn/test/runner.cpp -o libbsn/test/a.o 
	@g++ -o libbsn/test/main.out libbsn/test/a.o libbsn/build/VitalStatistics.o -lopendavinci -lpthread
	./libbsn/test/main.out
install:
	sudo mkdir -p /usr/local/include/bsn/
	sudo mkdir -p  /usr/local/include/bsn/data/
	sudo cp libbsn/include/data/SensorData.h /usr/local/include/bsn/data/
	sudo mkdir -p  /usr/local/include/bsn/data/
	sudo cp libbsn/include/data/RawData.h /usr/local/include/bsn/data/
	sudo mkdir -p  /usr/local/include/bsn/entity/
	sudo cp libbsn/include/entity/BodyHubEntity.h /usr/local/include/bsn/entity/
	sudo mkdir -p  /usr/local/include/bsn/entity/
	sudo cp libbsn/include/entity/SensorNodeEntity.h /usr/local/include/bsn/entity/
	sudo mkdir -p  /usr/local/include/bsn/statistics/
	sudo cp libbsn/include/statistics/VitalStatistics.h /usr/local/include/bsn/statistics/
	sudo mkdir -p  /usr/local/include/bsn/persistence/
	sudo cp libbsn/include/persistence/PersistenceModule.h /usr/local/include/bsn/persistence/
uninstall:
	sudo rm -rf  /usr/local/include/bsn/