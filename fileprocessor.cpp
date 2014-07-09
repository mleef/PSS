#include "fileprocessor.h"

FileProcessor::FileProcessor() {


}


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

void FileProcessor::processBLAST(const char * b) {
	std::ifstream f1(b);
	std::string str;
	std::string probe_set_id;
	std::string probe_id;
	std::string seq;
    std::size_t found;
    std::vector<std::string> curLine;
    
    ProbeSetMap probesets;
    ProbeSet ps = ProbeSet("-1");
    
    while (std::getline(f1, str)){
    }

}


