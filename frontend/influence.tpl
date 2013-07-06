<style type="text/css">
    .influence-graph, .influence-pie {
        font: 10px sans-serif;
    }

    .influence-graph .axis path,
    .influence-graph .axis line {
        fill: none;
        stroke: #000;
        shape-rendering: crispEdges;
    }
     
    .influence-graph .x.axis path {
        display: none;
    }
     
    .influence-graph .area {
        fill: steelblue;
    }

</style>
<div class="hero-unit">
    <h2>Influence graph of <b>{{name}}</b></h2>
    <div class="row-fluid">
        <div class="span2">
            <img src="{{imgurl}}" alt="{{name}}" style="width: 100%; height: auto;"/>
            <p>{{name}}
        </div>
        <div class="span7">
            <div class="influence-graph" style="height: 200px; width: 100%;"></div>
        </div>
        <div class="span3">
            <div class="influence-pie" style="height: 200px; width: 100%;"></div>
        </div>
    </div>
</div>

<div class="topics">
</div>
%scripts = ["/static/influence.js"]
%rebase layout scripts=scripts
