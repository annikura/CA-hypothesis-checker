#include <algorithm>
#include <cassert>
#include <set>
#include <iostream>
#include "HierarchicalGraph.h"

static bool inlayer_cmp(Node* a, Node* b) {
    return a->get_string() < b->get_string();
}

static std::string overlap(const std::string& a, const std::string& b) {
    size_t max_len = std::min(a.size(), b.size());
    for (size_t i = max_len; i > 0; i--) {
        if (a.substr(a.size() - i, i) == b.substr(0, i)) {
            return b.substr(0, i);
        }
    }
    return "";
}

HierarchicalGraph::HierarchicalGraph(const std::vector<std::string>& strings) {
    unsigned long max_len = 0;
    storage.get_node("");

    for (auto& s: strings) {
        storage.get_node(s).set_root();
        max_len = std::max(max_len, s.size());
    }

    max_layer = max_len;

    layers.resize(max_len + 1);
    for (auto & [s, node]: storage.nodes) {
        layers[s.size()].push_back(node);
    }

    for (int i = 0; i <= max_len; i++) {
        std::sort(layers[i].begin(), layers[i].end(), inlayer_cmp);
    }
}

void HierarchicalGraph::add_path(const std::string &a, const std::string &b) {
    const std::string ovrlp = overlap(a, b);
    for (size_t i = a.size(); i > ovrlp.size(); i--) {
        storage.get_node(a.substr(a.size() - i, i)).add_suffix_edges(2);
    }
    for (size_t i = b.size(); i > ovrlp.size(); i--) {
        storage.get_node(b.substr(0, i)).add_prefix_edges(2);
    }
}

void HierarchicalGraph::add_cycle(const std::vector<std::string> &elements) {
    for (size_t i = 0; i < elements.size(); i++) {
        add_path(elements[i], elements[(i + 1) % elements.size()]);
    }
}

void HierarchicalGraph::clear() {
    for (auto& layer : layers) {
        for (auto& node : layer) {
            node->clear_edges();
        }
    }
}

void HierarchicalGraph::collapse_layer(size_t l) {
    assert(l <= max_layer);
    for (auto& node: layers[l]) {
        if (node->is_balanced() && node->has_parents() && !node->reaches_eps()) {
            node->collapse_all_but_one();
        } else {
            node->collapse_all();
        }
    }
}

bool HierarchicalGraph::check_weak_components(size_t l) {
    std::vector<std::vector<std::string>> upper_components;
    std::set<std::string> visited;
    //std::cerr << "creating upper components\n";
    for (auto& node : layers[l]) {
        if (visited.count(node->get_string())) {
            continue;
        }
        std::vector<std::string> new_visited = node->get_upper_component(l);
        visited.insert(new_visited.begin(), new_visited.end());
        new_visited.erase(std::remove_if(new_visited.begin(), new_visited.end(),
                [&l](std::string s){ return s.size() != l;}), new_visited.end());
        upper_components.emplace_back(new_visited);
    }
    std::map<std::string, int> lower_components;
    int cnt = 0;

    for (auto& node : layers[l]) {
        if (lower_components.count(node->get_string())) {
            continue;
        }
        node->get_lower_component(lower_components, cnt);
        cnt++;
    }
   for (const auto& component: upper_components) {
        for (int i = 0; i < component.size() - 1; i++) {
            if (lower_components[component[i]] != lower_components[component[i + 1]]) {
                return false;
            }
        }
    }

    return true;
}

void HierarchicalGraph::print() {
    for (size_t i = max_layer; i > 0; i--) {
        for (auto& node: layers[i]) {
            std::cout
            << "String: " << node->get_string()
            << ", from prefix: " << node->get_from_prefix()
            << ", to_suffix: " << node->get_to_suffix() << std::endl;
        }
    }
}
