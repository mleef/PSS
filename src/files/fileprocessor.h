/* 
   Header file for the fileproccessor object. This class serves a number of useful functions. The first
   is basic file IO, as it contains functions for reading and writing FASTA files. Perhaps most importantly,
   it contains the methods that take in design library files (.pgf and .mps specifically) and create a new
   FASTA with easily parsable probe IDs (PSS ID: (TRANSCRIPT CLUSTER ID) - (PROBE SET ID) - (PROBE ID) - 
   (# PROBES IN TRANSCRIPT CLUSTER) - (# PROBES IN PROBE SET)). It also contains functions that parse BLAST
   output for these special IDs, and then sort probe sets/transcript clusters by hit frequency.
*/

#ifndef FILEPROCESSOR_H
#define FILEPROCESSOR_H

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include "../probes/sequence.h"
#include "../probes/probe.h"
#include "../probes/probeset.h"

// Small datatype for storing information from the BLAST output in tab spaced format
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

// Mapping, pair, and iterator from probe set name to probe set object
typedef std::map<std::string, ProbeSet> ProbeSetMap;
typedef std::pair<std::string, ProbeSet> ProbeSetPair;
typedef std::map<std::string, ProbeSet>::iterator ps_iter;

// Mapping, pair, and iterator from probe set name to list of BLAST output lines
typedef std::map<std::string, std::vector<bLine> > ProbeSetLine;
typedef std::pair<std::string, std::vector<bLine> > LinePair;
typedef std::map<std::string, std::vector<bLine> >::iterator cp_iter;

// Mapping, pair, and iterator from probe set name boolean (for use in sorting)
typedef std::map<std::string, bool> CheckMap;
typedef std::pair<std::string, bool> CheckPair;
typedef std::map<std::string, bool>::iterator check_iter;

// Mapping, pair, and iterator from transcript cluster name to list of probe sets
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
		void processBLASTTabs(const char * b, bool exon, std::string id);
		void processBLASTAlignments(const char * b);
		void outputHTML(std::string query_id, ProbeSetLine map, bool exon, std::string id);



};

#endif