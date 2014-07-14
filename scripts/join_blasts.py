# Helper script to attach newly made HTML files to skeleton HTML file.

#!/usr/bin/env python
import sys



def insert_string(s1, s2, index):
    return s1[:index] + s2 + s1[index:]


def addFiles(f1,f2):

  	# open input file for reading, as well as .exon and .gene files produced by PSS to the same directory 	
	in1 = open(f1, 'r').read()
	in2 = open(f1 + ".exon", 'r').read()
	in3 = open(f1 + ".gene", 'r').read()	
   	out = open(f2, 'r+').read()
   	count = 0
   	
   	# find first tag to append to
   	loc1 = out.find('f3">') + 5
   	
   	s1 = ""
   	s2 = ""
   	s3 = ""
   	
   	# append raw BLAST results to third tab
   	s1 = insert_string(out, in1, loc1)
   	
   	# find next tag to append to
   	loc2 = s1.find('f1">') + 5
   	
   	# append exon level analysis HTML to temp
   	s2 = insert_string(s1, in2, loc2)
   	
   	loc3 = s2.find('f2">') + 5
   	
   	# append gene level analysis HTML to temp
   	s3 = insert_string(s2, in3, loc3)
   	
   	# print newly formatted HTML file
   	print(s3)
    
   		
   		
   	
# Argument one will be the raw BLAST results, argument two will be the skeleton HTML to edit.
def main(argv = sys.argv):
	f1 = argv[1]
	f2 = argv[2]
	
	addFiles(f1,f2)
	
	
	



if __name__ == "__main__":
    try:
        main(sys.argv)
    except KeyboardInterrupt:
        pass
        
        