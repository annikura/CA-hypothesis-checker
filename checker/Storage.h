#ifndef CHECKER_STORAGE_H
#define CHECKER_STORAGE_H

#include <map>

class Storage;

#include "Node.h"

// Hierarchical graph node storage
class Storage {
public:
    // Returns node corresponding to the string. Builds node subtree if it didn't exist.
    Node& get_node(const std::string& s);

    ~Storage() {
        for (auto & [s, node]: nodes) {
            delete node;
        }
    }
private:
    std::map<std::string, Node*> nodes;
    friend class HierarchicalGraph;
};


#endif //CHECKER_STORAGE_H
