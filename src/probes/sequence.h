/* 
   Header file for the sequence class. This is a vestige from an early build of the program, acts as 
   a less sophisticated version of the probe object.
*/

#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <string>
#include <vector>



class Sequence
{


	private:
		std::string name_;
		std::string description_;
		std::string seq_;
		std::vector<std::string> kmers;
	
	
	
	
	public:
		Sequence(std::string n, std::string d, std::string s);
		void kmerize();
		void complement();
		void reverse();
		std::string getName();
		std::string getSeq();
		std::string getDescription();





};

#endif