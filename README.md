---
title: Probe Set Search
description: Precision probe set coverage for custom sequences
author: Marc Leef
created:  2014 July 1
modified: 2014 July 21
---

# Probe Set Search
Probe Set Search is a web app for analyzing Affymetrix chip design specific coverage of custom sequences.

Features
--------

1. **Probe Set Coverage:**
	Identify design specific probe sets with the best coverage of the given sequence 
2. **High to Low Level Results:**
	View both exon and gene level probe coverage 
3. **Create Your Own Novel Probe Sets:**
	Create custom probe sets out of highest quality probes and download them in .spf format 
4. **View Probe Coverage Using Customizable Histograms:**
	Filter probes visually by setting cross hybridization score thresholds 

Screenshots
-----------

### Index Page
![alt text](https://github.com/mleef/PSS/raw/master/web/images/i.png "Search Page")

### Gene Level View
![alt text](https://github.com/mleef/PSS/raw/master/web/images/gv.png "Gene Level View")

### Adding to Custom Probeset
![alt text](https://github.com/mleef/PSS/raw/master/web/images/nps.png "Add to custom probe set")

### Histogram Coverage Viewer
![alt text](https://github.com/mleef/PSS/raw/master/web/images/cv.png "Histogram Coverage Viewer")


Dependencies
------------
1. Node.js
	* Connect
	* Express
	* Fileupload
	* Formidable
	* Fs-extra
	* Jquery
	* Mime
	* Sleep
2. BLAST
3. Python
4. g++


Contact
-------
### Marc_Leef@affymetrix.com