/* 
   Implementation file of the sequence class. Contains basic data similar to probe object, but also possesses
   some possibly helpful string manipulation functions.
*/

#include "sequence.h"


// Constructor, sequence name, sequence description, and the raw sequence, taken directly from FASTA formatted files.
Sequence::Sequence(std::string n, std::string d, std::string s) {
	name_ = n;
	description_ = d;
	seq_ = s;
	//complement();
	//kmerize();
	//reverse();
}

// Getter, returns sequence name
std::string Sequence::getName() {
	return name_;
}

// Getter, returns raw sequence
std::string Sequence::getSeq() {
	return seq_;
}

// Getter, returns sequence description
std::string Sequence::getDescription() {
	return description_;
}

// Breaks the sequence into 25mers and stores each one in the member variable vector 'kmers'
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

// Changes the contained sequence to its complement
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

// Reverses the sequence (sense -> anti-sense or vice versa)
void Sequence::reverse() {

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
	



