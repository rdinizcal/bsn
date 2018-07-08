all: install compile-odv compile-libbsn compile-main
run_tests: test_BodyHubEntity test_comunications test_vitalstatistics
compile-odv:
	g++ -std=c++11 -I odv/tcp_triggers/receiver/include -c odv/tcp_triggers/receiver/src/DataTriggeredReceiver.cpp -o odv/tcp_triggers/receiver/build/DataTriggeredReceiver.o
	g++ -std=c++11 -I odv/sensornode/handler/include -c odv/sensornode/handler/src/DataHandler.cpp -o odv/sensornode/handler/build/DataHandler.o
	g++ -std=c++11 -I odv/tcp_triggers/sender/include -c odv/tcp_triggers/sender/src/DataSender.cpp -o odv/tcp_triggers/sender/build/DataSender.o
	g++ -std=c++11 -I odv/sensornode/data/include -c odv/sensornode/data/src/DataCollector.cpp -o odv/sensornode/data/build/DataCollector.o
	g++ -std=c++11 -I odv/tcp_triggers/listener/include -c odv/tcp_triggers/listener/src/tcp_listener.cpp -o odv/tcp_triggers/listener/build/tcp_listener.o
compile-libbsn:
	g++ -std=c++11 -I libbsn/include/entity -c libbsn/src/entity/SensorNodeEntity.cpp -o libbsn/build/SensorNodeEntity.o
	g++ -std=c++11 -I libbsn/include/communication -c libbsn/src/communication/TCPReceive.cpp -o libbsn/build/TCPReceive.o
	g++ -std=c++11 -I libbsn/include/data -c libbsn/src/data/SensorData.cpp -o libbsn/build/SensorData.o
	g++ -std=c++11 -I libbsn/include/entity -c libbsn/src/entity/BodyHubEntity.cpp -o libbsn/build/BodyHubEntity.o
	g++ -std=c++11 -I libbsn/include/statistics -c libbsn/src/statistics/VitalStatistics.cpp -o libbsn/build/VitalStatistics.o
	g++ -std=c++11 -I libbsn/include/communication -c libbsn/src/communication/TCPSend.cpp -o libbsn/build/TCPSend.o
	g++ -std=c++11 -I libbsn/include/data -c libbsn/src/data/RawData.cpp -o libbsn/build/RawData.o
	g++ -std=c++11 -I libbsn/include/persistence -c libbsn/src/persistence/PersistenceModule.cpp -o libbsn/build/PersistenceModule.o
compile-main:
	g++ -std=c++11 -c odv/tcp_triggers/sender/apps/SenderApp.cpp -o odv/tcp_triggers/sender/build/SenderApp.o
	g++ -o odv/tcp_triggers/sender/build/SenderApp odv/tcp_triggers/sender/build/SenderApp.o odv/tcp_triggers/sender/build/DataSender.o libbsn/build/TCPSend.o libbsn/build/RawData.o -lopendavinci -lpthread
	g++ -std=c++11 -c odv/tcp_triggers/listener/apps/tcp_listenerApp.cpp -o odv/tcp_triggers/listener/build/tcp_listenerApp.o
	g++ -o odv/tcp_triggers/listener/build/tcp_listenerApp odv/tcp_triggers/listener/build/tcp_listenerApp.o libbsn/build/TCPReceive.o odv/tcp_triggers/listener/build/tcp_listener.o libbsn/build/SensorData.o -lopendavinci -lpthread
	g++ -std=c++11 -c odv/tcp_triggers/receiver/apps/DataTriggeredApp.cpp -o odv/tcp_triggers/receiver/build/DataTriggeredApp.o
	g++ -o odv/tcp_triggers/receiver/build/DataTriggeredApp odv/tcp_triggers/receiver/build/DataTriggeredApp.o odv/tcp_triggers/receiver/build/DataTriggeredReceiver.o libbsn/build/SensorData.o -lopendavinci -lpthread
	g++ -std=c++11 -c odv/sensornode/data/apps/DataCollectorApp.cpp -o odv/sensornode/data/build/DataCollectorApp.o
	g++ -o odv/sensornode/data/build/DataCollectorApp odv/sensornode/data/build/DataCollectorApp.o odv/sensornode/data/build/DataCollector.o libbsn/build/RawData.o -lopendavinci -lpthread
	g++ -std=c++11 -c odv/sensornode/handler/apps/DataHandlerApp.cpp -o odv/sensornode/handler/build/DataHandlerApp.o
	g++ -o odv/sensornode/handler/build/DataHandlerApp odv/sensornode/handler/build/DataHandlerApp.o odv/sensornode/handler/build/DataHandler.o libbsn/build/RawData.o -lopendavinci -lpthread
test_BodyHubEntity:
	@cxxtestgen --error-printer -o libbsn/test/runner.cpp libbsn/test/test_BodyHubEntity.h
	@g++ -std=c++11 -c libbsn/test/runner.cpp -o libbsn/test/a.o 
	@g++ -o libbsn/test/main.out libbsn/test/a.o libbsn/build/VitalStatistics.o libbsn/build/SensorData.o libbsn/build/BodyHubEntity.o libbsn/build/PersistenceModule.o -lopendavinci -lpthread
	./libbsn/test/main.out
test_comunications:
	@cxxtestgen --error-printer -o libbsn/test/runner.cpp libbsn/test/test_comunications.h
	@g++ -std=c++11 -c libbsn/test/runner.cpp -o libbsn/test/a.o 
	@g++ -o libbsn/test/main.out libbsn/test/a.o libbsn/build/TCPReceive.o libbsn/build/TCPSend.o -lopendavinci -lpthread
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
	sudo mkdir -p  /usr/local/include/bsn/communication/
	sudo cp libbsn/include/communication/TCPReceive.hpp /usr/local/include/bsn/communication/
	sudo mkdir -p  /usr/local/include/bsn/communication/
	sudo cp libbsn/include/communication/TCPSend.hpp /usr/local/include/bsn/communication/
uninstall:
	sudo rm -rf  /usr/local/include/bsn/