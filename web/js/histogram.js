
  var hyb_score = 45;
  var bin_size = 10;
  var start = 0;
  var stop = 0;
  var show_bin;
  var $ids = []
  var ids = []
  $("#tab1 #titles").each(function() { $ids.push($(this).text()) });
  $("#tab1 caption").each(function() { ids.push($(this).text()) });
  $("#s1").slider()
  $("#s2").slider()

  //console.log(queryIDs)

  for (i = 0; i < $ids.length; i++) { 
    var values = [];
     for(m = hyb_score; m < 46; m++) {
          $("#tab1 #" + $ids[i] + " #s" + m).each(function() { 
            values.push(parseInt($(this).text())) 
      })}
    var text = $("#tab3 pre:eq(" + i + ")").text()
    var pos = text.search("Length")
    var len = text.slice(pos + 7, pos + 20).replace(/\D/g,'');
    makeGraph(values,len, ids[i], bin_size, start, stop)
  }



function makeGraph(values, length, query_name, bin_size, start1, stop1) {
   /* console.log("Domain: " + start1 + "," + stop1)
    console.log(values)
    console.log(values.length + " total probes")
    console.log("Start:" + start1)
    console.log("Stop: " + stop1)
*/
    var formatCount = d3.format(",.0f");

    var margin = {top: 40, right: 30, bottom: 40, left: 40},
        width = 960 - margin.left - margin.right,
        height = 500 - margin.top - margin.bottom;

    if(length > 100000) {
      length = Math.round(length/100000)*100000
    }

    else if(length > 10000) {
      length = Math.round(length/10000)*10000
    }
    else if(length > 1000) {
      length = Math.round(length/1000)*1000
    }
    else if(length > 100) {
      length = Math.round(length/100)*100
    }

    var st, stp = 0

    if(start1 == 0 && stop1 == 0) {
      st = 0
      stp = length
      if(stop < length) { 
        stop = length
      }
      start = 0
    
    }
    else {
      st = start1
      stp = stop1
    }

    $(".start").val(st)
    $(".stop").val(stp)

    var x = d3.scale.linear()
        .domain([st, stp])
        .range([0, width]);

    // Generate a histogram using twenty uniformly-spaced bins.
    var data = d3.layout.histogram()
        .bins(x.ticks(bin_size))
        (values);

    barwidth = width/data.length - 1


    var y = d3.scale.linear()
        .domain([0, d3.max(data, function(d) {show_bin = d.dx; return d.y; })])
        .range([height, 0]);

    var xAxis = d3.svg.axis()
        .scale(x)
        .orient("bottom");


    var yAxis = d3.svg.axis()
        .scale(y)
        .orient("left");

    var tip = d3.tip()
        .attr('class', 'd3-tip')
        .offset([-10, 0])
        .html(function(d) {
          return "<strong>Frequency:</strong> <span style='color:red'>" + d.y + " probes (" + d.x + " - " + (d.x+d.dx) + ")</span>";
      })

    $("#l2").text("Bin Size: " + show_bin)

    var svg = d3.select("#tab4").append("svg")
        .attr("width", width + margin.left + margin.right + 30)
        .attr("height", height + margin.top + margin.bottom + 20)
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

    bar.append("rect")
        .attr("x", 1)
        .attr("width", barwidth)
        .attr("height", function(d) {return height - y(d.y); });

    svg.append("g")
        .attr("class", "x axis")
        .attr("transform", "translate(0," + height + ")")
        .call(xAxis);

    svg.append("g")
        .attr("class", "y axis")
        .call(yAxis);

    svg.append("text")
        .attr("x", (width / 2))             
        .attr("y", 0 - (margin.top / 2))
        .attr("text-anchor", "middle")  
        .style("font-size", "24px") 
        .style("text-decoration", "underline")  
        .text(query_name + " Probe Distribution");

    svg.append("text")      // text label for the x axis
        .attr("transform", "translate(" + (width / 2) + " ," + (height + margin.bottom + 15) + ")")
        .style("text-anchor", "middle")
        .style("font-size", "20px") 
        .text("Sequence Position");

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


    d3.select("#range1")
    .select("input")
    .on("change", function () {


      var bin = this.value;
      hyb_score = bin

      $("svg").remove();
      $(".d3-tip").remove();

      redraw(start,stop)

      $("#l1").text("Minimum Hybridization Score: " + hyb_score)
    });


    d3.select("#range2")
    .select("input")
    .on("change", function () {
      var bin = this.value;
      bin_size = (-2*bin) + 210

      $("svg").remove();
      $(".d3-tip").remove();

      redraw(start,stop)
          
    });


    function redraw(st, stp) {
      $("hr").remove()
      $("svg").remove();
      $(".d3-tip").remove();

      for (i = 0; i < $ids.length; i++) { 
        var values = [];
        for(m = hyb_score; m < 46; m++) {
          $("#tab1 #" + $ids[i] + " #s" + m).each(function() { 
            var val = parseInt($(this).text())
            if(val >= st && val <= stp) {
              values.push(val) 
            };
        })}

        var text = $("#tab3 pre:eq(" + i + ")").text()
        var pos = text.search("Length")
        var len = text.slice(pos + 7, pos + 20).replace(/\D/g,'');
        start = st;
        stop = stp;

        makeGraph(values,len, ids[i], bin_size, start, stop)




    }
  }



  $("button.update").on("click", function (event) {
    var st = $(".start").val()
    var stp = $(".stop").val()
    redraw(st,stp)


  })


  $("button.reset").on("click", function (event) {
        $("svg").remove();
        $(".d3-tip").remove();
        $("hr").remove()
    for (i = 0; i < $ids.length; i++) { 
      var values = [];
      for(m = hyb_score; m < 46; m++) {
          $("#tab1 #" + $ids[i] + " #s" + m).each(function() { 
            values.push(parseInt($(this).text())) 
      })}
      var text = $("#tab3 pre:eq(" + i + ")").text()
      var pos = text.search("Length")
      var len = text.slice(pos + 7, pos + 20).replace(/\D/g,'');
      start = 0
      stop = 0
      makeGraph(values,len, ids[i], bin_size, start, stop)
  }


  })


  $("button.default").on("click", function (event) {
    


  })












