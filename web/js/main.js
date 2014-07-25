
var design = ""
var curTab = "tab1"
var genePrevVal = 38

$(document).ready(function(){
	//sleep(500)
	//window.scrollReveal = new scrollReveal(config);

	//Hide subtables 
	$("td[colspan=8]").find("table").hide();

	//Populate header with details from server
	getQueryDetails()

	//Populate summary page
	summarize()

	//Config object for scroll reveal, optional
	var config = {
	  enter: 'top',
	  move: '50px',
	  over: '.2s',
	  easing: 'ease-in-out',
	  viewportFactor: 1,
	  reset: true,
	  init: true
	};
	
	//Tab view controller function
	$("#tabs li").click(function() {
	    //  First remove class "active" from currently active tab
	    $("#tabs li").removeClass('active');

	    //  Now add class "active" to the selected/clicked tab
	    $(this).addClass("active");

	    //  Hide all tab content
	    $(".tab_content").hide();

	    //  Here we get the href value of the selected tab
	    var selected_tab = $(this).find("a").attr("href");

	    curTab = $(selected_tab).attr("id")
	    if(curTab == "tab2") {
	    	$("#downnpsbutton").prop("disabled", false)
	    	$("#downresbutton").prop("disabled", false)
	    	$("#downnpsbutton").text("Download Exon Level Novel Probe Grouping")
	    	$("#downresbutton").text("Download Exon Level Results")
	    }

	    else if(curTab == "tab1") {
	    	$("#downnpsbutton").prop("disabled", false)
	    	$("#downresbutton").prop("disabled", false)
	    	$("#downnpsbutton").text("Download Gene Level Novel Probe Grouping")
	    	$("#downresbutton").text("Download Gene Level Results")

	    }
	    else {
	    	$("#downnpsbutton").prop("disabled", true)
	    	$("#downresbutton").prop("disabled", true)
	    }
	    //  Show the selected tab content
	    $(selected_tab).fadeIn("slow");

	    //  At the end, we add return false so that the click on the link is not executed
	    return false;
	});

	//Helper function to link href probe ids to appropriate BLAST tab and the appropriate spot on the page
	$(".subtable a").click(function() {
		$("#tabs li").removeClass('active');
		$("#tab3").addClass("active");
		$(".tab_content").hide();
		var href = $(this).find("a").attr("href");
		$("#tab3").fadeIn("slow");
		$(document).scrollTop( href.offset().top );  
		return false;
	})


	//Helper function for showing/hiding subtables upon clicking rows
	$(".maintable").click(function(event) {
	    event.stopPropagation();
	    var $target = $(event.target);
	    var id = event.target.id
	    if(id != 'nc' && id != 'nopad') {
	    	$target.show()
	        if ( $target.closest(".maintable td").attr("colspan") > 1 ) {
	            $target.slideUp(5);
	            $target.closest("tr").prev().find("td:first").html("+");
	        } else {
	            $target.closest("tr").next().find("table").slideToggle(5);
	            if ($target.closest("tr").find("td:first").html() == "+")
	                $target.closest("tr").find("td:first").html("-");
	            else
	                $target.closest("tr").find("td:first").html("+");                
	        } 
	    }                   
	});

	//Back button listener to search again
	$("button.back").on("click", function (event) {
    	window.location = "http://localhost:3000/"
  	})

	//Download novel probe set listener, aggregates all checked probes and generates a .spf file
  	$("button.downnps").on("click", function (event) {
  		var probesets = {}
    	var result = "#spf-format=1\n"

    	if(curTab == "tab1") {
    		result += "#%chip_type=" + design + "\n#%level=gene\n"
    	}

    	else {
    		result += "#%chip_type=" + design + "\n#%level=exon\n"
    	}

    	var selected = []
		$("#" + curTab + " .checkboxes").each(function() {
			if($(this).is(':checked')) {
				if($(this).attr("name") != 'probeset') {
					var ps = $(this).attr('class').split(' ')[1]
					var p = $(this).attr('value')
					if(!probesets[ps]) {
						probesets[ps] = [p]
					}

					else {
						probesets[ps].push(p)
					}
		    	}
			}
		});

		var numPS = 0
		for(var key in probesets) {
			numPS += 1
		}

		result += "#%num-probesets=" + numPS + "\n"
		result += "name\ttype\tnum_blocks\tblock_sizes\tblock_annotations\tnum_match\tnum_probes\tprobes\n"

  		for (var key in probesets) {
    		var list = probesets[key];
    		pcount = list.length 
    		result += key + "\t1\t1\t" + pcount + "\t0\t1\t" + pcount + "\t"
    		list.forEach( function (element) {
    			result += element + ","
    		})
    		result = result.substring(0, result.length - 1)
    		result += "\n"
    	}

		if(curTab == "tab1") {
			download('novel_probe_grouping_exon_level.spf', result);
		}
		else {
			download('novel_probe_grouping_gene_level.spf', result);
		}
		


  	})


  	//Download results listener, generates a .tsv representation of the tables
  	$("button.downres").on("click", function (event) {
  		var result
  		var count = 0
  		var upperBound = 0
  		var miniCount = 0;
  		var numProbes = 0

  		if(curTab == "tab1") {
  			result = "#%chip_type=" + design + "\n#%level=gene\n#%header1=sequence name\n#%header2=\tprobeset_id\tunique hits/hits in probeset\thit percentage\n#%header3=\t\tprobe_id\tpercent identity\tstart\tstop\tevalue\tbit score\thybridization score\n"
  		}

  		else {
  			result = "#%chip_type=" + design + "\n#%level=exon\n#%header1=sequence name\n#%header2=\ttranscriptcluster_id\tunique hits/hits in transcriptcluster\thit percentage\n#%header3=\t\tprobe_id\tpercent identity\tstart\tstop\tevalue\tbit score\thybridization score\n"
  		}

  		$("#" + curTab + " .maintable").each(function () {
  			var caption  = $(this).find("caption").text()
  			var first = true
	  		$(this).find("td").each(function() {
	  			if(first) {
	  				result += caption + "\n"
					result += "\t"		
					first = false
	  			}

				if(count < 5) {
					if(count == 2) {
						upperBound = $(this).parent().next().find("tr").length + 5
					}
					if($(this).text() != "+" && $(this).text() != "-") {
						result += $(this).text() + "\t"
					}//
					count += 1
				}
				else if(count == 5) {
					count += 1
					result += "\n"
					result += "\t"
				}

				else if(count > 4 && count < upperBound) {
					result += "\t" + $(this).text() + "\t"
					miniCount += 1
					if(miniCount == 8) {
						result += "\n"
						result += "\t"
						miniCount = 0
						count += 1
					}
				}

				else if(count == upperBound) {
					count = 0
					miniCount = 0
					numProbes = 0
					upperBound = 0
					result += "\n"
					first = true
					count += 1

				}
		    })

			count = 0
			miniCount = 0
			numProbes = 0
			upperBound = 0
			result = result.slice(0, result.length - 30)

		})

		if(curTab == "tab1") {
			download('gene_level_results.tsv', result);
		}
		else {
			download('exon_level_results.tsv', result);
		}
  	})



	//Listens for checkboxes and propogates selection to subtable
  	$(".checkboxes").on("click", function (event) {
  		var target = $(event.target)
  		if(target.attr("name") == 'probeset') {
  			var nextRow = target.parent().parent().next()
	 		var boxes = $(nextRow).find(':nth-child(8)').children(".checkboxes")

	 		$(boxes).each(function() {this.checked = !this.checked})
  		}
  	})

  	$("#generange").on('change', function (event) {
  		var val = this.value
  		$("#nopad tr").each(function () {
  			var parentRow = $(this).parents().eq(4).prev()
  			var firstNum, secondNum, secondHalf, newPerc, ratio
  			if(parseInt($(this).attr("class")) < val) {
				$(this).hide()
				ratio = $(parentRow).find("td").eq(2).text()
				firstNum = parseInt(ratio.substring(0, ratio.indexOf("/")))
				secondHalf = ratio.substring(ratio.indexOf("/"))
				secondNum= ratio.substring(ratio.indexOf("/") + 1)
				firstNum -= 1
				$(parentRow).find("td").eq(2).text(firstNum + secondHalf)
				
  		
  			}
  			else {
  				if(parseInt($(this).attr("class")) < genePrevVal) {
  					$(this).show()
					ratio = $(parentRow).find("td").eq(2).text()
	  				firstNum = parseInt(ratio.substring(0, ratio.indexOf("/")))
	  				secondHalf = ratio.substring(ratio.indexOf("/"))
	  				firstNum += 1
	  				$(parentRow).find("td").eq(2).text(firstNum + secondHalf)

  				}
  			}

  			firstNum = 0
  			secondNum = 0
  			ratio = 0
  			secondHalf = 0
  			newPerc = 0
  		})

  		genePrevVal = val
  		//console.log("gene val: " + this.value)
  	})

  	$("#exonrange").on('change', function (event) {
  		var val = this.value
  		$("#nopad tr").each(function () {
  			if(parseInt($(this).attr("class")) < val) {
  				$(this).hide()
  			}
  			else {
  				$(this).show()
  			}
  		})
  		//console.log("exon val: " + this.value)
  	})



});


//Helper function for querying server for information about user's search parameters
function getQueryDetails() {

	$.get("/details", function (data) {
		var files = ""
		data["files"].forEach( function (file) {
			files += file + ","
		})
		var seqs = $("caption").length
		design = data["design"]
		$("#geneinfo").text("Design=" + data["design"] + " " + "Files=" + files.slice(0,-1) + " " + "Sequences=" + seqs/2)
		$("#exoninfo").text("Design=" + data["design"] + " " + "Files=" + files.slice(0,-1) + " " + "Sequences=" + seqs/2)



		$("html").toggle()

	});
}


function summarize() {
	var summary = {"id" : "", "topg" : "", "tope" : "", "nump" : "", "numps" : "", "numtc" : ""}
	var count = 0;
	$("#tab1 #summary").each( function () {

		var id = "sum" + count
		summary.id = $(this).attr("title")
		summary.topg = $(this).find("#top").text()
		summary.nump = $(this).find("#nump").text()
		summary.numtc = $(this).find("#numps").text()

		$("#tab2 #summary").each( function() {
			if($(this).attr("title") == summary.id) {
				summary.tope = $(this).find("#top").text()
				summary.numps = $(this).find("#numps").text()
			}
		})

		$("#tab5").append("<div id='" + id + "' </div>")
		$("#tab5 #" + id).append("<h3>Sequence: " + summary.id + "</h3>")
		$("#tab5 #" + id).append("<p>Top Transcript Cluster ID: <span id ='good'>" + summary.topg + "</span</p>")
		$("#tab5 #" + id).append("<p>Top Probe Set ID: <span id ='good'>" + summary.tope + "</span></p>")
		$("#tab5 #" + id).append("<p>Total Transcript Clusters Registering Hits: <span id ='good'>" + summary.numtc + "</span></p>")
		$("#tab5 #" + id).append("<p>Total Probe Sets Registering Hits: <span id ='good'>" + summary.numps + "</span></p>")
		$("#tab5 #" + id).append("<p>Total Probe Hits: <span id ='good'>"  + summary.nump + "</span></p>")
		$("#tab5").append("<hr>")

		count += 1;
		summary = {"id" : "", "topg" : "", "tope" : "", "nump" : "", "numps" : "", "numtc" : ""}
	})

}


//Helper function to create a file for the user to download out of a javascript string
function download(filename, text) {
    var pom = document.createElement('a');
    pom.setAttribute('href', 'data:text/plain;charset=utf-8,' + encodeURIComponent(text));
    pom.setAttribute('download', filename);

    //required for ff, ie
    document.body.appendChild(pom);
    pom.target="_self" ;

    pom.click();
}
