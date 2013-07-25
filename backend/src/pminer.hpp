#include <string>
#include <vector>
#include "serialization/serialization_includes.hpp"

using namespace std;

struct Patent {
    int id;
    string title;
};

struct Inventor {
    string name;
};

struct Group {
    int id;
    string name;
    int patCount;
};

struct PatentInventor {
};

struct PatentGroup {
};

