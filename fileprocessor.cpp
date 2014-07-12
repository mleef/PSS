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
    TranscriptClusterMap transcriptclusters;
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
	std::vector<ProbeSet> psList;
	
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
					tc_iter iter = transcriptclusters.find(tc_id); 
					if(iter != transcriptclusters.end()){				
						iter->second.push_back(it->second);	
					}
					else {
						psList.push_back(it->second);
						transcriptclusters.insert(TranscriptClusterPair(tc_id, psList));	
						psList.clear();
					}
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
						tc_iter iter = transcriptclusters.find(tc_id); 
						if(iter != transcriptclusters.end()){			
							iter->second.push_back(it->second);	
						}
						else {
							psList.push_back(it->second);
							transcriptclusters.insert(TranscriptClusterPair(tc_id, psList));	
							psList.clear();
						}
					}
					
				
					
				}
				
				
			}
			
		}
	
	
	
	}
	
	for(tc_iter iterator = transcriptclusters.begin(); iterator != transcriptclusters.end(); iterator++) {
		int probes = 0;
		for(int i = 0; i < iterator->second.size(); i++) {
			probes += iterator->second.at(i).getProbeCount();
		}
		for(int i = 0; i < iterator->second.size(); i++) {
			ps_iter it = probesets.find(iterator->second.at(i).getPSID());
			if(it != probesets.end()) {
				it->second.setProbeCounts(probes);
			}
		}
		
		probes = 0;
	}	
	
	
	return probesets;
	
}

void FileProcessor::processBLAST(const char * b, bool exon) {
	styleHeadings();
	std::ifstream f1(b);
	std::string str;
	std::size_t found;
	
	std::string probe_set_id;
	std::string tc_id;
	
    std::vector<std::string> curLine;
    std::vector<std::string> ids;
    std::vector<bLine> lines;
	
	std::string curQuery = "-1";
	std::string prevQuery = "-1";
	bLine line;
	ProbeSetLine map;
	ProbeSetLine tc_map;
	
    while (std::getline(f1, str)){
    	found = str.find("#");
    	
		if(found == std::string::npos) {
			curLine = split(str,'\t');
			curQuery = curLine.at(0);
			if(curQuery != "-1" && prevQuery != "-1" && curQuery != prevQuery) {
				if(exon) {
					outputHTML(prevQuery, map, true);
					map.clear();	
				}
				
				else {
					outputHTML(prevQuery, tc_map, false);
					tc_map.clear();	
				}	
			}
			

			line.query_id = curLine.at(0);
			
			ids = split(curLine.at(1),'-');
			line.tc_id = ids.at(0);
			tc_id = ids.at(0);
			line.probe_set_id = ids.at(1);
			line.probes_in_probeset = atoi(ids.at(4).c_str());
			line.probes_in_tc = atoi(ids.at(3).c_str());
			probe_set_id = ids.at(1);
			line.probe_id = ids.at(2);
			
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
			prevQuery = curQuery;
			
			
			if(exon) {
				ProbeSetLine::iterator it = map.find(probe_set_id);
				if (it != map.end()) {
					it->second.push_back(line);
				}
			
				else {
					lines.push_back(line);
					map.insert(LinePair(probe_set_id, lines));
					lines.clear();
				}
			}
			else {
				ProbeSetLine::iterator itr = tc_map.find(tc_id);
				if (itr != tc_map.end()) {
					itr->second.push_back(line);
				}
			
				else {
					lines.push_back(line);
					tc_map.insert(LinePair(tc_id, lines));
					lines.clear();
				}
			}
			
		}
    }
    
    if(exon) {
    	outputHTML(curQuery, map, true);
    }
    else {
    	outputHTML(curQuery, tc_map, false);
    }
    
		
}

void FileProcessor::outputHTML(std::string query_id ,ProbeSetLine map, bool exon) {
	
	std::string header1;
	std::string header2;
	
	std::string pk = "712";
	
	if(exon) {
		header1 = "Probe Set ID";
		header2 = "Probe Hits/Probes in Probe Set";
	}
	else {
		header1 = "Transcript Cluster ID";
		header2 = "Probe Hits/Probes in Transcript Cluster";
	}
	
	std::cout << "<table>" << std::endl;
	std::cout << "<caption>" << query_id << "</caption>" << std::endl;
	std::cout << "<thead>" << std::endl;
	std::cout << "<tr>" << std::endl;
    std::cout << "<th>" << header1 << "</th>" << std::endl;
    std::cout << "<th>" << header2 << "</th>" << std::endl;
    std::cout << "<th>Hit Percentage</th>" << std::endl;
	std::cout << "</tr>" << std::endl;
    std::cout << "</thead>" << std::endl;
    
    
	
	for(cp_iter iterator = map.begin(); iterator != map.end(); iterator++) {
		int size = iterator->second.size();
    	for(int i = 0; i < size; i++) {
    		iterator->second.at(i).probe_hits = size;	
    		if(exon) {
    			iterator->second.at(i).percent = static_cast<double>(iterator->second.at(i).probe_hits)/static_cast<double>(iterator->second.at(i).probes_in_probeset) * 100;
    		}
    		else {
    		    iterator->second.at(i).percent = static_cast<double>(iterator->second.at(i).probe_hits)/static_cast<double>(iterator->second.at(i).probes_in_tc) * 100;
    		}
    	}  	
    }
 
    int maxCount = 0;
    std::string remove;
    CheckMap checkedProbeSets;
    check_iter checker;
    std::vector<LinePair> sortedProbeSets;
    LinePair pair;
    
    for(int i = 0; i < map.size(); i++) {
    	for(cp_iter iterator = map.begin(); iterator != map.end(); iterator++) {
    		if(iterator->second.size() > maxCount && checkedProbeSets.find(iterator->first) == checkedProbeSets.end()) {
    			remove = iterator->first;
    			maxCount = iterator->second.size();
    			pair.first = iterator->first;
    			pair.second = iterator->second;
    		}
		}
		std::string color = "";
		if(pair.second.at(0).percent > 80.00) {
			color = " id='good'";
		}
		else if(pair.second.at(0).percent < 20.00) {
			color = " id='bad'";
		}
		checkedProbeSets.insert(CheckPair(remove, true));
		if(exon) {
			std::cout << "<tr><td><a href='https://www.affymetrix.com/analysis/netaffx/exon/wtgene_probe_set.affx?pk=" << pk << ":" << pair.first << "' target='_blank'>" << pair.first << "<a/></td><td>" << pair.second.at(0).probe_hits << "/" << pair.second.at(0).probes_in_probeset << "</td><td" << color << ">" << pair.second.at(0).percent << "%" << "</td></tr>" << std::endl;
		}
		else {
			std::cout << "<tr><td><a href='https://www.affymetrix.com/analysis/netaffx/exon/wtgene_transcript.affx?pk=" << pk << ":" << pair.first << "' target='_blank'>" << pair.first << "<a/></td><td>" << pair.second.at(0).probe_hits << "/" << pair.second.at(0).probes_in_tc << "</td><td" << color << ">" << pair.second.at(0).percent << "%" << "</td></tr>" << std::endl;		
		}
		//sortedProbeSets.push_back(pair);
		maxCount = 0;
	}
	
	std::cout << "</table>" << std::endl;
}	

void FileProcessor::styleHeadings() {
	std::cout << "<style>" << std::endl;
	std::cout << "table" << std::endl;
	std::cout << "{" << std::endl;
	std::cout << "font-family: 'Lucida Sans Unicode', 'Lucida Grande', Sans-Serif;" << std::endl;
	std::cout << "font-size: 12px;" << std::endl;
	std::cout << "margin: 45px;" << std::endl;
	std::cout << "width: 550px;" << std::endl;
	//std::cout << "margin: 0px auto;" << std::endl;
	std::cout << "border-collapse: collapse;" << std::endl;
	std::cout << "}" << std::endl;
	std::cout << "th" << std::endl;
	std::cout << "{" << std::endl;
	std::cout << "text-align: center;" << std::endl;
	std::cout << "font-size: 13px;" << std::endl;
	std::cout << "font-weight: normal;" << std::endl;
	std::cout << "padding: 8px;" << std::endl;
	std::cout << "background: #b9c9fe;" << std::endl;
	std::cout << "border-top: 4px solid #aabcfe;" << std::endl;
	std::cout << "border-bottom: 1px solid #fff;" << std::endl;
	std::cout << "color: #039;" << std::endl;
	std::cout << "}" << std::endl;

	std::cout << "td" << std::endl;
	std::cout << "{" << std::endl;
	std::cout << "text-align: center;" << std::endl;
	std::cout << "padding: 8px;" << std::endl;
	std::cout << "background: #e8edff;" << std::endl;
	std::cout << "border-bottom: 1px solid #fff;" << std::endl;
	std::cout << "color: #669;" << std::endl;
	std::cout << "border-top: 1px solid transparent;" << std::endl;
	std::cout << "}" << std::endl;

	std::cout << "tr:hover td" << std::endl;
	std::cout << "{" << std::endl;
	std::cout << "background: #d0dafd;" << std::endl;
	std::cout << "color: #339;" << std::endl;
	std::cout << "}" << std::endl;

	std::cout << "caption" << std::endl;
	std::cout << "{" << std::endl;
	std::cout << "margin-bottom: 5px;" << std::endl;
	std::cout << "text-transform: lowercase;" << std::endl;
	std::cout << "font-size: 160%;" << std::endl;
	std::cout << "padding: 5px;" << std::endl;
	std::cout << "letter-spacing: .2px;" << std::endl;
	std::cout << "font-weight: bold;" << std::endl;
	std::cout << "}" << std::endl;

	std::cout << "#good {" << std::endl;
	std::cout << "color: #32CD32;" << std::endl;
	std::cout << "}" << std::endl;

	std::cout << "#bad {" << std::endl;
	std::cout << "color: red;" << std::endl;
	std::cout << "}" << std::endl;

	std::cout << "</style>" << std::endl;
}
	


