#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cstdio>
#include "io/mgraph.hpp"
#include "pminer.hpp"

using namespace std;

#define GROUP_BASE 0
#define PATENT_BASE 200000
#define INVENTOR_BASE 4500000

vector<string> split(string s, char c) {
    int last = 0;
    vector<string> v;
    for (int i=0; i<s.size(); i++) {
        if (s[i] == c) {
            v.push_back(s.substr(last, i - last));
            last = i + 1;
        }
    }
    v.push_back(s.substr(last, s.size() - last));
    return v;
}

int convert_to_int(string s) {
    istringstream is;
    is.str(s);
    int i;
    is >> i;
    return i;
}

int main() {
    sae::io::GraphBuilder<uint64_t> builder;
    builder.AddVertexDataType("Patent");
    builder.AddVertexDataType("Inventor");
    builder.AddVertexDataType("Group");
    builder.AddEdgeDataType("PatentInventor");
    builder.AddEdgeDataType("PatentGroup");

    cerr << "Loading company.txt..." << endl;
    map<int, int> com2group;
    FILE* company = fopen("company.txt", "r");
    int x, y;
    while (fscanf(company, "%d\t%d", &x, &y) != EOF) {
        com2group[x] = y;
    }
    fclose(company);

    cerr << "Loading groupcom.txt..." << endl;
    ifstream group_file("groupcom.txt");
    string group_input;
    while (getline(group_file, group_input)) {
        vector<string> inputs = split(group_input, '\t');
        int id = convert_to_int(inputs[0]);
        string name = inputs[1];
        int patentCount = convert_to_int(inputs[2]);

        Group group{id, name, patentCount};
        builder.AddVertex(GROUP_BASE + id, group, "Group");
    }
    group_file.close();

    cerr << "Loading patent.txt ..." << endl;
    map<string, vector<int>> inventor_map;
    ifstream patent_file("patent.txt");
    string patent_input;
    while (getline(patent_file, patent_input)) {
        vector<string> inputs = split(patent_input, '\t');
        int id = convert_to_int(inputs[0]);
        string title = inputs[1];
        vector<string> inventors = split(inputs[2].substr(1, inputs[2].size() - 1), '#');

        for (auto it = inventors.begin(); it!=inventors.end(); it++) {
            inventor_map[*it].push_back(id);
        }
        
        Patent patent{id, title};
        builder.AddVertex(PATENT_BASE + id, patent, "Patent");
    }
    patent_file.close();

    cerr << "Crearing Patent-Inventor ... " << endl;
    int inventor_id = 0;
    for (auto it = inventor_map.begin(); it!=inventor_map.end(); it++) {
        Inventor inventor{(*it).first};
        builder.AddVertex(INVENTOR_BASE + inventor_id, inventor, "Inventor");

        for (auto vit = (*it).second.begin(); vit != (*it).second.end(); vit++) {
            builder.AddEdge(INVENTOR_BASE + inventor_id, PATENT_BASE + (*vit), PatentInventor(), "PatentInventor");
        }
        inventor_id ++;
    }

    cerr << "Loading pat2com.txt ..." << endl;
    FILE* pat2com_file = fopen("pat2com.txt", "r");
    int pat, com;
    while (fscanf(pat2com_file, "%d\t%d", &pat, &com) != EOF) {
        int group = com2group[com];
        builder.AddEdge(PATENT_BASE + pat, GROUP_BASE + group, PatentGroup(), "PatentGroup");
    }
    fclose(pat2com_file);

    cerr << "Saving graph pminer..." << endl;
    builder.Save("pminer");

    return 0;
}
