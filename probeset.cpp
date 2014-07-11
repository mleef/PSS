#include "probeset.h"


ProbeSet::ProbeSet() {
	probe_set_id_ = ""; 
	probe_count_ = 0;
}


ProbeSet::ProbeSet(std::string id) {
	probe_set_id_ = id; 
	probe_count_ = 0;
}

void ProbeSet::addProbe(Probe p) {
	probes.insert(ProbePair(p.getPID(), p));
	probe_count_++;
}

void ProbeSet::setTCID(std::string id) {
	tc_id_ = id;
	for(it_type iterator = probes.begin(); iterator != probes.end(); iterator++) {
    // iterator->first = key
    	iterator->second.setTCID(tc_id_, probe_count_);
    // Repeat if you also want to iterate through the second map.
	}	
}

void ProbeSet::printProbeSet() {
	std::cout << tc_id_  << "\t" << probe_set_id_ << "\t" << probe_count_ << std::endl;
	for(it_type iterator = probes.begin(); iterator != probes.end(); iterator++) {
    	iterator->second.printProbe();
	}
	
	std::cout << std::endl;
}

void ProbeSet::printFASTA() {
	for(it_type iterator = probes.begin(); iterator != probes.end(); iterator++) {
    	iterator->second.printFASTA();
	}
	
}

void ProbeSet::setProbeCounts(int counts) {
	for(it_type iterator = probes.begin(); iterator != probes.end(); iterator++) {
    	iterator->second.setProbeCounts(counts);
	}	
}
