#include "search.hpp"
#include "indexing/search.hpp"
#include "interface.pb.h"

using namespace std;
using namespace demoserver;
using namespace indexing;

namespace {
    DocumentCollection testData() {
        DocumentCollection dc;
        Document doc1, doc2, doc3;
        doc1.id = 1;
        doc2.id = 2;
        doc3.id = 3;
        doc1.push_back({"title", "initial document"});
        doc1.push_back({"content", "To maximize the scale of the community around a project, by reducing the friction for new Contributors and creating a scaled participation model with strong positive feedbacks;"});
        doc2.push_back({"title", "second document"});
        doc2.push_back({"content", "To relieve dependencies on key individuals by separating different skill sets so that there is a larger pool of competence in any required domain;"});
        doc3.push_back({"title", "third document"});
        doc3.push_back({"content", "To allow the project to develop faster and more accurately, by increasing the diversity of the decision making process;"});
        dc[1] = doc1;
        dc[2] = doc2;
        dc[3] = doc3;
        return dc;
    }

    DocumentCollection& getDataset(const string& name) {
        // XXX only test data now
        static DocumentCollection dc = testData();
        return dc;
    }

    Index& getIndex(const DocumentCollection& dc) {
        // XXX only test data now
        static Index index = Index::build(dc);
        return index;
    }

    SearchResult search(const DocumentCollection &dataset, const string &query) {
        Searcher searcher(getIndex(dataset));
        return searcher.search(query);
    }
}

namespace demoserver {

bool EntitySearch(const string& input, string& output) {
    EntitySearchRequest request;
    request.ParseFromString(input);

    string dataset = request.dataset();
    DocumentCollection& dc = getDataset(dataset);
    string query = request.query();
    cout << "searching on " << dataset << " for query " << query << endl;
    auto result = search(dc, query);

    EntitySearchResponse response;
    response.set_total_count(result.total_count);
    response.set_query(query);

    for (int i = 0; i < result.size(); i ++)
    {
        DetailedEntity *de = response.add_entity();
        int id = result[i].docId;
        de->set_title(dc[id][0].value);
        de->set_id(id);
    }

    return response.SerializeToString(&output);
}

}
