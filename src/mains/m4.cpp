/* 
   Execution file for the "gene_html" binary.
*/

#include "../files/fileprocessor.h"


int main (int argc, char *argv[]) {
    FileProcessor fp = FileProcessor();
    fp.processBLAST(argv[1], false);

}


