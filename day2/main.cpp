#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include <sstream>

bool is_invalid(uint64_t number) {
    std::string number_str = std::to_string(number);
    if (number_str.size() % 2 == 0 &&
        number_str.substr(0, number_str.size() / 2) == 
        number_str.substr(number_str.size() / 2)) {
        return true;
    }
    return false;
}

bool is_invalid_b(uint64_t number) {
    std::string number_str = std::to_string(number);
    for (uint32_t i = 2; i <= number_str.size(); i++) {
        if (number_str.size() % i != 0) {
            continue;
        }
        uint32_t seq_size = number_str.size() / i;
        bool is_invalid = true;
        for (uint32_t j = 0; j < i - 1; j++) {
            if (number_str.substr(j * seq_size, seq_size) !=
                number_str.substr(j * seq_size + seq_size, seq_size)) {
                is_invalid = false;
                break;
            }
        }
        if (is_invalid) {
            return true;
        }
    }
    return false;
}

int main() {
    std::string input;

    std::cin >> input;

    std::vector<std::pair<uint64_t, uint64_t>> ranges;
    std::stringstream ss(input);
    uint64_t res_a = 0;
    uint64_t res_b = 0;

    while (std::getline(ss, input, ',')) {
        std::stringstream ss2(input);
        uint64_t start, end;
        ss2 >> start;
        ss2.ignore(1);
        ss2 >> end;
        ranges.push_back(std::make_pair(start, end));
    }

    for (const auto& range : ranges) {
        for (uint64_t i = range.first; i <= range.second; i++) {
            if (is_invalid(i)) {
                res_a += i;
            }
            if (is_invalid_b(i)) {
                res_b += i;
            }
        }
    }

    std::cout << res_a << std::endl;
    std::cout << res_b << std::endl;
    return 0;
}