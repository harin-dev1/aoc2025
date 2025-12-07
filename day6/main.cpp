#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include <sstream>
#include <cassert>
#include <algorithm>

std::string get_operations(const std::string& line) {
    std::string operations;
    for (char operation : line) {
        if (operation == '+') {
            operations += '+';
        } else if (operation == '*') {
            operations += '*';
        }
    }
    return operations;
}

std::vector<uint64_t> get_result(const std::vector<std::string>& lines, const std::string& operations) {
    std::vector<uint64_t> result;
    for (char operation : operations) {
        if (operation == '+') {
            result.push_back(0);
        } else if (operation == '*') {
            result.push_back(1);
        }
    }

    for (const std::string &line : lines) {
        std::stringstream ss2(line);
        uint32_t index = 0;
        for (char c : operations) {
            uint64_t number;
            ss2 >> number;
            if (c == '+') {
                result[index] += number;
            } else {
                result[index] *= number;
            }
            index++;
        }
        
    }
    return result;
}

std::vector<std::vector<std::string>> split_operations(const std::string& operations_str) {
    std::vector<std::vector<std::string>> result_b;
    uint32_t length = 0;
    uint32_t idx = 0;
    for (uint32_t i = 1; i < operations_str.size(); i++) {
        if (operations_str[i] == ' '){
            length++;
        } else {
            result_b.push_back(std::vector<std::string>(length));
            idx++;
            length = 0;
        }
    }
    result_b.push_back(std::vector<std::string>(length + 1));
    return result_b;
}

void append_line(std::vector<std::vector<std::string>>& result_b, const std::string& line) {
    uint32_t char_idx = 0;
    for (uint32_t i = 0; i < result_b.size(); i++) {
        for (uint32_t j = 0; j < result_b[i].size(); j++) {
            assert(char_idx < line.size());
            result_b[i][j] += line[char_idx];
            char_idx++;
        }
        char_idx++;
    }
    assert(char_idx == line.size() + 1);
}

int main() {
    std::string input;
    std::vector<std::string> lines;
    while (std::getline(std::cin, input)) {
        lines.push_back(input);
    }

    std::string operations = get_operations(lines.back());
    std::string operations_str = lines.back();
    lines.pop_back();
    std::vector<uint64_t> result = get_result(lines, operations);

    uint64_t res_a = 0;
    for (uint64_t number : result) {
        res_a += number;
    }
    std::cout << res_a << std::endl;

    std::vector<std::vector<std::string>> result_b = split_operations(operations_str);
    for (const auto& line : lines) {
        append_line(result_b, line);
    }

    uint64_t res_b = 0;
    uint64_t index = 0;
    for (const auto& result : result_b) {
        uint64_t cur_res = (operations[index] == '+') ? 0 : 1;
        for (const auto& number : result) {
            if (std::all_of(number.begin(), number.end(), [](unsigned char c){ return std::isspace(c); })) {
                continue;
            }
            if (operations[index] == '+') {
                cur_res += std::stoull(number);
            } else {
                cur_res *= std::stoull(number);
            }
        }
        res_b += cur_res;
        index++;
    }

    std::cout << res_b << std::endl;

    return 0;
}