//TODO: big tables still leak out when page loads
	$("td[colspan=4]").find("table").hide();
	      var config = {
              enter: 'top',
              move: '50px',
              over: '.2s',
              easing: 'ease-in-out',
              viewportFactor: 1,
              reset: true,
              init: true
            };
	window.scrollReveal = new scrollReveal(config);
	$(document).ready(function(){
		//sleep(500)
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
    	})

		function sleep(milliseconds) {
		  var start = new Date().getTime();
		  for (var i = 0; i < 1e7; i++) {
		    if ((new Date().getTime() - start) > milliseconds){
		      break;
		    }
		  }
		}
		 
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

	});