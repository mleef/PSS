/* 
   Execution file for the "build_fasta" binary
*/

#include "../files/fileprocessor.h"


int main (int argc, char *argv[]) {
   
    FileProcessor fp = FileProcessor();
    ProbeSetMap probesets = fp.processLibraryFiles(argv[1], argv[2]);
    
    for(ps_iter iterator = probesets.begin(); iterator != probesets.end(); iterator++) {
    	iterator->second.printFASTA();
	}	
	

}


