#include <random>

#include <iostream>
#include <algorithm>
#include "Storage.h"
#include "HierarchicalGraph.h"

// Runs hypothesis checks on given set of strings tries times (each time generating a new walk permutation)
std::vector<std::string> test(std::vector<std::string> strs, int tries = 1, bool log = false);

// Generates string that contains up to max_alphabet different characters and has length up to max_length.
std::string generate_string(int max_alphabet, int max_length);

// Generates random set of strings generated independently. Set will contain up to max_strings strings.
std::vector<std::string> generate_random_set(int max_alphabet, int max_strings, int max_length);

//Generates set of strings based on a single random superstring. Set will contain up to max_strings strings.
std::vector<std::string> generate_substrings_set(int max_alphabet, int max_strings, int max_length);

// Prints error message about invalid input arguments.
void bad_params() {
    std::cout << "Invalid arguments, try --help" << std::endl;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        bad_params();
        return 0;
    }
    if (std::string(argv[1]) == "--help") {
        std::cout << "Use '--help' to see this message\n"
                     "Use 'GENERATION_TYPE NUMBER_OF_SETS NUMBER_OF_PERMUTATIONS_PER_SET' to run checks\n"
                     "\tGENERATION_TYPE is one of {'S', 'R'}, where\n"
                     "\t\tS stands for substrings dataset,\n"
                     "\t\tR stands for random dataset.\n"
                     "Example call: '.\\out S'";
        return 0;
    }
    std::string check_type(argv[1]);

    if (check_type != "S" && check_type != "R") {
        bad_params();
        return 0;
    }

    int cnt = 0;

    auto generating_fundtion = check_type == "R" ? generate_random_set : generate_substrings_set;

    while (true) {
        std::cout << "Generating set #" << cnt << std::endl;
        if (!test(generating_fundtion(7, 10, 6), 5).empty()) {
            std::cerr << "Hypothesis failed";
            break;
        }
        cnt++;
    }

    return 0;
}

static std::random_device rd;
static std::mt19937 rnd(rd());

std::vector<std::string> test(std::vector<std::string> strs, int tries, bool log) {
    strs.emplace_back("");
    if (log)
        std::cerr << "Building graph\n";
    HierarchicalGraph hg = HierarchicalGraph(strs);
    if (log)
        std::cerr << "Built graph\n";
    for (int i = 0; i < tries; i++) {
        std::cout << "Starting try #" << i << "    ";
        std::shuffle(strs.begin(), strs.end(), rnd);
        std::cout << "Order: ";
        for (auto& s : strs) {
            std::cout << s << ", ";
        }
        std::cout << std::endl;

        if (log)
            std::cerr << "Creating cycle\n";
        hg.add_cycle(strs);

        if (log)
            std::cerr << "Created cycle\n";
        for (size_t j = hg.get_max_layer(); j > 0; j--) {
            if (log)
                std::cerr << "Checking weak components for layer #" << j << "\n";
            if (!hg.check_weak_components(j)) {
                return strs;
            }
            if (log)
                std::cerr << "Checked, collapsing\n";
            hg.collapse_layer(j);
            if (log)
                std::cerr << "Collapsed\n";
        }
        hg.clear();
    }

    return {};
}

std::string generate_string(int max_alphabet, int max_length) {
    std::uniform_int_distribution<> len_gen(2, max_length);
    std::uniform_int_distribution<> alp_gen(0, max_alphabet);

    int len = len_gen(rnd);
    std::string s;

    for (int i = 0; i < len; i++) {
        char sym = static_cast<char>(alp_gen(rnd) + 'a');
        s += sym;
    }
    return s;
}

std::vector<std::string> generate_random_set(int max_alphabet, int max_strings, int max_length) {
    std::uniform_int_distribution<> str_gen(3, max_strings);

    std::vector<std::string> result;
    int size = str_gen(rnd);

    result.reserve(static_cast<unsigned long>(size));
    for (int i = 0; i < size; i++) {
        result.emplace_back(generate_string(max_alphabet, max_length));
    }

    return result;
}

std::vector<std::string> generate_substrings_set(int max_alphabet, int max_strings, int max_length) {
    std::uniform_int_distribution<> str_gen(3, max_strings);

    std::vector<std::string> result;
    int size = str_gen(rnd);
    result.reserve(static_cast<unsigned long>(size));

    std::uniform_int_distribution<> s_len_gen(max_length, max_length * size / 2);

    std::string s = generate_string(max_alphabet, s_len_gen(rnd));
    std::uniform_int_distribution<> position_gen(0, static_cast<int>(s.size() - 1));

    for (auto i = 0; i < size; i++) {
        int start = position_gen(rnd), end = position_gen(rnd);
        if (start > end) {
            std::swap(start, end);
        }
        end++;
        result.emplace_back(s.substr(static_cast<unsigned long>(start), static_cast<unsigned long>(end - start)));
    }

    return result;
}