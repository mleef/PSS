# Helper script to add line breaks between newly concatenated FASTA file.

import sys

def addLineBreaks(file):
	f = open(file, 'r+').read()
	
	# For every sequence, insert a line break before it
	out = f.replace(">", "\n>")
	
	
	print(out)

def main(argv = sys.argv):
	f1 = argv[1]
	
	addLineBreaks(f1)
	
	
	



if __name__ == "__main__":
    try:
        main(sys.argv)
    except KeyboardInterrupt:
        pass
        