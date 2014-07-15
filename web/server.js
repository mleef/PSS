
var express = require("express"), http = require("http"), connect = require("connect"),formidable = require('formidable'),
    http = require('http'),
    util = require('util'),
    fs   = require('fs-extra'),
    mime = require('mime'),
    stream = require('stream'),
    net = require('net'),
    jquery = require('jquery'),
    exec = require('child_process').exec,
    app;



var blast = '/Users/marc_leef/Desktop/Work/ncbi-blast-2.2.29+/bin/'
var save = '/Users/marc_leef/Desktop/Work/data/Server/files/'
var probe_db = '/Users/marc_leef/Desktop/Work/data/Databases/'
var results = '/Users/marc_leef/Desktop/Work/data/Server/results/'
var scripts= '/Users/marc_leef/Desktop/Work/PSS/scripts/'
var queries = '/Users/marc_leef/Desktop/Work/data/Queries/'
var web ='/Users/marc_leef/Desktop/Work/PSS/web/'

cur_db = ""

var m1, m2 = false
		
var status;
var design;

var cleanup = function( files ) {

	files.forEach(function ( file ) {
		exec('rm ' + file)
	})

}

var databaseSelector = function (num) {
	num = +num
	design = num
	switch(num) {
		case 4:
			cur_db = probe_db + 'HuGene-1_0-st-exon-level'
		case 712:
			cur_db = probe_db + 'HuGene-2_0-st-exon-level'
	}

}


var exonBlast = function(file_name, file1, html1, res) {
	status = {"step" : "(2/4) Querying sequences against probe database..."}

	var exonBlast = exec(blast + 'blastn' + ' -query ' + save + file_name + ' -db ' + cur_db + ' -num_threads 4 -outfmt 11 -task blastn-short -out ' + file1, function (error, stdout, stderr) {
			console.log(stderr)
			status = {"step" : "(3/4) Analyzing results..."}
			exec(scripts + 'format_blast.sh ' + file1 + ' ' + html1 + ' ' + queries + file_name + '.tsv', function (error, stdout, stderr) {
				console.log(stderr)
				status = {"step" : "(4/4) Formatting output..."}
				exec('python ' + scripts + 'join_blasts.py ' + html1 + ' ' + web + 'temp.html > ' + web + 'output.html' , function (error, stdout, stderr) {
					console.log(stderr)
					fs.readFile('./output.html', function (err, html) {
		    		if (err) {
		        		throw err; 
		    		} 
		    		res.end(html)
		    		cleanup([queries + file_name + '.tsv', save + file_name, file1, html1, html1 + '.exon', html1 + '.gene', "output.html"])
		    		cur_db = blast
					})
				})
			})
		})
	}

// Create our Express-powered HTTP server // and have it listen on port 3000
app = express(); http.createServer(app).listen(3000);

app.use(express.static(__dirname));

// set up our routes
app.post("/upload", 
	function (req, res) { 
		var timestamp = Date.now() || +new Date()
		var form = new formidable.IncomingForm();
		
	    form.parse(req, function(err, fields, files) {
	    	databaseSelector(fields.q8_chipDesign)
	    	res.writeHead(200, {'content-type': 'text/html'});
	    });
	 	
	    form.on('progress', function(bytesReceived, bytesExpected) {
	        var percent_complete = (bytesReceived / bytesExpected) * 100;
	  		status = {"step" : "(1/4) Receiving Files", "percentage" : percent_complete} 
	    });
	 
	    form.on('error', function(err) {
	        console.error(err);
	    });
	    
	 
	    form.on('end', function(fields, files) {
	        /* New location of our uploaded file */
	        var outfile = "temp" + timestamp
	        var outpath = save + outfile

	        /* Concatenate uploaded files then remove them */
	        this.openedFiles.forEach( function (element) {
	        	//console.log(element.path)
	        	//console.log("echo | cat " + element.path + " >> " + outpath)
	        	exec("cat " + element.path + " >> " + outpath, function (error, stdout, stderr) {
		        	cleanup([element.path])
		        })
	        })
	        //console.log('python ' + scripts + 'line_breaks.py ' + outpath + ' > results' + timestamp)
	        exec('python ' + scripts + 'line_breaks.py ' + outpath + ' > ' + save + 'results' + timestamp, function (error, stdout, stderr) {

	        	cleanup([outpath])

		        outfile = "results" + timestamp
		        outpath = save + outfile
		    	

		    	file1 =  results + outfile + '_exon_level_results'
						
				html1 = file1 + '.html'

				/* Analyze uploaded files */
				exonBlast(outfile, file1, html1, res)
			})


    	});
});


app.get("/status",
	function (req, res) {
		res.json(status)
});

		
