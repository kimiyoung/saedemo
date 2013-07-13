/**
 * This program imports the AMiner Dataset into SAE.
 * Currently we need the following data schema for the demo.
 *
 * 	Author:
 * 		Id
 * 		Names
 * 		Position
 * 		Affiliation
 * 		H-Index
 * 		CitationNumber
 * 		PublicationNumber
 * 		Topics
 * 		ImgUrl
 *
 * 	Publication:
 * 		Id
 * 		Title
 * 		Year
 * 		Topics
 *
 * 	Conference:
 * 		JConfId
 * 		ConferenceName
 *
 *	Relations:
 *		Author-Publication
 *		Publication-Conference
 *
 */

#include <iostream>
#include <fstream>
#include <unordered_map>
#include "io/mgraph.hpp"
#include "aminer.hpp"

using namespace std;

unordered_map<int, int> aid_map;
unordered_map<int, int> pid_map;
unordered_map<int, int> jconf_map;

int getId(unordered_map<int, int>& map, int id) {
	auto it = map.find(id);
	if (it == map.end()) {
		int next = map.size();
		map.insert(make_pair(id, next));
		return next;
	} else {
		return it->second;
	}
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

#define AUTHOR_BASE 0
#define PUBLICATION_BASE 1 << 32
#define JCONF_BASE 1 << 33

int main() {
	sae::io::vid_t gvid = 0, geid = 0;

	// GraphBuilder is not efficient for this kind of large data.
	cerr << "graph builder..." << endl;
	sae::io::GraphBuilder<uint64_t> builder;
	builder.AddVertexDataType("Author");
	builder.AddVertexDataType("Publication");
	builder.AddVertexDataType("JConf");
	builder.AddEdgeDataType("Publish");
	builder.AddEdgeDataType("Appear");

	// build Authors
	cerr << "building authors..." << endl;
	{
		ifstream id2name("aid2name.txt");
		Author a;
		string names;
		while (id2name >> a.id) {
			getline(id2name >> ws, names);
			a.names = split(names, ',');
			aid_map[a.id] = gvid++;

			builder.AddVertex(a.id, a, "Author");
		}
	}

	// build Conferences
	cerr << "building conferences..." << endl;
	{
		ifstream jconfs("jconf_name.txt");
		JConf j;
		while (jconfs >> j.id) {
			getline(jconfs >> std::ws, j.name);
			jconf_map[j.id] = gvid++;
			builder.AddVertex(JCONF_BASE + j.id, j, "JConf");
		}
	}

	// build Publications
	cerr << "building publications and appear relations..." << endl;
	{
		ifstream pubs("publication.txt");
		Publication p;
		while (pubs >> p.id) {
			getline(pubs.ignore(), p.title, '\t');
			pubs >> p.jconf;
			pubs >> p.year;

			string authors;
			getline(pubs.ignore(), authors, '\t');  // unused

			string citation_number;
			getline(pubs, citation_number);
			if (citation_number.size() > 0) {
				//cerr << "!!!" << citation_number << endl;
				p.citation_number = stoi(citation_number);
			} else {
				p.citation_number = -1;
			}

			pid_map[p.id] = gvid++;
			builder.AddVertex(PUBLICATION_BASE + p.id, p, "Publication");

			auto jit = jconf_map.find(p.jconf);
			if (jit == jconf_map.end()) {
				if (p.jconf > 0)
					cerr << "jconf not found: " << p.jconf << endl;
			} else {
				builder.AddEdge(PUBLICATION_BASE + p.id, JCONF_BASE + p.jconf, PublicationJConf(), "Appear");
			}
			if (pubs.fail()) {
				cerr << "Read failed!" << p.id << endl;
				pubs.clear(ios::failbit);
			}

			//cerr << "pub id: " << p.id << " % " << gvid << " % " << p.title << " % " << authors << " % "  << p.jconf << " % " << p.year << " % "<< p.citation_number << endl;
		}
	}

	// build relations
	cerr << "building publish relations..." << endl;
	{
		ifstream a2p("a2p_a.txt");
		AuthorPublication ap;
		int aid, pid;
		int ncount = 0;
		while (a2p >> aid >> pid) {
			auto ait = aid_map.find(aid);
			auto pit = pid_map.find(pid);
			if (ait == aid_map.end()) {
				cerr << "Author not found: " << aid << endl;
			}
			if (pit == pid_map.end()) {
				cerr << "Publication not found: " << pid << ", not found count: " << (++ncount) << endl;
			}
			if (ait == aid_map.end() || pit == pid_map.end()) {
				continue;
			}
			builder.AddEdge(AUTHOR_BASE + ait->second, PUBLICATION_BASE + pit->second, ap, "Publish");
		}
	}

	cerr << "saving graph..." << endl;
	// save graph
	builder.Save("aminer");
	cerr << "saved graph" << endl;
	return 0;
}
