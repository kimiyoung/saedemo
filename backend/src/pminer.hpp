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

namespace sae {
    namespace serialization {
        namespace custom_serialization_impl {
            template <>
            struct serialize_impl<sae::serialization::OSerializeStream, Patent> {
                static void run(sae::serialization::OSerializeStream& ostr, Patent& p) {
                    ostr << p.id << p.title;
                }
            };

            template <>
            struct deserialize_impl<sae::serialization::ISerializeStream, Patent> {
                static void run(sae::serialization::ISerializeStream& istr, Patent& p) {
                    istr >> p.id >> p.title;
                }
            };
        }
    }
}

namespace sae {
    namespace serialization {
        namespace custom_serialization_impl {
            template <>
            struct serialize_impl<sae::serialization::OSerializeStream, Inventor> {
                static void run(sae::serialization::OSerializeStream& ostr, Inventor& i) {
                    ostr << i.name;
                }
            };

            template <>
            struct deserialize_impl<sae::serialization::ISerializeStream, Inventor> {
                static void run(sae::serialization::ISerializeStream& istr, Inventor& i) {
                    istr >> i.name;
                }
            };
        }
    }
}

namespace sae {
    namespace serialization {
        namespace custom_serialization_impl {
            template <>
            struct serialize_impl<sae::serialization::OSerializeStream, Group> {
                static void run(sae::serialization::OSerializeStream& ostr, Group& g) {
                    ostr << g.id << g.name << g.patCount;
                }
            };

            template <>
            struct deserialize_impl<sae::serialization::ISerializeStream, Group> {
                static void run(sae::serialization::ISerializeStream& istr, Group& g) {
                    istr >> g.id >> g.name >> g.patCount;
                }
            };
        }
    }
}
