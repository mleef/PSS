/* 
   Header file for the probe set class. Probe set objects are data representations of Affymetrix probe sets and 
   include information such as various IDs (transcript cluster, probe set, probe) as well as a mapping of all
   probes contained in the set (probe name -> probe object). These objects are mainly used to organize the probes' 
   information before building the unique PSS ID's. 
*/

#ifndef PROBESET_H
#define PROBESET_H

#include <string>
#include <map>
#include <iostream>
#include "probe.h"

typedef std::map<std::string, Probe> ProbeMap;
typedef std::pair<std::string, Probe> ProbePair;

typedef std::map<std::string, Probe>::iterator it_type;

class ProbeSet
{


	private:
		std::string probe_set_id_;
		std::string tc_id_;
		std::string name_;
		int probe_count_;
		ProbeMap probes;
			
	
	
	public:
		ProbeSet(std::string id);
		ProbeSet();
		void setTCID(std::string id);
		void setPSID(std::string id){probe_set_id_ = id;};
		void setName(std::string n){name_ = n;};
		std::string getPSID(){return probe_set_id_;};
		void addProbe(Probe p);
		int getProbeCount(){return probe_count_;};
		void setProbeCounts(int counts);
		void printProbeSet();
		void printFASTA();
};

#endif