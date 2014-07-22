/* 
   Implementation file for the probe class. Mostly consists of getters and setters for probe attributes
*/

#include "probe.h"

// Constructor for the probe object. Takes in a probe ID, probe set ID, and a sequence (25mer). The 
// reverse() method is called because sequences taken from the .pgf files are in anti-sense orientation.
Probe::Probe(std::string pid, std::string s, std::string psid) {
	probe_id_ = pid;
	seq_ = s;
	probe_set_id_ = psid;
	reverse();
}


// Prints contents of the probe to stdout
void Probe::printProbe() {
	std::cout << "\t" << id_ << "\t" << seq_ << std::endl;
}

// Prints contents of the probe to stdout in FASTA format
void Probe::printFASTA() {
	if(id_.length() > 1) {
		std::cout << ">" << id_ << std::endl;
		std::cout << seq_ << std::endl;
	}
}

// Sets the enclosing transcript cluster ID of the probe. Additionally sets the number of 
// probes in the enclosing probe set.
void Probe::setTCID(std::string tc_id, int count) {
	tc_id_ = tc_id;
	probes_in_probeset_ = count;
}

// Occurs after the number of probes in the enclosing transcript cluster have been calculated. As
// this is the final bit of information needed to construct the probe's special id_, the id_ is also
// set in this function in the format of:
// (TRANSCRIPT CLUSTER ID) - (PROBE SET ID) - (PROBE ID) - (# PROBES IN TRANSCRIPT CLUSTER) - (# PROBES IN PROBE SET)
void Probe::setProbeCounts(int counts) {
	probes_in_tc_ = counts;
	id_ = tc_id_ + "-" + probe_set_id_ + "-" + probe_id_ + "-" + std::to_string(probes_in_tc_) + "-" + std::to_string(probes_in_probeset_);
}

// Changes the contained probe sequence from sense to anti sense orientation or vice versa
void Probe::reverse() {

	int low = 0;
	int high = seq_.length() - 1;
	
	while(low < high) {
		char a = seq_[low];
		char b = seq_[high];
		
		seq_[high] = a;
		seq_[low] = b;
		
		low++;
		high--;
		
	}


}
