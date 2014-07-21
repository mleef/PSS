/* 
   Execution file for the "process_alignments" binary
*/

#include "../files/fileprocessor.h"

int main (int argc, char *argv[]) {
   
    FileProcessor fp = FileProcessor();
	ProbeScoreMap probes = fp.processBLASTAlignments(argv[1]);
	for(pscore_iter iterator = probes.begin(); iterator != probes.end(); iterator++) {
    	std::cout << iterator->first << " " << iterator->second.href << " " << iterator->second.hyb_score << std::endl;
	}	

}


