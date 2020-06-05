#include "Storage.h"

Node &Storage::get_node(const std::string& s) {
    if (!nodes.count(s)) {
        if (s.empty()) {
            nodes[s] = new Node();
        } else {
            nodes[s] = new Node(s, *this);
        }
    }
    return *nodes[s];
}
