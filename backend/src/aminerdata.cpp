#include <iostream>
#include <unordered_map>
#include "io/mgraph.hpp"
#include "serialization/serialization_includes.hpp"
#include "aminer.hpp"
#include "indexing/search.hpp"

using namespace std;
using namespace sae::io;

template <typename T>
inline T parse(const string& data) {
    return sae::serialization::convert_from_string<T>(data);
}

template <typename T>
inline std::unique_ptr<T> pparse(const string& data) {
    T *ret;
    std::stringstream stream;
    stream.str(data);
    sae::serialization::ISerializeStream decoder(&stream);
    decoder >> (*ret);
    return std::unique_ptr<T>(ret);
}

struct AMinerData {
    AMinerData(char const * prefix) {
        cerr << "loading aminer graph..." << endl;
        g.reset(MappedGraph::Open(prefix));

        cerr << "building index..." << endl;
        for (auto ai = g->Vertices(); ai->Alive(); ai->Next()) {
            if (ai->GlobalId() % 100000 == 0)
                cerr << "Parsing vertex: " << ai->GlobalId() << ", type: " << ai->Typename() << endl;
            if (ai->Typename() == "Author") {
                auto a = parse<Author>(ai->Data());
                author_index.addSingle(ai->GlobalId(), "name", a.names[0]);
            } else if (ai->Typename() == "Publication") {
                auto p = parse<Publication>(ai->Data());
                pub_index.addSingle(ai->GlobalId(), "title", p.title);
            } else if (ai->Typename() == "JConf") {
                auto j = parse<JConf>(ai->Data());
                jconf_index.addSingle(ai->GlobalId(), "name", j.name);
            }
        }
        cerr << "index built!" << endl;

    }

    ~AMinerData() {
    }

    indexing::SearchResult searchPublications(const string& query) {
        return indexing::Searcher(pub_index).search(query);
    }

    template<typename T>
    T get(vid_t id) {
        auto vi = g->Vertices();
        vi->MoveTo(id);
        return parse<T>(vi->Data());
    }

    indexing::Index author_index, pub_index, jconf_index;
    std::unique_ptr<MappedGraph> g;
};


int main() {
    AMinerData aminer("aminer");
    auto result = aminer.searchPublications("data mining");
    for (auto& i : result) {
        cout << "search result: " << i.docId << ", score: " << i.score << endl;
        auto p = aminer.get<Publication>(i.docId);
        cout << "title: " << p.title << endl;
    }
}
