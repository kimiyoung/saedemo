#!env python2

import os.path
from bottle import route, run, template, view, static_file, request, urlencode
from saeclient import SAEClient

import sample_data

client = SAEClient("tcp://127.0.0.1:40111")


@route('/')
def index():
    return template('index')


@route('/<dataset>/search')
@view('search')
def search(dataset):
    q = request.query.q or ''
    print 'searching', q, 'in academic'
    result = client.entity_search(dataset, q)
    print result

    return dict(
        query=q,
        encoded_query=urlencode({"q": result.query}),
        count=result.total_count,
        results_title=dataset,
        results=[
            dict(
                id=e.id,
                name=e.title,
                url="link:entity/%s" % e.id,
                description=e.description,
                stats=dict(
                    (s.type, s.value) for s in e.stat
                ),
                topics=e.topics,
                imgurl=e.imgurl
            ) for e in result.entities
        ],
        extra_results_list=[
            {
                "title": extra_list.title,
                "items": [
                    {
                        "title": item.title,
                        "link": "link:%s/%s" % (extra_list.title, item.id)
                    } for item in extra_list.item
                ]
            } for extra_list in result.extra_list
        ]
    )


@route('/patent/search')
@view('search')
def search():
    q = request.query.q or ''
    print 'searching', q, 'in patent'
    return dict(
        query=q,
        count=0,
        results=[],
        encoded_query=urlencode({"q": q})
    )


@route('/weibo/search')
@view('search')
def search():
    q = request.query.q or ''
    print 'searching', q, 'in weibo'
    return dict(
        query=q,
        count=0,
        results=[],
        encoded_query=urlencode({"q": q})
    )


@route('/<data>/topictrends')
@view('topictrends')
def search(data):
    q = request.query.q or ''
    print 'rendering trends for', q, 'on', data
    return dict(
        query=q
    )


@route('/<data>/<uid:int>/influence/trends.tsv')
def influence_trends(data, uid):
    return open('static/influence.tsv')


@route('/<data>/<uid:int>/influence/topics/<date>')
@view('influence_topics')
def influence_topics(data, uid, date):
    # TODO return topics for the given data
    return sample_data.influence_topics


@route('/<data>/<uid:int>/influence')
@view('influence')
def influence(data, uid):
    return sample_data.influence_index


@route('/static/<path:path>')
def static(path):
    curdir = os.path.dirname(os.path.realpath(__file__))
    return static_file(path, root=curdir + '/static/')

run(server='auto', host='0.0.0.0', port=8082, reloader=True, debug=True)
