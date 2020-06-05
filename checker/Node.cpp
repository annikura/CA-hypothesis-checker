#include <iostream>
#include <set>
#include "Node.h"

Node::Node() {
    is_empty = true;
}

Node::Node(const std::string& s, Storage& storage):
    prefix(&storage.get_node(s.substr(0, s.size() - 1))),
    suffix(&storage.get_node(s.substr(1, s.size() - 1))),
    str(s) {
    this->prefix->add_prefix_parent(this);
    this->suffix->add_suffix_parent(this);
}

void Node::print() const {
    if (is_empty) {
        std::cout << "eps node!" << std::endl;
        return;
    }
    std::cout << "Entered string: \"" << str << "\" " << this << " " << std::endl;
    std::cout << "Going to prefix of " << str << std::endl;
    prefix->print();
    std::cout << "Going to suffix of " << str << std::endl;
    suffix->print();
}

bool Node::eps_search(const std::string &blocked, std::set<std::string> &visited) const {
    if (visited.count(get_string())) {
        return false;
    }
    if (is_empty) {
        return true;
    }
    visited.insert(get_string());

    if (get_string() != blocked) {
        if (from_prefix > 0) {
            if (prefix->eps_search(blocked, visited))
                return true;
        }
        if (to_suffix > 0) {
            if (suffix->eps_search(blocked, visited))
                return true;
        }
    }

    for (auto& node: prefix_parents) {
        if (node->from_prefix > 0) {
            if (node->eps_search(blocked, visited))
                return true;
        }
    }
    for (auto& node: suffix_parents) {
        if (node->to_suffix > 0) {
            if (node->eps_search(blocked, visited))
                return true;
        }
    }
    return false;
}

bool Node::reaches_eps() const {
    std::set<std::string> used;
    return eps_search(get_string(), used);
}

void Node::get_lower_component(std::map<std::string, int>& used, int color) const {
    if (used.count(str)) {
        return;
    }
    //std::cerr << "node " << str << std::endl;
    //print();

    used[this->get_string()] = color;
    if (from_prefix > 0) {
        //std::cerr << "to prefix\n";
        for (auto& node: prefix->suffix_parents) {
            if (node->to_suffix > 0) {
                //std::cerr << "to suffix of prefix: \"" << node->get_string() << "\"" << std::endl;
                node->get_lower_component(used, color);
            }
        }
        for (auto& node: prefix->prefix_parents) {
            //std::cerr << "to prefix of prefix: " << node->get_string() << std::endl;
            if (node->from_prefix > 0) {
                node->get_lower_component(used, color);
            }
        }
    }
    if (to_suffix > 0) {
        //std::cerr << "to suffix\n";
        for (auto& node: suffix->suffix_parents) {
            if (node->to_suffix > 0) {
                node->get_lower_component(used, color);
            }
        }
        for (auto& node: suffix->prefix_parents) {
            if (node->from_prefix > 0) {
                node->get_lower_component(used, color);
            }
        }
    }
}

void Node::upper_dfs(size_t min_level, std::set<std::string>& visited) const {
    if (visited.count(get_string())) {
        return;
    }
    visited.insert(get_string());
    if (get_string().size() > min_level) {
        if (from_prefix > 0) {
            prefix->upper_dfs(min_level, visited);
        }
        if (to_suffix > 0) {
            suffix->upper_dfs(min_level, visited);
        }
    }

    for (auto& node: prefix_parents) {
        if (node->from_prefix > 0) {
            node->upper_dfs(min_level, visited);
        }
    }
    for (auto& node: suffix_parents) {
        if (node->to_suffix > 0) {
            node->upper_dfs(min_level, visited);
        }
    }
}

std::vector<std::string> Node::get_upper_component(size_t level) const {
    std::set<std::string> visited;
    upper_dfs(level, visited);
    return std::vector(visited.begin(), visited.end());
}
