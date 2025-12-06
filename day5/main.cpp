#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <algorithm>

std::vector<std::pair<uint64_t, uint64_t>> merge_ranges(std::vector<std::pair<uint64_t, uint64_t>>& ranges) {
    std::sort(ranges.begin(), ranges.end(), [](const auto& a, const auto& b) {
        return a.first < b.first;
    });

    std::vector<std::pair<uint64_t, uint64_t>> merged_ranges;
    for (const auto& range : ranges) {
        if (merged_ranges.empty() || merged_ranges.back().second < range.first) {
            merged_ranges.push_back(range);
        } else {
            merged_ranges.back().second = std::max(merged_ranges.back().second, range.second);
        }
    }

    return merged_ranges;
}

int main() {
    std::string line;
    std::vector<std::pair<uint64_t, uint64_t>> ranges;
    uint64_t res_a = 0;

    // Parse ranges section
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }
        auto pos = line.find('-');
        ranges.push_back(std::make_pair(std::stoull(line.substr(0, pos)), 
                                        std::stoull(line.substr(pos + 1))));
    }

    // Parse numbers section
    while (std::getline(std::cin, line)) {
        uint64_t number = std::stoull(line);
        for (const auto& range : ranges) {
            if (number >= range.first && number <= range.second) {
                res_a++;
                break;
            }
        }
    }


    uint64_t res_b = 0;

    std::vector<std::pair<uint64_t, uint64_t>> merged_ranges = merge_ranges(ranges);
    for (const auto& range : merged_ranges) {
        res_b += range.second - range.first + 1;
    }

    std::cout << res_a << std::endl;
    std::cout << res_b << std::endl;
    return 0;
}