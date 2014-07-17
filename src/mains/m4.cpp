/* 
   Execution file for the "gene_html" binary.
*/

#include "../files/fileprocessor.h"


int main (int argc, char *argv[]) {
    FileProcessor fp = FileProcessor();
    ProbeScoreMap probes = fp.processBLASTAlignments(argv[3]);
    fp.processBLASTTabs(argv[1], probes, false, argv[2]);

}


