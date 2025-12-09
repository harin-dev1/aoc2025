#include <iostream>
#include <string>
#include <cstdint>
#include <vector>

std::pair<int32_t, int32_t> get_start_pos(const std::vector<std::string>& input) {
    for (int32_t i = 0; i < input[0].size(); i++) {
        if (input[0][i] == 'S') {
            return std::make_pair(0, i);
        }
    }
    return std::make_pair(-1, -1);
}

void dfs(std::vector<std::string>& input, int32_t row, int32_t col) {
    input[row][col] = '|';
    if (row == input.size() - 1) {
        return;
    }
    if (input[row + 1][col] == '.') {
        dfs(input, row + 1, col);
    }
    if (input[row + 1][col] == '^') {
        if (col - 1 >= 0) {
            dfs(input, row + 1, col - 1);
        }
        if (col + 1 < input[row + 1].size()) {
            dfs(input, row + 1, col + 1);
        }
    }
}

uint32_t get_split_count(const std::vector<std::string>& input) {
    uint32_t split_count = 0;
    for (int32_t i = 0; i < input.size(); i++) {
        for (int32_t j = 0; j < input[i].size(); j++) {
            if (input[i][j] == '^' && input[i - 1][j] == '|') {
                split_count++;
            }
        }
    }
    return split_count;
}

uint64_t bfs(std::vector<std::string>& input, int32_t row, int32_t col) {
    uint64_t timelines = 0;
    std::vector<std::pair<int32_t, int32_t>> bfs_queue;
    std::vector<std::vector<uint64_t>> timeline_matrix(input.size(), std::vector<uint64_t>(input[0].size(), 0));
    timeline_matrix[row][col] = 1;
    bfs_queue.push_back(std::make_pair(row, col));
    uint32_t level = 1;
    while (!bfs_queue.empty()) {
        std::vector<std::pair<int32_t, int32_t>> new_bfs_queue;
        for (const auto& current : bfs_queue) {
            if (current.first == input.size() - 1) {
                timelines += timeline_matrix[current.first][current.second];
                continue;
            }
            if (input[current.first + 1][current.second] == '^') {
                if (current.second - 1 >= 0) {
                    timeline_matrix[current.first + 1][current.second - 1] += timeline_matrix[current.first][current.second];
                    if (new_bfs_queue.empty() || new_bfs_queue.back() != std::make_pair(current.first + 1, current.second - 1)) {
                        new_bfs_queue.push_back(std::make_pair(current.first + 1, current.second - 1));
                    }
                }
                if (current.second + 1 < input[current.first + 1].size()) {
                    timeline_matrix[current.first + 1][current.second + 1] += timeline_matrix[current.first][current.second];
                    if (new_bfs_queue.empty() || new_bfs_queue.back() != std::make_pair(current.first + 1, current.second + 1)) {
                        new_bfs_queue.push_back(std::make_pair(current.first + 1, current.second + 1));
                    }
                }
            }
            if (input[current.first + 1][current.second] == '|') {
                timeline_matrix[current.first + 1][current.second] += timeline_matrix[current.first][current.second];
                if (new_bfs_queue.empty() || new_bfs_queue.back() != std::make_pair(current.first + 1, current.second)) {
                    new_bfs_queue.push_back(std::make_pair(current.first + 1, current.second));
                }
            }
        }
        bfs_queue = new_bfs_queue;
    }
    #if 0
    for (int32_t i = 0; i < input.size(); i++) {
        for (int32_t j = 0; j < input[i].size(); j++) {
            std::cout << timeline_matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
    #endif
    return timelines;
}
int main() {
    std::string s;
    std::vector<std::string> input;
    while (std::getline(std::cin, s)) {
        input.push_back(s);
    }

    std::pair<int32_t, int32_t> start_pos = get_start_pos(input);

    dfs(input, start_pos.first, start_pos.second);
    std::cout << get_split_count(input) << std::endl;
    std::cout << bfs(input, start_pos.first, start_pos.second) << std::endl;
    return 0;
}