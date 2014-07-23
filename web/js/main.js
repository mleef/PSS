
$(document).ready(function(){
	//sleep(500)
	//window.scrollReveal = new scrollReveal(config);
	
	$("td[colspan=8]").find("table").hide();

	getQueryDetails()

	var config = {
	  enter: 'top',
	  move: '50px',
	  over: '.2s',
	  easing: 'ease-in-out',
	  viewportFactor: 1,
	  reset: true,
	  init: true
	};
	
	$("#tabs li").click(function() {
	    //  First remove class "active" from currently active tab
	    $("#tabs li").removeClass('active');

	    //  Now add class "active" to the selected/clicked tab
	    $(this).addClass("active");

	    //  Hide all tab content
	    $(".tab_content").hide();

	    //  Here we get the href value of the selected tab
	    var selected_tab = $(this).find("a").attr("href");

	    //  Show the selected tab content
	    $(selected_tab).fadeIn("slow");

	    //  At the end, we add return false so that the click on the link is not executed
	    return false;
	});

	$(".subtable a").click(function() {
		$("#tabs li").removeClass('active');
		$("#tab3").addClass("active");
		$(".tab_content").hide();
		var href = $(this).find("a").attr("href");
		$("#tab3").fadeIn("slow");
		$(document).scrollTop( href.offset().top );  
		return false;
	})


	 
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

	$("button.back").on("click", function (event) {
    	window.location = "http://localhost:3000/"
  	})

  	$("button.downnps").on("click", function (event) {
    	var result = "#spf-format=1\n"
    	var selected = []
		$(".checkboxes").each(function() {
			if($(this).is(':checked')) {
				if($(this).attr("name") != 'probeset') {
		    		selected.push($(this).attr('value'))
		    	}
			}
		});

		var uniqueSelected = selected.filter(function(elem, pos) {
    		return selected.indexOf(elem) == pos;
		})

		uniqueSelected.forEach( function (element) {
			result += element + "\n"
		})

		download('novel_probe_set.spf', result);
  	})


  	$("button.downres").on("click", function (event) {
  		var result = ""
  		var count = 0
  		var upperBound = 0
  		var miniCount = 0;
  		var numProbes = 0
  		$("td").each(function() {
			//console.log("************")
  			//console.log($(this).text())
			//result += "Count: " + count + "UpperBound: " + upperBound + "MiniCount: " + miniCount + "NumProbes: " + numProbes + "\n"
			if(count < 5) {
				if(count == 2) {
					numProbes = parseInt($(this).text().slice(0, $(this).text().indexOf("/")))
					upperBound = count + numProbes + 4
				}
				result += $(this).text() + "\t"
				count += 1
			}
			else if(count == 5) {
				count += 1
				result += "\n"
			}

			else if(count > 4 && count < upperBound) {
				result += "\t" + $(this).text() + "\t"
				miniCount += 1
				if(miniCount == 8) {
					result += "\n"
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
				result += $(this).text() + "\t"
				count += 1

			}
  			//console.log("************")

/*	    	$(this).find(".subtable td").each(function () {
	    		result += $(this).text() + "\t"
	    		count += 1
	    		if(count == 8) {
	    			result += "\n"
	    			count = 0
	    		}
	    	})
	    	count = 0
	    	result += "\n"*/
	    })

  		//console.log(result)
	   download('results.tsv', result);
  	})


  	$(".checkboxes").on("click", function (event) {
  		var target = $(event.target)
  		if(target.attr("name") == 'probeset') {
  			var nextRow = target.parent().parent().next()
	 		var boxes = $(nextRow).find(':nth-child(8)').children(".checkboxes")

	 		$(boxes).each(function() {this.checked = !this.checked})
  		}
  	})



});


function getQueryDetails() {

	$.get("/details", function (data) {
		var files = ""
		data["files"].forEach( function (file) {
			files += file + ","
		})
		var seqs = $("caption").length
		$("#geneinfo").text("Design=" + data["design"] + " " + "Files=" + files.slice(0,-1) + " " + "Sequences=" + seqs/2)
		$("#exoninfo").text("Design=" + data["design"] + " " + "Files=" + files.slice(0,-1) + " " + "Sequences=" + seqs/2)



		$("html").toggle()

	});
}

function download(filename, text) {
    var pom = document.createElement('a');
    pom.setAttribute('href', 'data:text/plain;charset=utf-8,' + encodeURIComponent(text));
    pom.setAttribute('download', filename);
    pom.click();
}
