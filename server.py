#!env python2

import os.path
from bottle import route, run, template, view, static_file, request, urlencode
from dcclient import DataCenterClient

import sample_data

client = DataCenterClient("tcp://10.1.1.211:32011")


@route('/')
def index():
    return template('index')


@route('/academic/search')
@view('search')
def search():
    q = request.query.q or ''
    print 'searching', q, 'in academic'
    fields = "naid names homepage affiliation position".split()
    fields += "citation_no pub_count h_index".split()
    fields += "interest_years interest_by_year".split()
    fields += "imgurl imgname imgsrc".split()
    result = client.searchAuthors(q, returned_fields=fields)
    conferences = client.searchConferences(q)
    publications = client.searchPublications(q)

    def genimgurl(imgurl, imgname, imgsrc):
        # TODO https://gist.github.com/anonymous/5677264
        defaultImgUrl = "http://pic.aminer.org/picture/images/no_photo.jpg"
        if imgurl and imgurl.strip().startswith("http://") and \
                'arnetminer' in imgurl and '/upload/' in imgurl:
            return imgurl
        if imgname:
            if imgname.lower().startswith('http'):
                return imgname
            else:
                return 'http://pic1.aminer.org/picture/' + imgname
        elif imgurl.startswith('http://'):
            return imgurl
        elif imgsrc.strip():
            return imgsrc
        return defaultImgUrl

    def gentopics(interest_years, interest_by_year):
        if interest_years:
            interests = sorted(zip(interest_years, interest_by_year))
            return interests[-1][1].split(',')
        else:
            return []

    return dict(
        query=q,
        encoded_query=urlencode({"q": q}),
        count=result.total_count,
        results_title="Experts",
        results=[
            dict(
                id=a.naid,
                name=a.names[0],
                url="http://aminer.org/person/-%s.html" % a.naid,
                description="%s, %s" % (a.position, a.affiliation)
                    if a.affiliation else a.position,
                stats=dict(
                    h_index=a.h_index,
                    papers=a.pub_count,
                    citations=a.citation_no
                ),
                topics=gentopics(a.interest_years, a.interest_by_year),
                imgurl=genimgurl(a.imgurl, a.imgname, a.imgsrc),
            ) for a in result.authors
        ],
        extra_results_list=[
            {
                "title": "Conferences",
                "items": [
                    {
                        "text": c.name,
                        "link": "http://aminer.org/conference/-%s.html" % c.id
                    } for c in conferences.confs
                ]
            },
            {
                "title": "Publications",
                "items": [
                    {
                        "text": p.title,
                        "link": "http://aminer.org/publication/-%s.html" % p.id
                    } for p in publications.publications
                ]
            },
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

run(server='auto', host='0.0.0.0', port=8080, reloader=True, debug=True)
