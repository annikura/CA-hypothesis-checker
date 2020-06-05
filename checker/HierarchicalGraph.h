#ifndef CHECKER_HIERARCHICALGRAPH_H
#define CHECKER_HIERARCHICALGRAPH_H


#include "Storage.h"

class HierarchicalGraph {
public:
    explicit HierarchicalGraph(const std::vector<std::string>& strings);

    // Adds doubled path between strings a and b.
    void add_path(const std::string& a, const std::string& b);

    // Adds doubled cycle following through the passed elements in a given order.
    void add_cycle(const std::vector<std::string>& elements);

    // Removes all edges from the graph
    void clear();

    // Number of maximum layer getter.
    size_t get_max_layer() {
        return max_layer;
    }

    // Applies collapsing procedure to all nodes of level l in lexicographical order.
    void collapse_layer(size_t l);

    //Checks that hypothesis holds for edges between layers l and l - 1
    bool check_weak_components(size_t l);

    // Prints the graph
    void print();
private:
    Storage storage;
    std::vector<std::vector<Node*>> layers;
    size_t max_layer;
};


#endif //CHECKER_HIERARCHICALGRAPH_H
