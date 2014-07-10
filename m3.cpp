#include "fileprocessor.h"


int main (int argc, char *argv[]) {
    FileProcessor fp = FileProcessor();
    fp.processBLAST(argv[1], argv[2]);

}


