
var express = require("express"), http = require("http"), connect = require("connect"), formidable = require('formidable'),
    http = require('http'),
    util = require('util'),
    fs   = require('fs-extra'),
    mime = require('mime'),
    stream = require('stream'),
    net = require('net'),
    jquery = require('jquery'),
    exec = require('child_process').exec,
    sleep = require('sleep'),
    app;


// Set up routes for various programs/files
var blast = '/Users/marc_leef/Desktop/Work/ncbi-blast-2.2.29+/bin/'
var save = '/Users/marc_leef/Desktop/Work/data/Server/files/'
var probe_db = '/Users/marc_leef/Desktop/Work/data/Databases/'
var results = '/Users/marc_leef/Desktop/Work/data/Server/results/'
var scripts= '/Users/marc_leef/Desktop/Work/PSS/scripts/'
var queries = '/Users/marc_leef/Desktop/Work/data/Queries/'
var web ='/Users/marc_leef/Desktop/Work/PSS/web/'



var chosenDesign = ""
var sequenceText = ""
var jobQueue = {}



// Selected design database

// JSON variable to be passed to client based on program progression		
var status = {"step" : "(1/4) Recieving Data..."}

// Numerical design identifier, to be used for generating href links for probesets/transcript clusters later on
var design;

// Remove files once we've finished with them
var cleanup = function( files ) {

	files.forEach(function ( file ) {
		exec('rm ' + file)
	})

}

// Determine which database to use
var databaseSelector = function (num) {
	num = +num
	design = num
	switch(num) {
		case 1:
			chosenDesign = 'HuEx-1_0-st'
			return probe_db + 'HuEx-1_0-st'
			break
		case 2:
			chosenDesign = 'MouseEx-1_0-st'
			return probe_db + 'MouseEx-1_0-st'
			break
		case 3:
			chosenDesign = 'RatEx-2_0-st'
			return probe_db + 'RatEx-1_0-st'
			break
		case 4:
			chosenDesign = 'HuGene-1_0-st'
			return probe_db + 'HuGene-1_0-st'
			break
		case 5:
			chosenDesign = 'MouseGene-1_0-st'
			return probe_db + 'MouseGene-1_0-st'
			break
		case 6:
			chosenDesign = 'RatGene-1_0-st'
			return probe_db + 'RatGene-1_0-st'
			break

		case 712:
			chosenDesign = 'HuGene-2_0-st'
			return probe_db + 'HuGene-2_0-st'
			break
		case 715:
			chosenDesign = 'MouseGene-2_0-st'
			return probe_db + 'MouseGene-2_0-st'
			break
		case 718:
			chosenDesign = 'RatGene-2_0-st'
			return probe_db + 'RatGene-2_0-st'
			break
			
		case 725:
			chosenDesign = 'HTA-2_0'
			return probe_db + 'HTA-2_0'
			break
	}

}

// Core of the program execution
// BLAST query sequence -> Transform output into HTML and tab spaced -> Process the output and join HTML files -> Return to client -> Cleanup used files
var exonBlast = function(file_name, file1, html1, cur_db, clientID) {
	jobQueue[clientID].status = {"step" : "(2/4) Querying sequences against probe database..."}
	status = {"step" : "(2/4) Querying sequences against probe database..."}
	var exonBlast = exec(blast + 'blastn' + ' -query ' + save + file_name + ' -db ' + cur_db + ' -num_threads 4 -outfmt 11 -word_size 7 -gapopen 5 -gapextend 2 -reward 1 -penalty -1 -out ' + file1, function (error, stdout, stderr) {
			console.log(stdout)
			console.log(error)
			console.log(stderr)
			console.log("3")
			status = {"step" : "(3/4) Analyzing results..."}
			jobQueue[clientID].status = {"step" : "(3/4) Analyzing results..."}
			//sleep.sleep(2)
			exec(scripts + 'format_blast.sh ' + file1 + ' ' + html1 + ' ' + queries + file_name + '.tsv' + ' ' + design, function (error, stdout, stderr) {
				console.log(stdout)
				console.log(error)
				console.log(stderr)
				console.log("4")
				status = {"step" : "(4/4) Formatting output..."}
				jobQueue[clientID].status = {"step" : "(4/4) Formatting output..."}
				//sleep.sleep(1)
				exec('python ' + scripts + 'join_blasts.py ' + html1 + ' ' + web + 'temp.html > ' + web + 'output.html' , function (error, stdout, stderr) {
					console.log("5")
					console.log(stdout)
					console.log(error)
					console.log(stderr)
					fs.readFile('./output.html', function (err, html) {
			    		if (err) {
			        		throw err; 
			    		} 
			    		jobQueue[clientID].resp.end(html)
			    		jobQueue[clientID].details.time.end= new Date().getTime();
			    		jobQueue[clientID].details.time.elapsed = (jobQueue[clientID].details.time.end - jobQueue[clientID].details.time.start)/1000
			    		cleanup([queries + file_name + '.tsv', save + file_name, file1, html1, html1 + '.exon', html1 + '.gene', 'output.html'])
					})
				})
			})
		})
	}

// Create our Express-powered HTTP server // and have it listen on port 3000
app = express(); http.createServer(app).listen(3000);

// For caching purposes, disabled in developement
//var oneYear = 31557600000;
//app.use(express.static(__dirname, { maxAge: oneYear }));

app.use(express.static(__dirname));
// set up our routes
app.post("/upload", 
	function (req, res) { 
		var timestamp = Date.now() || +new Date()
		var form = new formidable.IncomingForm();
		var db = ""
		var clientID = ""
		var fileNames =[]

	    form.parse(req, function(err, fields, files) {
	    	clientID = fields.id
	    	db = databaseSelector(fields.q8_chipDesign)
	    	sequenceText = fields.pf
	    	jobQueue[clientID] = {"resp" : res, "status" : {"step" : "(1/4) Recieving Data..."}, "details" : {"files" : [], "design" : db.replace(probe_db, ""), "time" : {"start" : 0, "end" : 0, "elapsed" : 0}}}
	    	jobQueue[clientID].details.time.start = new Date().getTime();
	    	jobQueue[clientID].resp.writeHead(200, {'content-type': 'text/html'});
	    });
	 	
	    form.on('progress', function(bytesReceived, bytesExpected) {
	    		    

	        var percent_complete = (bytesReceived / bytesExpected) * 100;
	  		//jobQueue[clientID].status = {"step" : "(1/4) Receiving Data...", "percentage" : percent_complete} 
	    });
	 
	    form.on('error', function(err) {
	        console.error(err);
	    });
	    
	 
	    form.on('end', function(fields, files) {
	        /* New location of our uploaded file */
	        var outfile = "temp" + timestamp
	        var outpath = save + outfile
	        console.log("1")


	        if(sequenceText.length > 0) {
	        	exec("echo '" + sequenceText + "' >> " + outpath, function (error, stdout, stderr) {
	     			fileNames.push("None")
	        	})
	        }

	        else {
		        /* Concatenate uploaded files then remove them */
		        this.openedFiles.forEach( function (element) {
		        	fileNames.push(element.name)
		        	exec("cat " + element.path + " >> " + outpath, function (error, stdout, stderr) {
			        	cleanup([element.path])
			        })
		        })
		    }

	        sleep.sleep(1)
	        //console.log('python ' + scripts + 'line_breaks.py ' + outpath + ' > results' + timestamp)
	        exec('python ' + scripts + 'line_breaks.py ' + outpath + ' > ' + save + 'results' + timestamp, function (error, stdout, stderr) {
	        	console.log("2")
	        	cleanup([outpath])

		        outfile = "results" + timestamp
		        outpath = save + outfile
		    	

		    	file1 =  results + outfile + '_exon_level_results'
						
				html1 = file1 + '.html'

				jobQueue[clientID].details.files = fileNames


				/* Analyze uploaded files */
				exonBlast(outfile, file1, html1, db, clientID)
			})
			
		});

});

//Route for status updates
app.get("/status",
	function (req, res) {
		res.json(jobQueue[req.query.id].status)
});

//Route for sending details about the user search to the client
app.get("/details",
	function (req, res) {
		res.json(jobQueue[req.query.id].details)
		delete jobQueue[req.query.id]
});

		
