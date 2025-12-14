#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <regex>
#include <sstream>
#include <unordered_set>
#include <stack>

class Graph {
private:
    std::unordered_map<std::string, std::vector<std::string>> adj_list;
    std::stack<std::string> topological_order;
    void parse_input() {
        std::string line;
        while (std::getline(std::cin, line)) {
            std::regex re(R"((\w+): ([\w\s]+))");
            std::smatch match;
            if (std::regex_match(line, match, re)) {
                std::string key = match[1].str();
                std::stringstream ss(match[2].str());
                std::string value;
                while (ss >> value) {
                    adj_list[key].push_back(value);
                }
            }
        }
    }
    void dfs(const std::string& start, std::unordered_set<std::string>& visited) {
        visited.insert(start);
        if (adj_list.find(start) != adj_list.end()) {
            for (const auto& neighbor : adj_list.at(start)) {
                if (visited.find(neighbor) == visited.end()) {
                    dfs(neighbor, visited);
                }
            }
        }
        topological_order.push(start);
    }
public:
    Graph reverse_graph() {
        Graph reverse_graph;
        for (const auto& [key, value] : adj_list) {
            for (const auto& neighbor : value) {
                reverse_graph.adj_list[neighbor].push_back(key);
            }
        }
        return reverse_graph;
    }

    Graph() {
        parse_input();
    }

    std::stack<std::string> topological_sort() {
        if (topological_order.empty()) {
            std::unordered_set<std::string> visited;
            dfs("out", visited);
        }
        return topological_order;
    }

    const std::vector<std::string>& get_adjacent_nodes(const std::string& node) const {
        return adj_list.at(node);
    }
    std::unordered_map<std::string, uint64_t> num_paths(std::string start) {
        std::stack<std::string> topological_order = reverse_graph().topological_sort();
        std::unordered_map<std::string, uint64_t> num_out_paths;
        num_out_paths[start] = 1;
        topological_order.pop();
        while (!topological_order.empty()) {
            std::string node = topological_order.top();
            topological_order.pop();
            for (const auto& neighbor : adj_list.at(node)) {
                num_out_paths[node] += num_out_paths[neighbor];
            }
        }
        return num_out_paths;
    }
};


int main() {
    Graph graph;
    std::unordered_map<std::string, uint64_t> num_out_paths = graph.num_paths("out");
    std::unordered_map<std::string, uint64_t> num_fft_paths = graph.num_paths("fft");
    std::unordered_map<std::string, uint64_t> num_dac_paths = graph.num_paths("dac");

    uint64_t dac_to_out_paths = num_out_paths["dac"];
    uint64_t fft_to_out_paths = num_out_paths["fft"];
    uint64_t fft_to_dac_paths = num_dac_paths["fft"];
    uint64_t dac_to_fft_paths = num_fft_paths["dac"];
    uint64_t svr_to_fft_paths = num_fft_paths["svr"];
    uint64_t svr_to_dac_paths = num_dac_paths["svr"];

    uint64_t res_b = svr_to_fft_paths * fft_to_dac_paths * dac_to_out_paths + 
                     svr_to_dac_paths * dac_to_fft_paths * fft_to_out_paths;
    std::cout << num_out_paths["you"] << std::endl;
    std::cout << res_b << std::endl;
    return 0;
}