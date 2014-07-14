/* 
   Implementation file of the probe set class. 
*/
#include "probeset.h"

// Constructor, initializes ID and contained probes to zero values.
ProbeSet::ProbeSet() {
	probe_set_id_ = ""; 
	probe_count_ = 0;
}

// Overloaded constructor, takes in probe set ID
ProbeSet::ProbeSet(std::string id) {
	probe_set_id_ = id; 
	probe_count_ = 0;
}

// Inserts new probe into probe map
void ProbeSet::addProbe(Probe p) {
	probes.insert(ProbePair(p.getPID(), p));
	probe_count_++;
}

// Passes the transcript cluster ID information down to all contained probe objects
void ProbeSet::setTCID(std::string id) {
	tc_id_ = id;
	for(it_type iterator = probes.begin(); iterator != probes.end(); iterator++) {
    // iterator->first = key
    	iterator->second.setTCID(tc_id_, probe_count_);
    // Repeat if you also want to iterate through the second map.
	}	
}

// Outputs the contained probes to stdout
void ProbeSet::printProbeSet() {
	std::cout << tc_id_  << "\t" << probe_set_id_ << "\t" << probe_count_ << std::endl;
	for(it_type iterator = probes.begin(); iterator != probes.end(); iterator++) {
    	iterator->second.printProbe();
	}
	
	std::cout << std::endl;
}

// Used to output probe set contents in FASTA format
void ProbeSet::printFASTA() {
	for(it_type iterator = probes.begin(); iterator != probes.end(); iterator++) {
    	iterator->second.printFASTA();
	}
	
}

// Passes information about the number of probes in the transcript cluster down to the individual probes
void ProbeSet::setProbeCounts(int counts) {
	for(it_type iterator = probes.begin(); iterator != probes.end(); iterator++) {
    	iterator->second.setProbeCounts(counts);
	}	
}
