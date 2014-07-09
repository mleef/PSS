#ifndef FILEPROCESSOR_H
#define FILEPROCESSOR_H

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include "sequence.h"
#include "probe.h"
#include "probeset.h"

typedef std::map<std::string, ProbeSet> ProbeSetMap;
typedef std::pair<std::string, ProbeSet> ProbeSetPair;
typedef std::map<std::string, ProbeSet>::iterator ps_iter;




class FileProcessor
{


	private:
	
	
	
	
	public:
		FileProcessor();
		std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
		std::vector<std::string> split(const std::string &s, char delim);
		std::vector<Sequence> readFASTA(const char * f);
		void writeFASTA(std::vector<Sequence> sequences);
		ProbeSetMap processLibraryFiles(const char * pgf, const char * mps);



};

#endif