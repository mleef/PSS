all: main.o sequence.o pss

main.o: main.cpp
	g++ -g -c main.cpp
	
sequence.o: sequence.cpp sequence.h
	g++ -g -c sequence.cpp
	
pss: main.o sequence.o
	g++ -g -o pss main.o sequence.o
	
clean:
	rm *.o pss