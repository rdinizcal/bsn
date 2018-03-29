all:	
	cxxtestgen --error-printer -o runner.cpp test_suite.h 
	g++ -std=c++11 -I /usr/include -c runner.cpp -o MyService.o
	g++ -o service MyService.o -lopendavinci -lpthread
	