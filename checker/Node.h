#ifndef CHECKER_NODE_H
#define CHECKER_NODE_H

#include <vector>
#include <string>
#include <set>
#include <algorithm>

class Node;
#include "Storage.h"

// Hierarchical graph node
class Node {
public:
    // Empty string node constructor.
    Node();

    // Constructs node corresponding to string 's' in storage 'storage'
    Node(const std::string& s, Storage& storage);

    // Adds node to the list of prefix parents.
    void add_prefix_parent(Node* parent) {
        this->prefix_parents.push_back(parent);
    }

    // Adds node to the list of suffix parents.
    void add_suffix_parent(Node* parent) {
        this->suffix_parents.push_back(parent);
    }

    // Returns string corresponding to the node
    const std::string & get_string() const {
        return str;
    }

    // Removes all node edges
    void clear_edges() {
        from_prefix = 0;
        to_suffix = 0;
    }

    // Adds cnt prefix edges to the node.
    void add_prefix_edges(int cnt = 1) {
        from_prefix += cnt;
    }

    // Adds cnt suffix edges to the node.
    void add_suffix_edges(int cnt = 1) {
        to_suffix += cnt;
    }

    // Collapses all possible pairs of lower edges.
    void collapse_all() {
        collapse(std::min(from_prefix, to_suffix));
    }

    // Collapses all possible pairs of lower edges but one pair.
    void collapse_all_but_one() {
        collapse(std::max(std::min(from_prefix, to_suffix) - 1, 0UL));
    }

    // Returns true is node is edge-balanced.
    bool is_balanced() const {
        return from_prefix == to_suffix;
    }

    // Returns number of prefix edges.
    size_t get_from_prefix() const {
        return from_prefix;
    }

    // Returns number of suffix edges.
    size_t get_to_suffix() const {
        return to_suffix;
    }

    // Returns true if node is a root node or if it's a child to a root node, false otherwise.
    bool has_parents() const {
        if (is_root)
            return true;
        for (auto& node: prefix_parents) {
            if (node->from_prefix > 0)
                return true;
        }
        for (auto& node: suffix_parents) {
            if (node->to_suffix > 0)
                return true;
        }
        return false;
    }

    // Marks node as a root node
    void set_root() {
        is_root = true;
    }

    // Checks if node is reachable from eps node.
    bool reaches_eps() const;

    // Returns list of strings from weak upper component.
    std::vector<std::string> get_upper_component(size_t level) const;

    // Returns list of strings from layer below.
    void get_lower_component(std::map<std::string, int>& used, int color) const;

    // Prints node subtree
    void print() const;
private:
    void upper_dfs(size_t min_level, std::set<std::string>& visited) const;
    bool eps_search(const std::string &blocked, std::set<std::string> &visited) const;

    void collapse(size_t n) {
        n = std::min(n, std::min(from_prefix, to_suffix));
        from_prefix -= n;
        to_suffix -= n;

        prefix->to_suffix += n;
        suffix->from_prefix += n;
    }

    std::vector<Node*> prefix_parents;
    std::vector<Node*> suffix_parents;

    Node *prefix = nullptr, *suffix = nullptr;
    size_t from_prefix = 0, to_suffix = 0;
    bool is_empty = false;
    bool is_root = false;

    std::string str;
};


#endif //CHECKER_NODE_H
