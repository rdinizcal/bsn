all: install compile-odv compile-libbsn compile-main
run_tests: test_comunications test_BodyHubEntity test_vitalstatistics
compile-odv:
	g++ -std=c++11 -I odv/communication/include -c odv/communication/src/TCPReceive.cpp -o odv/communication/build/TCPReceive.o
	g++ -std=c++11 -I odv/sensornode/data/include -c odv/sensornode/data/src/DataCollector.cpp -o odv/sensornode/data/build/DataCollector.o
	g++ -std=c++11 -I odv/communication/include -c odv/communication/src/TCPSend.cpp -o odv/communication/build/TCPSend.o
	g++ -std=c++11 -I odv/sensornode/handler/include -c odv/sensornode/handler/src/DataHandler.cpp -o odv/sensornode/handler/build/DataHandler.o
	g++ -std=c++11 -I odv/tcp_triggers/receiver/include -c odv/tcp_triggers/receiver/src/DataTriggeredReceiver.cpp -o odv/tcp_triggers/receiver/build/DataTriggeredReceiver.o
	g++ -std=c++11 -I odv/tcp_triggers/listener/include -c odv/tcp_triggers/listener/src/tcp_listener.cpp -o odv/tcp_triggers/listener/build/tcp_listener.o
compile-libbsn:
	g++ -std=c++11 -I libbsn/include/entity -c libbsn/src/entity/SensorNodeEntity.cpp -o libbsn/build/SensorNodeEntity.o
	g++ -std=c++11 -I libbsn/include/persistence -c libbsn/src/persistence/PersistenceModule.cpp -o libbsn/build/PersistenceModule.o
	g++ -std=c++11 -I libbsn/include/data -c libbsn/src/data/RawData.cpp -o libbsn/build/RawData.o
	g++ -std=c++11 -I libbsn/include/data -c libbsn/src/data/SensorData.cpp -o libbsn/build/SensorData.o
	g++ -std=c++11 -I libbsn/include/entity -c libbsn/src/entity/BodyHubEntity.cpp -o libbsn/build/BodyHubEntity.o
	g++ -std=c++11 -I libbsn/include/statistics -c libbsn/src/statistics/VitalStatistics.cpp -o libbsn/build/VitalStatistics.o
compile-main:
	g++ -std=c++11 -c odv/tcp_triggers/listener/apps/tcp_listenerApp.cpp -o odv/tcp_triggers/listener/build/tcp_listenerApp.o
	g++ -o odv/tcp_triggers/listener/build/tcp_listenerApp odv/tcp_triggers/listener/build/tcp_listenerApp.o odv/communication/build/TCPReceive.o libbsn/build/SensorData.o odv/tcp_triggers/listener/build/tcp_listener.o -lopendavinci -lpthread
	g++ -std=c++11 -c odv/tcp_triggers/receiver/apps/DataTriggeredApp.cpp -o odv/tcp_triggers/receiver/build/DataTriggeredApp.o
	g++ -o odv/tcp_triggers/receiver/build/DataTriggeredApp odv/tcp_triggers/receiver/build/DataTriggeredApp.o libbsn/build/SensorData.o odv/tcp_triggers/receiver/build/DataTriggeredReceiver.o -lopendavinci -lpthread
	g++ -std=c++11 -c odv/sensornode/data/apps/DataCollectorApp.cpp -o odv/sensornode/data/build/DataCollectorApp.o
	g++ -o odv/sensornode/data/build/DataCollectorApp odv/sensornode/data/build/DataCollectorApp.o libbsn/build/RawData.o odv/sensornode/data/build/DataCollector.o -lopendavinci -lpthread
	g++ -std=c++11 -c odv/sensornode/handler/apps/DataHandlerApp.cpp -o odv/sensornode/handler/build/DataHandlerApp.o
	g++ -o odv/sensornode/handler/build/DataHandlerApp odv/sensornode/handler/build/DataHandlerApp.o libbsn/build/RawData.o odv/sensornode/handler/build/DataHandler.o -lopendavinci -lpthread
test_comunications:
	@cxxtestgen --error-printer -o odv/communication/test/runner.cpp odv/communication/test/test_comunications.h
	@g++ -std=c++11 -c odv/communication/test/runner.cpp -o odv/communication/test/a.o 
	@g++ -o odv/communication/test/main.out odv/communication/test/a.o odv/communication/build/TCPReceive.o odv/communication/build/TCPSend.o -lopendavinci -lpthread
	./odv/communication/test/main.out
test_BodyHubEntity:
	@cxxtestgen --error-printer -o libbsn/test/runner.cpp libbsn/test/test_BodyHubEntity.h
	@g++ -std=c++11 -c libbsn/test/runner.cpp -o libbsn/test/a.o 
	@g++ -o libbsn/test/main.out libbsn/test/a.o libbsn/build/SensorData.o libbsn/build/BodyHubEntity.o libbsn/build/VitalStatistics.o libbsn/build/PersistenceModule.o -lopendavinci -lpthread
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