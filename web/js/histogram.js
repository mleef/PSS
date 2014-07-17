
  var queryIDs = []
  $("#tab1 caption").each(function() { queryIDs.push($(this).text()) });

  //console.log(queryIDs)

  queryIDs.forEach( function (element) {
    //console.log(element.replace(/[&\/\\#,|+()$~%.'":*?<>{}]/g,'\\\\'))
    var values = [];
    $("#" + element + " p").each(function() { values.push($(this).text()) });
    //console.log("#" + element + " p")
    //console.log(values)
    var text = $("#tab3 pre").text()
    var pos = text.search("Length")
    var len = text.slice(pos + 7, pos + 20).replace(/\D/g,'');
    makeGraph(values,len, element)
  })
  



  

function makeGraph(values, length, query_name) {

    var formatCount = d3.format(",.0f");

    var margin = {top: 40, right: 30, bottom: 40, left: 30},
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

    var x = d3.scale.linear()
        .domain([0, length])
        .range([0, width]);

    // Generate a histogram using twenty uniformly-spaced bins.
    var data = d3.layout.histogram()
        .bins(x.ticks(20))
        (values);

    var y = d3.scale.linear()
        .domain([0, d3.max(data, function(d) { return d.y; })])
        .range([height, 0]);

    var xAxis = d3.svg.axis()
        .scale(x)
        .orient("bottom");

    var tip = d3.tip()
        .attr('class', 'd3-tip')
        .offset([-10, 0])
        .html(function(d) {
          console.log(d)
          return "<strong>Frequency:</strong> <span style='color:red'>" + d.y + " probes" + "</span>";
      })

    var svg = d3.select("#tab4").append("svg")
        .attr("width", width + margin.left + margin.right)
        .attr("height", height + margin.top + margin.bottom + 20)
      .append("g")
        .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

    svg.call(tip);

    var bar = svg.selectAll(".bar")
        .data(data)
      .enter().append("g")
        .attr("class", "bar")
        .attr("transform", function(d) { return "translate(" + x(d.x) + "," + y(d.y) + ")"; })
        .on('mouseover', tip.show)
        .on('mouseout', tip.hide);

    bar.append("rect")
        .attr("x", 1)
        .attr("width", x(data[0].dx) - 1)
        .attr("height", function(d) { return height - y(d.y); });

    svg.append("g")
        .attr("class", "x axis")
        .attr("transform", "translate(0," + height + ")")
        .call(xAxis);

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
        .attr("y", 0 - margin.left)
        .attr("x",0 - (height / 2))
        .attr("dy", "1em")
        .style("font-size", "20px") 
        .style("text-anchor", "middle")
        .text("# Probes");

  }







