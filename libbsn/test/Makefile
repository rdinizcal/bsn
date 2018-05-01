BUILD = ../build/

all: bodyhub vitalstatistics

bodyhub:
	$(info Testing bodyhub...)
	cxxtestgen --error-printer -o $(BUILD)runner.cpp test_bodyhub.h
	g++ -std=c++11 -c $(BUILD)runner.cpp -o $(BUILD)a.o
	g++ -o main.out $(BUILD)a.o $(BUILD)libbsn.so
	./main.out

vitalstatistics:
	$(info Testing vitalstatistics...)
	cxxtestgen --error-printer -o $(BUILD)runner.cpp test_vitalstatistics.h 
	g++ -std=c++11 -c $(BUILD)runner.cpp -o $(BUILD)a.o
	g++ -o main.out $(BUILD)a.o $(BUILD)libbsn.so	
	./main.out	

clean:
	$(info Clean all)
	rm -rf *~ *.out *.o runner.cpp