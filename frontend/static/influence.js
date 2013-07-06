function drawChart() {
  var container = $(".influence-graph").html('');

  var margin = {left: 40, right: 0, top: 0, bottom: 30},
      width = container.innerWidth() - margin.left - margin.right,
      height = container.innerHeight() - margin.top - margin.bottom;

  var parseDate = d3.time.format("%d-%b-%y").parse;

  var x = d3.time.scale().range([0, width]);
  var y = d3.scale.linear().range([height, 0]);

  var xAxis = d3.svg.axis().scale(x).orient("bottom");
  var yAxis = d3.svg.axis().scale(y).orient("left");

  var area = d3.svg.area()
    .x(function(d) { return x(d.date); })
    .y0(height)
    .y1(function(d) { return y(d.value); });

  var svg = d3.select(container[0]).append("svg")
    .attr("width", width + margin.left + margin.right)
    .attr("height", height + margin.top + margin.bottom)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

  d3.tsv("influence/trends.tsv", function(error, data) {
    data.forEach(function(d) {
      d.date = parseDate(d.date);
      d.value = +d.value;
    });

    x.domain(d3.extent(data, function(d) { return d.date; }));
    y.domain([0, d3.max(data, function(d) { return d.value; })]);

    svg.append("path")
      .datum(data)
      .attr("class", "area")
      .attr("d", area);

    svg.append("g")
      .attr("class", "x axis")
      .attr("transform", "translate(0," + height + ")")
      .call(xAxis);

    svg.append("g")
        .attr("class", "y axis")
        .call(yAxis)
      .append("text")
        .attr("transform", "rotate(-90)")
        .attr("y", 6)
        .attr("dy", ".71em")
        .style("text-anchor", "end")
        .text("Influence");
  });
};

function drawPie() {
  var container = $(".influence-pie").html('');
  var w = container.innerWidth();
      h = container.innerHeight();
      r = Math.min(w, h) / 2;
  color = d3.scale.category20c();

  data = [{"label":"data mining", "value":20}, 
          {"label":"XML data", "value":50}, 
          {"label":"Information Retrieval", "value":30}];
  
  var vis = d3.select(container[0])
      .append("svg:svg")
      .data([data])
          .attr("width", w)
          .attr("height", h)
      .append("svg:g")
          .attr("transform", "translate(" + r + "," + r + ")")

  var arc = d3.svg.arc()
      .outerRadius(r);

  var pie = d3.layout.pie().value(function(d) { return d.value; });

  var arcs = vis.selectAll("g.slice")
      .data(pie)
      .enter()
          .append("svg:g")
              .attr("class", "slice");

      arcs.append("svg:path")
              .attr("fill", function(d, i) { return color(i); } )
              .attr("d", arc);

      arcs.append("svg:text")
          .attr("transform", function(d) {
              d.innerRadius = 0;
              d.outerRadius = r;
              return "translate(" + arc.centroid(d) + ")";
          })
          .attr("text-anchor", "middle")
          .text(function(d, i) { return data[i].label; });
};

!function() {
  $('.topics').on('click', '.topic-list li', function() {
    if ($(this).hasClass('active')) return;
    var index = $(this).data('index');
    $('.topic-analysis:visible').fadeOut();
    $('.topic-analysis.index' + index).fadeIn();
    $(this).siblings().removeClass('active');
    $(this).addClass('active');
  });
}();

!function() {
  $(window).resize(function() {
    drawPie();
    drawChart();
  }).trigger('resize');
  $('.topics').load('influence/topics/latest', function() {
    $('.topic-list li:nth(0)').click();
  });
}();
