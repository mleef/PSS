/* 
   Execution file for the "exon_html" binary.
*/

#include "../files/fileprocessor.h"


int main (int argc, char *argv[]) {
    FileProcessor fp = FileProcessor();
    fp.processBLASTTabs(argv[1], true, argv[2]);

}


