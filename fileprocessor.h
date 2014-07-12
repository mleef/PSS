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

struct bLine {
	std::string query_id;
	std::string tc_id;
	std::string probe_set_id;
	std::string probe_id;
	std::string perc_identity;
	std::string length;
	std::string mismatches;
	std::string gaps;
	std::string q_start;
	std::string q_end;
	std::string s_start;
	std::string s_end;
	std::string evalue;
	std::string score;
	int probes_in_probeset;
	int probes_in_tc;
	int probe_hits;
	double percent;
};

typedef std::map<std::string, ProbeSet> ProbeSetMap;
typedef std::pair<std::string, ProbeSet> ProbeSetPair;
typedef std::map<std::string, ProbeSet>::iterator ps_iter;

typedef std::map<std::string, std::vector<bLine> > ProbeSetLine;
typedef std::pair<std::string, std::vector<bLine> > LinePair;
typedef std::map<std::string, std::vector<bLine> >::iterator cp_iter;

typedef std::map<std::string, bool> CheckMap;
typedef std::pair<std::string, bool> CheckPair;
typedef std::map<std::string, bool>::iterator check_iter;

typedef std::map<std::string, std::vector<ProbeSet> > TranscriptClusterMap;
typedef std::pair<std::string, std::vector<ProbeSet> > TranscriptClusterPair;
typedef std::map<std::string, std::vector<ProbeSet> >::iterator tc_iter;


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
		void processBLAST(const char * b, bool exon);
		void outputHTML(std::string query_id, ProbeSetLine map, bool exon);
		void styleHeadings();



};

#endif