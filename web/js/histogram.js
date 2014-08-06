
  var hyb_score = 38;
  var bin_size = 20;
  var start = 0;
  var stop = 0;
  var show_bin;
  var $ids = []
  var ids = []

  var binValues = [1, 3, 5, 10, 20, 40, 80, 200]
  var binCounter = 4

  //Gather list of table names to draw data from
  $("#tab2 #titles").each(function() { $ids.push($(this).text()) });
  $("#tab2 caption").each(function() { ids.push($(this).text()) });
  $("#s1").slider()
  $("#s2").slider()

  //Iterate through data identifers and generate data sets from each one using initial parametrs, then draw the histogram
  for (i = 0; i < $ids.length; i++) { 
    var values = [];
     for(m = hyb_score; m < 46; m++) {
          $("#tab2 #" + $ids[i] + " #s" + m).each(function() { 
            values.push(parseInt($(this).text())) 
      })}
    var text = $("#tab4 pre:eq(" + i + ")").text()
    var pos = text.search("Length")
    var len = text.slice(pos + 7, pos + 20).replace(/\D/g,'');
    makeGraph(values,len, ids[i], bin_size, 0, 0)
  }


//Graph drawing function, values is the data set, length is the x upper bound, query_name is the sequence the data is derived from, bin_size and start1/stop1 are graphical options
function makeGraph(values, length, query_name, bin_size, start1, stop1) {
   /* console.log("Domain: " + start1 + "," + stop1)
    console.log(values)
    console.log(values.length + " total probes")
    console.log("Start:" + start1)
    console.log("Stop: " + stop1)
*/
    var formatCount = d3.format(",.0f");

    //set margins
    var margin = {top: 40, right: 30, bottom: 40, left: 40},
        width = 960 - margin.left - margin.right,
        height = 500 - margin.top - margin.bottom;

    //Round off length to nearest 1000000,100000, etc..
    if(length > 100000) {
      length = Math.ceil(length/100000)*100000
    }

    else if(length > 10000) {
      length = Math.ceil(length/10000)*10000
    }
    else if(length > 1000) {
      length = Math.ceil(length/1000)*1000
    }
    else if(length > 100) {
      length = Math.ceil(length/100)*100
    }

    var st, stp = 0

    //Figure out largest domain of all data to set as starting window
    if(start1 == 0 && stop1 == 0) {
      st = 0
      stp = length
    }
    else {
      st = start1
      stp = stop1
    }

    if(stop < stp) {
      stop = stp
    }

    if(start > st) {
      start = st
    }

    //Update text boxes
    $(".start").val(st)
    $(".stop").val(stp)

    var x = d3.scale.linear()
        .domain([st, stp])
        .range([0, width]);

    // Generate a histogram using uniformly-spaced bins.
    var data = d3.layout.histogram()
        .bins(x.ticks(bin_size))
        (values);

    //Calculate width of rectangles for bar representation
    barwidth = width/data.length - 2


    var y = d3.scale.linear()
        .domain([0, d3.max(data, function(d) {show_bin = d.dx; return d.y; })])
        .range([height, 0]);

    var xAxis = d3.svg.axis()
        .scale(x)
        .orient("bottom")
        .tickFormat(d3.format("d"));


    var yAxis = d3.svg.axis()
        .scale(y)
        .orient("left")
        .tickFormat(d3.format("d"));

    var tip = d3.tip()
        .attr('class', 'd3-tip')
        .offset([-10, 0])
        .html(function(d) {
          return "<strong>Frequency:</strong> <span style='color:red'>" + d.y + " probes (" + d.x + " - " + (d.x+d.dx) + ")</span>";
      })

    //Update bin size text box
    $("#b2").text("Bin Size: " + show_bin)



    //Main graph object
    var svg = d3.select("#tab5").append("svg")
        .attr("width", width + margin.left + margin.right + 30)
        .attr("height", height + margin.top + margin.bottom + 30)
      .append("g")
        .attr("transform", "translate(" + (margin.left+30) + "," + margin.top + ")");
    svgVar = svg;
    svg.call(tip);

    var bar = svg.selectAll(".bar")
        .data(data)
      .enter().append("g")
        .attr("class", "bar")
        .attr("transform", function(d) { return "translate(" + x(d.x) + "," + y(d.y) + ")"; })
        .on('mouseover', tip.show)
        .on('mouseout', tip.hide)
        .on('click', function(d){ redraw(d.x, d.x + d.dx);});

    //Create bars
    bar.append("rect")
        .attr("x", 1)
        .attr("width", barwidth)
        .attr("height", function(d) {return height - y(d.y); });

    //Create x axis
    svg.append("g")
        .attr("class", "x axis")
        .attr("transform", "translate(0," + height + ")")
        .call(xAxis);

    //Create y axis
    svg.append("g")
        .attr("class", "y axis")
        .call(yAxis);

    //Label graph title
    svg.append("text")
        .attr("x", (width / 2))             
        .attr("y", 0 - (margin.top / 2))
        .attr("text-anchor", "middle")  
        .style("font-size", "24px") 
        .style("text-decoration", "underline")  
        .text(query_name + " Probe Distribution");

    //Label graph x axis
    svg.append("text")      // text label for the x axis
        .attr("transform", "translate(" + (width / 2) + " ," + (height + margin.bottom + 15) + ")")
        .style("text-anchor", "middle")
        .style("font-size", "20px") 
        .text("Position");

   //Label graph y axis
   svg.append("text")
        .attr("transform", "rotate(-90)")
        .attr("y", 0 - margin.left - 15)
        .attr("x",0 - (height / 2))
        .attr("dy", "1em")
        .style("font-size", "20px") 
        .style("text-anchor", "middle")
        .text("# Probes");

    $("#tab4").append("<hr>")
  }

// Listener for the hyb score slider
    d3.select("#range1")
    .select("input")
    .on("change", function () {
      var bin = this.value;
      hyb_score = bin
      redraw(start,stop)
      $("#b1").text("Minimum Hybridization Score: " + hyb_score)
    });

// Listener for the bin size slider
    // d3.select("#range2")
    // .select("input")
    // .on("change", function () {

    //   var bin = this.value;
    //   bin_size = (-2*bin) + 201
    //   console.log(bin_size)
       
    //   redraw(start,stop)
    // });

//Bin values that change data 
    $("#bbp").on("click", function (event) {
      if(binCounter > 1) {
        binCounter -= 1
        bin_size = binValues[binCounter]
        redraw(start,stop)
      }

    })
    $("#bbm").on("click", function (event) {
      if(binCounter < binValues.length - 1) {
        binCounter += 1
        bin_size = binValues[binCounter]
        redraw(start,stop)
      }
    })



// Function to redraw the graphs with a new domain
    function redraw(st, stp) {
      $("#tab5 hr").remove()
      $("svg").remove();
      $(".d3-tip").remove();

      for (i = 0; i < $ids.length; i++) { 
        var values = [];
        for(m = hyb_score; m < 46; m++) {
          $("#tab2 #" + $ids[i] + " #s" + m).each(function() { 
            var val = parseInt($(this).text())
            if(val >= st && val <= stp) {
              values.push(val) 
            };
        })}

        var text = $("#tab4 pre:eq(" + i + ")").text()
        var pos = text.search("Length")
        var len = text.slice(pos + 7, pos + 20).replace(/\D/g,'');
        start = st;
        stop = stp;

        makeGraph(values,len, ids[i], bin_size, start, stop)




    }
  }

// Listener for the domain update button
  $("button.update").on("click", function (event) {
    var st = $(".start").val()
    var stp = $(".stop").val()
    redraw(st,stp)


  })

// Resets the domain to original values
  $("button.reset").on("click", function (event) {
        $("svg").remove();
        $(".d3-tip").remove();
        $("#tab5 hr").remove()
    for (i = 0; i < $ids.length; i++) { 
      var values = [];
      for(m = hyb_score; m < 46; m++) {
          $("#tab2 #" + $ids[i] + " #s" + m).each(function() { 
            values.push(parseInt($(this).text())) 
      })}
      var text = $("#tab4 pre:eq(" + i + ")").text()
      var pos = text.search("Length")
      var len = text.slice(pos + 7, pos + 20).replace(/\D/g,'');
      start = 0
      stop = 0
      makeGraph(values,len, ids[i], bin_size, 0, 0)
  }


  })


  $("button.default").on("click", function (event) {
    


  })












