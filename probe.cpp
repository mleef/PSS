#include "probe.h"


Probe::Probe(std::string pid, std::string s, std::string psid) {
	probe_id_ = pid;
	seq_ = s;
	probe_set_id_ = psid;
	reverse();
}



void Probe::printProbe() {
	std::cout << "\t" << id_ << "\t" << seq_ << std::endl;
}

void Probe::printFASTA() {
	std::cout << ">" << id_ << std::endl;
	std::cout << seq_ << std::endl;
}

void Probe::setTCID(std::string tc_id, int count) {
	tc_id_ = tc_id;
	probes_in_probeset_ = count;
}

void Probe::setProbeCounts(int counts) {
	probes_in_tc_ = counts;
	id_ = tc_id_ + "-" + probe_set_id_ + "-" + probe_id_ + "-" + std::to_string(probes_in_tc_) + "-" + std::to_string(probes_in_probeset_);
}

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
