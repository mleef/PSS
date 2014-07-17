bin=/Users/marc_leef/Desktop/Work/PSS/bin
src=/Users/marc_leef/Desktop/Work/PSS/src



all: sequence.o probe.o probeset.o fileprocessor.o m1.o m2.o m3.o m4.o process_alignments build_fasta exon_html gene_html
	rm *.o
	
m1.o: $(src)/mains/m1.cpp
	g++ -g -c $(src)/mains/m1.cpp
	
m2.o: $(src)/mains/m2.cpp
	g++ -g -c $(src)/mains/m2.cpp
	
m3.o: $(src)/mains/m3.cpp
	g++ -g -c $(src)/mains/m3.cpp	

m4.o: $(src)/mains/m4.cpp
	g++ -g -c $(src)/mains/m4.cpp

sequence.o: $(src)/probes/sequence.cpp $(src)/probes/sequence.h
	g++ -g -c $(src)/probes/sequence.cpp
	
probe.o: $(src)/probes/probe.cpp $(src)/probes/probe.h
	g++ -g -c $(src)/probes/probe.cpp

probeset.o: $(src)/probes/probeset.cpp $(src)/probes/probeset.h
	g++ -g -c $(src)/probes/probeset.cpp

fileprocessor.o: $(src)/files/fileprocessor.cpp $(src)/files/fileprocessor.h
	g++ -g -c $(src)/files/fileprocessor.cpp

process_alignments: m1.o sequence.o probe.o probeset.o fileprocessor.o
	g++ -g -o $(bin)/process_alignments m1.o sequence.o probe.o probeset.o fileprocessor.o

build_fasta: m2.o sequence.o probe.o probeset.o fileprocessor.o
	g++ -g -o $(bin)/build_fasta m2.o sequence.o probe.o probeset.o fileprocessor.o

exon_html: m3.o sequence.o probe.o probeset.o fileprocessor.o
	g++ -g -o $(bin)/exon_html m3.o sequence.o probe.o probeset.o fileprocessor.o

gene_html: m4.o sequence.o probe.o probeset.o fileprocessor.o
	g++ -g -o $(bin)/gene_html m4.o sequence.o probe.o probeset.o fileprocessor.o
	
clean:
	rm $(bin)/build_pss $(bin)/build_fasta $(bin)/exon_html $(bin)/gene_html