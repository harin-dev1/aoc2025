#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <queue>

std::vector<std::vector<int32_t>> calc_adj_count(const std::vector<std::string>& lines) {
    std::vector<std::vector<int32_t>> adj_count_matrix(lines.size(), std::vector<int32_t>(lines[0].size(), -1));
    for (int32_t i = 0; i < lines.size(); i++) {
        for (int32_t j = 0; j < lines[i].size(); j++) {
            if (lines[i][j] == '@') {
                adj_count_matrix[i][j] = 0;
                for (int32_t k = -1; k <= 1; k++) {
                    for (int32_t l = -1; l <= 1; l++) {
                        if (k == 0 && l == 0) {
                            continue;
                        }
                        if (i + k < 0 || i + k >= lines.size() || j + l < 0 || j + l >= lines[i + k].size()) {
                            continue;
                        }
                        if (lines[i + k][j + l] == '@') {
                            adj_count_matrix[i][j]++;
                        }
                    }
                }
            }
        }
    }

    return adj_count_matrix;
}

uint64_t calc_res_a(std::vector<std::vector<int32_t>>& adj_count_matrix) {
    uint64_t res_a = 0;
    for (int32_t i = 0; i < adj_count_matrix.size(); i++) {
        for (int32_t j = 0; j < adj_count_matrix[i].size(); j++) {
            if (adj_count_matrix[i][j] < 4 && adj_count_matrix[i][j] >= 0) {
                res_a++;
            }
        }
    }
    return res_a;
}

uint64_t calc_res_b(std::vector<std::string>& lines) {
    uint64_t res_b = 0;
    while (true) {
        std::vector<std::vector<int32_t>> adj_count_matrix = calc_adj_count(lines);
        uint64_t num_changes = 0;
        for (int32_t i = 0; i < adj_count_matrix.size(); i++) {
            for (int32_t j = 0; j < adj_count_matrix[i].size(); j++) {
                if (adj_count_matrix[i][j] < 4 && adj_count_matrix[i][j] >= 0) {
                    num_changes++;
                    lines[i][j] = '.';
                }
            }
        }
        if (num_changes == 0) {
            break;
        }
        res_b += num_changes;
    }

    return res_b;
}

int main() {
    std::string input;
    std::vector<std::string> lines;
    while (std::cin >> input) {
        lines.push_back(input);
    }

    std::vector<std::vector<int32_t>> adj_count_matrix = calc_adj_count(lines);
    std::cout << calc_res_a(adj_count_matrix) << std::endl;
    std::cout << calc_res_b(lines) << std::endl;
    return 0;
}