
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

  	$("button.download").on("click", function (event) {
    	var selected = ""
		$(".checkboxes").each(function() {
			if($(this).is(':checked')) {
				if($(this).attr("name") != 'probeset') {
		    		selected += ($(this).attr('value')) + "\n";
		    	}
			}
		});

		download('novel_probe_set.spf', selected);
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
