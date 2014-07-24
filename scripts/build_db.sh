# Script that builds the HuGene 1.0/2.0 BLAST DB 


# Set up paths
makeblastdb='/Users/marc_leef/Desktop/Work/ncbi-blast-2.2.29+/bin/makeblastdb'
build_fasta="/Users/marc_leef/Desktop/Work/PSS/bin/build_fasta"
designs='/Users/marc_leef/Desktop/Work/data/Designs'
databases='/Users/marc_leef/Desktop/Work/data/Databases'

array='HTA-2_0'
pgf=$array'.pgf'
mps=$array'.mps'
pss=$array'.pss'
fasta=$array'.fa'

fpath=$designs/$array/$fasta

echo Building new FASTA...
# Build new fasta with unique IDs
$build_fasta $designs/$array/$pgf $designs/$array/$mps > $fpath

echo Building exon level database...
# Build original exon database from newly made FASTA ##
$makeblastdb -in $fpath -input_type fasta -dbtype nucl -out $databases/$array



