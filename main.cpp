#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include "sequence.h"



std::vector<Sequence> processFile(const char * f) {

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
	return sequences;


}



int main (int argc, char *argv[]) {

	std::vector<Sequence> sequences = processFile(argv[1]);
    
	

}


