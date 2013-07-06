<style type="text/css">
	.item-img {
		width: 80px;
		padding: 10px;
	}
</style>

<form class="search" method="get">
	<h2>Search...</h2>
	<input class="input-block-level" name="q" value="{{query}}"/>
	<button class="btn btn-small btn-primary" type="submit">Search</button>
</form>

<p>Hot queries:
<span><a href="search?q=data%20mining">data mining</a></span>
<span><a href="search?q=machine%20learning">machine learning</a></span>

<p><a href="topictrends?{{encoded_query}}">topic trends</a>

<div class="row-fluid">
	<div class="results span8">
	<div class="results-summary pull-right">{{count}} results, displaying 0 - 15</div>
	<h4>{{get("results_title", "Results")}}</h4>
	<ul class="unstyled">
	%for item in results:
		<li class="result-item row-fluid">
			<div class="item-img span3">
				<a href="{{item['url']}}">
					<img src="{{item['imgurl']}}" alt="{{item['name']}}" style="width: 100%; height: auto;"/>
				</a>
			</div>
			<div class="item-description span9">
				<div class="item-name">
					<a href="{{item['url']}}">{{item['name']}}</a>
					<span class="pull-right">[<a href="{{item['id']}}/influence">Influence Analysis</a>]</span>
				</div>
				{{item['description']}}
				<ul class="item-stats inline">
				%for k, v in item['stats'].items():
					<li>{{k}}: {{v}}</li>
				%end
				</ul>
				<ul class="item-topics inline">
				%for t in item['topics']:
					<li>{{t}}</li>
				%end
				</ul>
			</div>
		</li>
	%end
	</ul>
	</div>
	%if defined("extra_results_list"):
	<div class="extra-results-pane span4">
		%for extra_results in extra_results_list:
		<div class="extra-results">
			<section>
			<h4>{{extra_results['title']}}</h4>
			<ul>
			%for item in extra_results['items']:
				<li><a href="{{item['link']}}">{{item['text']}}</a></li>
			%end
			</ul>
			</section>
		</div>
		%end
	</div>
	%end
</div>

%rebase layout
