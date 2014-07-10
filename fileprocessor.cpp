#include "fileprocessor.h"

FileProcessor::FileProcessor() {}


std::vector<std::string> & FileProcessor::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> FileProcessor::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}


std::vector<Sequence> FileProcessor::readFASTA(const char * f) {

	std::ifstream file(f);
	std::string str;
	std::string name;
	std::string description;
	std::string seq;
    std::size_t found;
    bool first = true;
    std::vector<Sequence> sequences;
    
    
    while (std::getline(file, str))
    {
		found = str.find(">");
		if(found != std::string::npos) {
			
			if(!first) {
				sequences.push_back(Sequence(name,description,seq));
			}
			
			name = "";
			seq = "";
			description = "";
			
			str.erase(std::remove(str.begin(), str.end(), '>'), str.end());
			
			found = str.find(" ");
			if(found != std::string::npos) {
				description = str.substr(found + 1);
				name = str.substr(0, found);
			}
			
			else {
				name = str;
			}
			
			first = false;
		}
		
		else if(str.length() > 0) {
			seq += str;
		}
    }
    sequences.push_back(Sequence(name,description,seq));
    file.close();
	return sequences;


}

void FileProcessor::writeFASTA(std::vector<Sequence> sequences) {

		std::ofstream file("out.fa");
		for(int i = 0; i < sequences.size(); i++) {
			file << ">" << sequences.at(i).getName() << " " << sequences.at(i).getDescription() << std::endl;
    		file << sequences.at(i).getSeq() << std::endl;	
		
		}
		file.close();
		

} 

ProbeSetMap FileProcessor::processLibraryFiles(const char * pgf, const char * mps) {
	std::ifstream f1(pgf);
	std::string str;
	std::string probe_set_id;
	std::string probe_id;
	std::string seq;
    std::size_t found;
    std::vector<std::string> curLine;
    
    ProbeSetMap probesets;
    ProbeSet ps = ProbeSet("-1");
    
    while (std::getline(f1, str)){
    
		found = str.find("#%");
		if(found == std::string::npos) {
			
			curLine = split(str,'\t');
			if(curLine.size() < 8) {
				if(curLine.at(0).length() > 0) {
					if(ps.getPSID() != "-1") {
						probesets.insert(ProbeSetPair(probe_set_id, ps));
						ps = ProbeSet("-1");
					}
					probe_set_id = curLine.at(0);
					ps.setPSID(probe_set_id);
					
				}

			}
			
			else {
				ps.addProbe(Probe(curLine.at(2), curLine.at(7), probe_set_id));;
			}
			
		}
		
	}
	
	f1.close();
	
	std::string tc_id;
	std::vector<std::string> listPS;
	
	std::ifstream f2(mps);
	
	while (std::getline(f2, str)){
		found = str.find("#%");
		if(found == std::string::npos) {
			curLine = split(str,'\t');
			if(curLine.at(1).length() < 1) {
				probe_set_id = curLine.at(0);
				tc_id = curLine.at(2);
				ps_iter it = probesets.find(probe_set_id);
				if(it != probesets.end()) {
					it->second.setTCID(tc_id);
				}
			}
			
			else {
			
				listPS = split(curLine.at(2),' ');
				
				tc_id = curLine.at(1);
				for(int i = 0; i < listPS.size(); i++) {
					probe_set_id = listPS.at(i);				
					ps_iter it = probesets.find(probe_set_id);
					if(it != probesets.end()) {
						it->second.setTCID(tc_id);
					}
					
				}
				
				
			}
			
		}
	
	
	
	}
	
	
	
	return probesets;
	
}

void FileProcessor::processBLAST(const char * b, const char * pss) {
	std::ifstream f1(b);
	std::string str;
	std::size_t found;
	
	std::string probe_set_id;
	
    std::vector<std::string> curLine;
    std::vector<std::string> ids;
    std::vector<bLine> lines;
	
	bLine line;
	ProbeSetCount map;
    
    while (std::getline(f1, str)){
    	found = str.find("#");
    	
		if(found == std::string::npos) {
			curLine = split(str,'\t');
			
			line.query_id = curLine.at(0);
			ids = split(curLine.at(1),'-');
			line.tc_id = ids.at(0);
			line.probe_set_id = ids.at(1);
			line.total_probes = atoi(ids.at(2).c_str());
			probe_set_id = ids.at(1);
			line.probe_id = ids.at(3);
			line.perc_identity = curLine.at(2);
			line.length = curLine.at(3);
			line.mismatches = curLine.at(4);
			line.gaps = curLine.at(5);
			line.q_start = curLine.at(6);
			line.q_end = curLine.at(7);
			line.s_start = curLine.at(8);
			line.s_end = curLine.at(9);
			line.evalue = curLine.at(10);
			line.score = curLine.at(11);
			
			
			ProbeSetCount::iterator it = map.find(probe_set_id);
			if (it != map.end()) {
				it->second.push_back(line);
			}
			
			else {
				lines.push_back(line);
				map.insert(CountPair(probe_set_id, lines));
				lines.clear();
			}
			
		}
    }
 
//sorts map by probe hits   
//     int maxCount = 0;
//     std::string remove;
//     CheckMap checkedProbeSets;
//     check_iter checker;
//     std::vector<CountPair> sortedProbeSets;
//     CountPair pair;
//     
//     for(int i = 0; i < map.size(); i++) {
//     	for(cp_iter iterator = map.begin(); iterator != map.end(); iterator++) {
//     		if(iterator->second.size() > maxCount && checkedProbeSets.find(iterator->first) == checkedProbeSets.end()) {
//     			remove = iterator->first;
//     			maxCount = iterator->second.size();
//     			pair.first = iterator->first;
//     			pair.second = iterator->second;
//     		}
// 		}
// 		
// 		checkedProbeSets.insert(CheckPair(remove, true));
// 		sortedProbeSets.push_back(pair);
// 		maxCount = 0;
// 		
// 	}
	for(cp_iter iterator = map.begin(); iterator != map.end(); iterator++) {
		int size = iterator->second.size();
    	for(int i = 0; i < size; i++) {
    		iterator->second.at(i).probe_hits = size;	
    		iterator->second.at(i).percent = static_cast<double>(iterator->second.at(i).probe_hits)/static_cast<double>(iterator->second.at(i).total_probes) * 100;
    	}
    	
    	std::cout << iterator->first << " " << iterator->second.at(0).probe_hits << "/" << iterator->second.at(0).total_probes << " " << iterator->second.at(0).percent << "%" << std::endl;
    }
	
	

}


// void FileProcessor::calculateProbeSetScores(ProbeSetCount ps, const char * pss) {
// 	std::ifstream file(pss);
// 	std::string str;
// 	std::size_t found;
// 	std::vector<std::string> curLine;
// 	
// 	int numFound = 0;
// 	int total = ps.size();
// 	
// 
// 	double percent = 0.0;
// 	
// 	
// 	while (std::getline(file, str)){
// 		curLine = split(str,'\t');
// 		if(curLine.size() > 0) {
// 			if(curLine.at(0).length() > 0) {
// 				cp_iter found = ps.find(curLine.at(1));
// 				if(found != ps.end()) {
// 					int size = found->second.size();
// 					for(int i = 0; i < size; i++) {
// 						found->second.at(i).total_probes = atoi(curLine.at(2).c_str());
// 						found->second.at(i).percent = static_cast<double>(found->second.at(i).probe_hits)/static_cast<double>(found->second.at(i).total_probes) * 100;
// 						percent += static_cast<double>(atoi(found->second.at(i).perc_identity.c_str()));
// 					}
// 					percent = percent / size;
// 					std::cout << found->first << "\t" << found->second.at(0).probe_hits << "/" << found->second.at(0).total_probes << " " << found->second.at(0).percent << "%" << " Average Probe Set Identity: " << percent << std::endl;
// 					percent = 0.0;
// 					numFound++;
// 					if(numFound == ps.size()) {
// 						break;
// 					}
// 				}
// 			}
// 		}
//     
//     }
// 	
// 	
// 	
// }
// 
// 
