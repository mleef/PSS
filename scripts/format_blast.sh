# Transforms BLAST index into HTML/tab spaced format. Tab format is then processed by PSS into HTML.


# Set up paths
scripts='/Users/marc_leef/Desktop/Work/PSS/scripts'
blastformat='/Users/marc_leef/Desktop/Work/ncbi-blast-2.2.29+/bin/blast_formatter'
web='/Users/marc_leef/Desktop/Work/PSS/web'
gene_html='/Users/marc_leef/Desktop/Work/PSS/bin/gene_html'
exon_html='/Users/marc_leef/Desktop/Work/PSS/bin/exon_html'
results='/Users/marc_leef/Desktop/Work/data/Server/results'


file1=$1
html1=$2
out=$3
pk=$4

# Convert archive to HTML format
$blastformat -archive $file1 -html -out $html1

# Convert archive to tab spaced file with comments
$blastformat -archive $file1 -outfmt 7 -out $out

# Convert archive to god knows what
$blastformat -archive $file1 -outfmt 1 -out $results/out

# Perform exon level analysis of BLAST results
$exon_html $out $pk > $html1'.exon'

# Perform gene level analysis of BLAST results
$gene_html $out $pk > $html1'.gene'