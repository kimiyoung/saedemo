<style type="text/css">
    .topic-list li {
        cursor: pointer;
    }
</style>
<div class="row-fluid topic">
    <div class="span4">
        <ul class="topic-list nav nav-tabs nav-stacked">
            %counter = 0
            %for topic in topics:
                <li data-index="{{counter}}"><a>{{topic['topic']}} ({{topic['score']}})</a></li>
                %counter += 1
            %end
        </ul>
    </div>
    <div class="span8">
        %counter = 0
        %for topic in topics:
        <div class="topic-analysis index{{counter}}" style="{{"display:none" if counter != 0 else ""}}">
            <p> Influenced by {{name}}:
            <ul>
            %for influencee in topic["influencees"]:
                <li>{{influencee[0]}} (because he/she is {{name}}'s {{influencee[2]}}) </li>
            %end
            </ul>
            <p> Influencers:
            <ul>
            %for influencer in topic["influencers"]:
                <li>{{influencer[0]}} (because he/she is {{name}}'s {{influencer[2]}}) </li>
            %end
            </ul>
            %counter += 1
        </div>
        %end
    </div>
</div>

