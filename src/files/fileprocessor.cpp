/* 
   Implementation file for the fileprocessor object.
*/

#include "fileprocessor.h"

FileProcessor::FileProcessor() {}

// Part of the split function, which tokenizes a string based on a delimiter and returns a vector of smaller strings
std::vector<std::string> & FileProcessor::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

// Part of the split function, which tokenizes a string based on a delimiter and returns a vector of smaller strings
std::vector<std::string> FileProcessor::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

// Deprecated function, reads in FASTA file and returns list of sequence objects
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

// Deprecated function, writes FASTA from list of sequence objects
void FileProcessor::writeFASTA(std::vector<Sequence> sequences) {

		std::ofstream file("out.fa");
		for(int i = 0; i < sequences.size(); i++) {
			file << ">" << sequences.at(i).getName() << " " << sequences.at(i).getDescription() << std::endl;
    		file << sequences.at(i).getSeq() << std::endl;	
		
		}
		file.close();
		

} 

// Takes in a .pgf as its first argument and a .mps as its second argument and returns
// a mapping of probe sets. The crux of this function is the construction of the unique
// PSS IDs which are then used to calculate hits and percentages later on.
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
    int pc = 0;
    
    // Read in .pgf file, create a mapping of probe set names to populated probe set objects
    while (std::getline(f1, str)){
    
		found = str.find("#%");
		// If not a comment line
		if(found == std::string::npos) {
			
			curLine = split(str,'\t');
			// If the list of probes for the given probe set has ended, store built up probe set, create new one, and move on
			if(curLine.size() < 8) {
				if(curLine.at(0).length() > 0) {
					if(ps.getPSID() != "-1") {
						probesets.insert(ProbeSetPair(probe_set_id, ps));
						ps = ProbeSet("-1");
					}
					probe_set_id = curLine.at(0);
					ps.setPSID(probe_set_id);
					if(curLine.at(2).length() > 0) {
						ps.setName(curLine.at(2));
					}
					
				}

			}
			// Current line is still probe within the current probe set, add it to list and move on
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
	
	// Read in .mps file, attach transcript cluster information to probe set mapping
	while (std::getline(f2, str)){
		found = str.find("#%");
		// If not a comment line
		if(found == std::string::npos) {
			curLine = split(str,'\t');
			// If there is only one probe set in the transcript cluster
			if(curLine.at(2).length() < 10) {
				probe_set_id = curLine.at(2);
				tc_id = curLine.at(0);
				//pc = atoi(curLine.at(3).c_str());
				ps_iter it = probesets.find(probe_set_id);
				// If the probe set id already exists in the mapping, set the transcript cluster id
				if(it != probesets.end()) {
					it->second.setTCID(tc_id);
					//it->second.setProbeCounts(pc);
					// TODO: Probe count appears to simply be stored at the end of the line, no need to store this info 
					// for later calculation
					
					// Store probe set in transcript cluster for calculation of probe counts
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
			
			// Multiple probe sets in transcript cluster, iterate through list and update transcript cluster
			// ID
			else {
			
				listPS = split(curLine.at(2),' ');
				//pc = atoi(curLine.at(3).c_str());
				
				tc_id = curLine.at(0);
				for(int i = 0; i < listPS.size(); i++) {
					probe_set_id = listPS.at(i);				
					ps_iter it = probesets.find(probe_set_id);
					if(it != probesets.end()) {
						it->second.setTCID(tc_id);
						//it->second.setProbeCounts(pc);
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
	
	// Calculate number of probes in each transcript cluster.
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

// Reads in BLAST output in tab format and creates a mapping from probe set/transcript cluster
// IDs to a list of lines that represent hits on that probe set/transcript cluster. The bool
// parameter is used to adjust method for exon/gene level analysis. 
void FileProcessor::processBLASTTabs(const char * b, ProbeScoreMap probes, bool exon, std::string id) {
	//styleHeadings();
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
	
	
	//TODO: Determine if multiple same ID probes in probe set is a bad thing, same probe set, different transcript clusters
	ProbeSetLine map;
	ProbeSetLine tc_map;

	
	
	// Read in BLAST output
    while (std::getline(f1, str)){
    	found = str.find("#");
    	
    	// If not a comment
		if(found == std::string::npos) {
			curLine = split(str,'\t');
			curQuery = curLine.at(0);
			
			// End of lines for specific query, output the HTML formatted version of the current mapping and move on
			if(curQuery != "-1" && prevQuery != "-1" && curQuery != prevQuery) {
				if(exon) {
					outputHTML(prevQuery, map, true, id, probes);
					map.clear();	
				}
				
				else {
					outputHTML(prevQuery, tc_map, false, id, probes);
					tc_map.clear();	
				}	
			}
			
			// Populate line object with proper data
			
			line.query_id = curLine.at(0);
			
			// IMPORTANT: Parse the PSS ID into relevant values
			ids = split(curLine.at(1),'-');
			line.tc_id = ids.at(0);
			tc_id = ids.at(0);
			line.probe_set_id = ids.at(1);
			line.probes_in_probeset = atoi(ids.at(4).c_str());
			line.probes_in_tc = atoi(ids.at(3).c_str());
			probe_set_id = ids.at(1);
			line.probe_id = ids.at(2);
			if(ids.size() == 6) {
				line.probe_set_name = ids.at(5);
			}
			
			// Get score and href link from calculated alignment map
			pscore_iter it = probes.find(line.probe_id);
			if(it != probes.end()) {
				line.href = it->second.href;
				line.hyb_score = it->second.hyb_score;
				line.qseq = it->second.qseq;
				line.hseq = it->second.hseq;
				line.midline = it->second.midline;

			}
			
			
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
			
			// Add line to probe set/transcript cluster mapping list
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
    
    
//  for(ProbeSetLine::iterator iterator = map.begin(); iterator != map.end(); iterator++) {
//     	std::cout << iterator->first << ": " << iterator->second.size() << std::endl;
//     	for(int i = 0; i < iterator->second.size(); i++) {
//     		std::cout << iterator->second.at(i).probe_id << std::endl;
//     	}
// 	}
//     
    // Output last query list to HTML format
   if(exon) {
    	outputHTML(curQuery, map, true, id, probes);
    }
    else {
    	outputHTML(curQuery, tc_map, false, id, probes);
    }
//     

	
		
}

// Creates the tables for exon/gene level viewing by the user
void FileProcessor::outputHTML(std::string query_id, ProbeSetLine map, bool exon, std::string id, ProbeScoreMap probes) {
	
	std::string header1;
	std::string header2;
	
	std::vector<ProbeLookup> probeLocations;
	
	std::string tableDeclaration = "<table class='maintable'>";
	
	std::string baseURL = "https://www.affymetrix.com/analysis/netaffx/";
	std::string psExtension = "exon/wtgene_probe_set.affx?pk=";
	std::string tcExtension = "exon/wtgene_transcript.affx?pk=";
	int pkint = atoi(id.c_str());
	
	if(pkint == 1) {
		psExtension = "exon/probe_set.affx?pk=";
		tcExtension = "exon/transcript.affx?pk=";
	}
	
	else if(pkint == 4 || pkint == 712) {
		psExtension = "exon/wtgene_probe_set.affx?pk=";
		tcExtension = "exon/wtgene_transcript.affx?pk=";
	}
	
	else if(pkint == 725) {
		psExtension = "exon/hta_probe_set.affx?pk=";
		tcExtension = "exon/hta_transcript.affx?pk=";
	}

	
	
	std::string pk = id;
	std::string eqid = query_id;
	eqid.erase(std::remove(eqid.begin(), eqid.end(), ':'), eqid.end());
	eqid.erase(std::remove(eqid.begin(), eqid.end(), '.'), eqid.end());
	eqid.erase(std::remove(eqid.begin(), eqid.end(), '-'), eqid.end());
	eqid.erase(std::remove(eqid.begin(), eqid.end(), '|'), eqid.end());
	eqid.erase(std::remove(eqid.begin(), eqid.end(), ' '), eqid.end());
	
	if(exon) {
		header1 = "Probe Set ID";
		header2 = "Unique Probe Hits/Probes in Probe Set";
	}
	else {
		header1 = "Transcript Cluster ID";
		header2 = "Unique Probe Hits/Probes in Transcript Cluster";
	}
	
	
	//std::cout << "<div data-scroll-reveal>" << std::endl;
	std::cout << "<div id ='titles' style='display:none'>" << eqid << "</div>";
	std::cout << "<div id='tablecontainer'>" << std::endl;
	std::cout << tableDeclaration << std::endl;
	std::cout << "<caption>" << query_id << "</caption>" << std::endl;
	std::cout << "<thead>" << std::endl;
	std::cout << "<tr>" << std::endl;
	std::cout << "<th></th>" << std::endl;
    std::cout << "<th>" << header1 << "</th>" << std::endl;
    std::cout << "<th>" << header2 << "</th>" << std::endl;
    std::cout << "<th>Hit Percentage</th>" << std::endl;
    std::cout << "<th>Add to Novel Probe Grouping</th>" << std::endl;

	std::cout << "</tr>" << std::endl;
    std::cout << "</thead>" << std::endl;
    
    
    CheckMap seen;
    check_iter ci;
    int uniqueProbes = 0;
	// Calculate the hit percentage and store that information in each line obect
	for(cp_iter iterator = map.begin(); iterator != map.end(); iterator++) {
		int size = iterator->second.size();
		// Calculate unique probe hits
		for(int i = 0; i < size; i++) {
			ci = seen.find(iterator->second.at(i).probe_id);
			if(ci == seen.end()) {
				if(iterator->second.at(i).hyb_score > 37) {
					uniqueProbes++;
				}
				seen.insert(CheckPair(iterator->second.at(i).probe_id, true));
			}
		}
		
		
    	for(int i = 0; i < size; i++) {
    		iterator->second.at(i).probe_hits = uniqueProbes;	
    		if(exon) {
    			double p = static_cast<double>(iterator->second.at(i).probe_hits)/static_cast<double>(iterator->second.at(i).probes_in_probeset) * 100;
    			iterator->second.at(i).percent = floor( p * 100.00 + 0.5 ) / 100.00;
    		}
    		else {
    		    double p = static_cast<double>(iterator->second.at(i).probe_hits)/static_cast<double>(iterator->second.at(i).probes_in_tc) * 100;
    		    iterator->second.at(i).percent = floor( p * 100.00 + 0.5 ) / 100.00;
    		}
    	}
    	
    	seen.clear();  	
    	uniqueProbes = 0;
    }
 
 	// Print the map in order of hit count
    int maxCount = 0;
    bool drawRow = false;
    std::string remove;
    CheckMap checkedProbeSets;
    check_iter checker;
    LinePair pair;
    
    //Statistics for sequence summary
    bool topRanked = true;
    std::string topRankedID;
    int numPS = 0;
    int numP = 0;
    std::string type;
    if(exon) {
    	type = "exon";
    }
    else {
    	type = "gene";
    }
    
    // Draw rows and subrows
    for(int i = 0; i < map.size(); i++) {
    	for(cp_iter iterator = map.begin(); iterator != map.end(); iterator++) {
    		if(iterator->second.at(0).probe_hits > maxCount && checkedProbeSets.find(iterator->first) == checkedProbeSets.end()) {
    			remove = iterator->first;
    			maxCount = iterator->second.at(0).probe_hits;
    			pair.first = iterator->first;
    			pair.second = iterator->second;
    			drawRow = true;
    		}
		}
		
		if(drawRow) {
			numPS++;
			// Color code table cell based on hit percentage
			std::string color = "";
			if(pair.second.at(0).percent > 80.00) {
				color = " id='good'";
			}
			else if(pair.second.at(0).percent < 20.00) {
				color = " id='bad'";
			}
			checkedProbeSets.insert(CheckPair(remove, true));
		

			// Output table row
			// TODO: Create href mappings between designs and pks for proper urls
			std::string cbid;
			std::string probeLines = "<tr id='nc'><th id='nc'>Probe ID</th><th id='nc'>% Identity</th><th id='nc'>Start</th><th id='nc'>Stop</th><th id='nc'>EValue</th><th id='nc'>Bit Score</th><th id='nc'>Hybridization Score</th><th id='nc'>Add to Novel Probe Grouping</th></tr>";
			for(int m = 0; m < pair.second.size(); m++) {
				if(pair.second.at(m).hyb_score > 37) {
					if(exon) {
						cbid = pair.second.at(m).probe_set_id;
					}
					else {
						cbid = pair.second.at(m).tc_id;
					}
					probeLines += "<tr class='" + std::to_string(pair.second.at(m).hyb_score) + "' id='nc'><td id='nc'><a id='nc' title='alignment' style='display:block' href='#" + pair.second.at(m).href + "'>" + pair.second.at(m).probe_id + "</a></td><td id='nc'>" + pair.second.at(m).perc_identity + "</td><td id='nc'>" + pair.second.at(m).q_start + "</td><td id='nc'>" + pair.second.at(m).q_end + "</td><td id='nc'>" + pair.second.at(m).evalue + "</td><td id='nc'>" + pair.second.at(m).score + "</td><td id='nc'>" + std::to_string(pair.second.at(m).hyb_score) + "</td><td id='nc'><input class='checkboxes " + cbid + "' id='nc' type='checkbox' name='probe' value='" + pair.second.at(m).probe_id +  "'></td></tr>";
					probeLocations.push_back(ProbeLookup(pair.second.at(m).probe_id, pair.second.at(m).q_start));
					numP++;
				}
			}
			
			

		
			if(exon) {
				//tr data-scroll-reveal TODO: animate rows downward instead of just upward
				//Check for probe set name for href link
				std::string link = "";
				if(pair.second.at(0).probe_set_name.length() > 1) {
					link = pair.second.at(0).probe_set_name;
				}
			
				else {
					link = pair.first;
				}
				std::cout << "<tr><td>+</td><td><a href='" << baseURL + psExtension << pk << ":" << link << "' target='_blank'>" << pair.first << "<a/></td><td>" << pair.second.at(0).probe_hits << "/" << pair.second.at(0).probes_in_probeset << "</td><td" << color << ">" << pair.second.at(0).percent << "%" << "</td><td><input class='checkboxes' id='nc' type='checkbox' name='probeset' value='" << pair.first << "'></td></tr><tr><td id='nopad' colspan='8'><div id='subtablecontainer'><table class='subtable'>" << probeLines << "</table></div></td></tr>" << std::endl;
			}
			else {
				std::cout << "<tr><td>+</td><td><a href='" << baseURL + tcExtension << pk << ":" << pair.first << "' target='_blank'>" << pair.first << "<a/></td><td>" << pair.second.at(0).probe_hits << "/" << pair.second.at(0).probes_in_tc << "</td><td" << color << ">" << pair.second.at(0).percent << "%" << "</td><td><input class='checkboxes' id='nc' type='checkbox' name='probeset' value='" << pair.first << "'></td></tr><tr><td id='nopad' colspan='8'><div id='subtablecontainer'><table class='subtable'>" << probeLines << "</table></div></td></tr>"  << std::endl;		
			}
			maxCount = 0;
			if(topRanked) {
					topRankedID = pair.first;
    				topRanked = false;
    		}
			drawRow = false;
		}
	}
	
	
	// Add invisible bottom row to table to correct moving rows/columns
	std::cout << "<tr style='visibility: hidden' id='nc'><td id='nc'>-</td><td id='nc'>ID</td><td id='nc'>N/N</td><td id='nc'>0%</td><td id='nc'>N/A</td></tr><tr id='nc'><td style='visibility: hidden' id='nopad' colspan='7'><div id='subtablecontainer'><table class='subtable'><tr id='nc'><th id='nc'>Probe ID</th><th id='nc'>% Identity</th><th id='nc'>Start</th><th id='nc'>Stop</th><th id='nc'>EValue</th><th id='nc'>Bit Score</th><th id='nc'>Hybridization Score</th><th id ='nc'>Add to Novel Probe Set</th></tr></table></div></tr>"  << std::endl;		
	std::cout << "</table>" << std::endl;
	std::cout << "</div>" << std::endl;	
	std::cout << "<div id ='" << eqid << "' style='display:none'>";
	
	// Embed probe location data in dom for histogram calculation
	for(int i = 0; i < probeLocations.size(); i++) {
		std::string score;
		//std::cout << "<div id='all'>" << probeLocations.at(i).second << "</div>";
		pscore_iter it = probes.find(probeLocations.at(i).first);
		if(it != probes.end()) {
			score = std::to_string(it->second.hyb_score);
		}
		std::cout << "<div id='s" << score << "'>" << probeLocations.at(i).second << "</div>";
	
	}
	std::cout << "</div>" << std::endl;
	
	// Embed summary data in DOM for summary page calculation
	std::cout << "<div title = '" << query_id << "' id = 'summary' style='display:none'>" << std::endl;
	std::cout << "<div id = 'type'>" << type << "</div>" << std::endl;
	std::cout << "<div id = 'top'>" << topRankedID << "</div>" << std::endl;
	std::cout << "<div id = 'numps'>" << numPS << "</div>" << std::endl;
	std::cout << "<div id = 'nump'>" << numP << "</div>" << std::endl;
	std::cout << "</div>" << std::endl;



	
	
	std::cout << "<hr>" << std::endl;
}
	



//Reads in BLAST output in XML format to process alignment strings for hybridization scores
ProbeScoreMap FileProcessor::processBLASTAlignments(const char * b) {
	std::ifstream f1(b);
	std::string str;
	std::size_t found;
	std::size_t fo1;
	std::size_t fo2;
	std::size_t flength;
	
	
	std::string id;
	std::string probe_id;
	std::string probe_set_id;
	std::string tc_id;
	std::string href = "-1";
	int start;
	int stop;
	int length = 0;
	std::string qseq;
	std::string hseq;
	std::string midline;
	
    std::vector<std::string> curLine;
    std::vector<std::string> ids;
	
	std::string wordToFind = "<Hit_id>";

	std::string data;
	
	ProbeScoreMap probes;
	
	
	// Read in BLAST XML output
    while (std::getline(f1, str)){
    	found = str.find(wordToFind);
    	

		if(found != std::string::npos) {
			fo1 = str.find(">");
			fo2 = str.find("</");	
			flength = fo2 - fo1 - 1;	
			data = str.substr(fo1 + 1, flength);
			
			
			if(wordToFind == "<Hit_id>") {
				if(href != "-1") {
					alignment a;
					int hscore = 0;
					href = href.substr(4, href.length()-4);
					std::size_t p = href.find("|");
					href.replace(p, 1, ":");
					if(start < stop) {
						hscore = calculateHybScore(start, midline);
					}
					else {
						hscore = calculateHybScore(stop, midline);
					}
					a.hyb_score = hscore;
					a.href = href;
					a.hseq = hseq;
					a.qseq = qseq;
					
					
					a.midline = midline;
					probes.insert(ProbeScorePair(probe_id, a));
					
				}
				href = data;
				wordToFind = "<Hit_def>";
			}
			
			else if(wordToFind == "<Hit_def>") {
				id = data;
				ids = split(data, '-');
				tc_id = ids.at(0);
				probe_set_id = ids.at(1);
				probe_id = ids.at(2);
				wordToFind = "<Hsp_hit-from>";	
			}
			
			else if(wordToFind == "<Hsp_hit-from>") {
				start = atoi(data.c_str());
				wordToFind = "<Hsp_hit-to>";
			}
			
			else if(wordToFind == "<Hsp_hit-to>") {
				stop = atoi(data.c_str());
				length = std::abs(stop - start);
				wordToFind = "<Hsp_qseq>";
			}
			
			else if(wordToFind == "<Hsp_qseq>") {
				qseq = data;
				wordToFind = "<Hsp_hseq>";
			}
			
			else if(wordToFind == "<Hsp_hseq>") {
				hseq = data;
				wordToFind = "<Hsp_midline>";
			}
				
			else if(wordToFind == "<Hsp_midline>") {
				midline = data;
				wordToFind = "<Hit_id>";
			}
			
		}

	}
		
	
	return probes;
	


}


//Iterates through alignment string looking for mismatch locations and scoring them accordingly
int FileProcessor::calculateHybScore(int start, std::string midline) {
	int score = 0;
	int pos = start;
	
	for(int i = 0; i < midline.length(); i++) {
		if(midline[i] == '|') {
			score += hybHit(pos);
		}
		pos++;
	}

	return score;
}


//Helper function to return proper score based on hit location within probe
int FileProcessor::hybHit(int pos) {


	if((pos >= 1 && pos <= 5) || (pos >= 21 && pos <= 25)) {
		return 1;
	}
	
	if((pos >= 6 && pos <= 10) || (pos >= 16 && pos <= 20)) {
		return 2;
	}
	
	return 3;

}

	


