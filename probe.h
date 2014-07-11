#ifndef PROBE_H
#define PROBE_H

#include <string>
#include <vector>
#include <iostream>



class Probe
{


	private:
		std::string probe_id_;
		std::string seq_;
		std::string probe_set_id_;
		std::string tc_id_;
		std::string id_;
		int probes_in_probeset_;
		int probes_in_tc_;
	
	
	
	
	public:
		Probe(std::string pid, std::string s, std::string psid);
		std::string getPID() {return probe_id_;};
		void setTCID(std::string tc_id, int count);
		void setProbeCounts(int counts);
		void printProbe();
		void printFASTA();
		void reverse();


};

#endif