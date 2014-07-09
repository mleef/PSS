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
		int probe_count_;
		ProbeMap probes;
			
	
	
	public:
		ProbeSet(std::string id);
		void setTCID(std::string id);
		void setPSID(std::string id){probe_set_id_ = id;};
		std::string getPSID(){return probe_set_id_;};
		void addProbe(Probe p);
		void printProbeSet();
		void printFASTA();






};

#endif