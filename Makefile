all: sequence.o probe.o probeset.o fileprocessor.o m1.o m2.o build_pss build_fasta

m1.o: m1.cpp
	g++ -g -c m1.cpp
	
m2.o: m2.cpp
	g++ -g -c m2.cpp

sequence.o: sequence.cpp sequence.h
	g++ -g -c sequence.cpp
	
probe.o: probe.cpp probe.h
	g++ -g -c probe.cpp

probeset.o: probeset.cpp probeset.h
	g++ -g -c probeset.cpp

fileprocessor.o: fileprocessor.cpp fileprocessor.h
	g++ -g -c fileprocessor.cpp

build_pss: m1.o sequence.o probe.o probeset.o fileprocessor.o
	g++ -g -o build_pss m1.o sequence.o probe.o probeset.o fileprocessor.o

build_fasta: m2.o sequence.o probe.o probeset.o fileprocessor.o
	g++ -g -o build_fasta m2.o sequence.o probe.o probeset.o fileprocessor.o
	
clean:
	rm *.o build_pss build_fasta