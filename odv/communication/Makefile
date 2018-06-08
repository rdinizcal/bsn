all: send recv main teste
comp: send recv main
send:
	clear
	g++ -std=c++11 -c TCPSendBytes.cpp -o TCPSendBytes.o
recv:	
	g++ -std=c++11 -c TCPReceiveBytes.cpp -o TCPReceiveBytes.o
main: 
	g++ -std=c++11 -c consumer.cpp -o consumer.o -lopendavinci -lpthread
	g++ -std=c++11 -c main.cpp -o main.o
	g++ -o main.out consumer.o TCPSendBytes.o main.o TCPReceiveBytes.o -lopendavinci -lpthread	
teste:
	cxxtestgen --error-printer -o runner.cpp test_comunications.h
	g++ -std=c++11 -c runner.cpp -o runner.o
	g++ -o test.out TCPReceiveBytes.o TCPSendBytes.o consumer.o runner.o -lopendavinci -lpthread
	./test.out
clean:
	rm -rf *.o	
	rm -rf *.out	