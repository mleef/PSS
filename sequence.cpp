#include "sequence.h"


Sequence::Sequence(std::string n, std::string d, std::string s) {
	name_ = n;
	description_ = d;
	seq_ = s;
	//complement();
	kmerize();
}

std::string Sequence::getName() {
	return name_;
}

std::string Sequence::getSeq() {
	return seq_;
}

std::string Sequence::getDescription() {
	return description_;
}

void Sequence::kmerize() {
	int beg = 0;
	int end = 25;
	
	while(end < seq_.length()) {
		std::string kmer = seq_.substr(beg,end);
		kmers.push_back(kmer);
		beg++;
		end++;
	}

}

void Sequence::complement() {
	std::string newSeq;
	
	for(int i = 0; i < seq_.length(); i++) {
		char base = seq_.at(i);
		switch(base) {
			case 'A':
				newSeq += 'T';
				break;
			case 'T':
				newSeq += 'A';
				break;
			case 'G':
				newSeq += 'C';
				break;
			case 'C':
				newSeq += 'G';
				break;
		}	
	}
	
	seq_ = newSeq;		
		
}
	



