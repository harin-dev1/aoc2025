#include <iostream>
#include <string>
#include <cstdint>
#include <algorithm>

uint64_t get_largest_possible_joltage(const std::string& bank) {
    uint64_t largest_possible_joltage = 0;
    auto max_it = std::max_element(bank.begin(), bank.end() - 1);
    size_t max_index = std::distance(bank.begin(), max_it);
    char max_digit = *max_it;
    char next_max_digit = *std::max_element(bank.begin() + max_index + 1, bank.end());
    largest_possible_joltage = static_cast<uint64_t>(max_digit - '0') * 10 +
                                   static_cast<uint64_t>(next_max_digit - '0');
    return largest_possible_joltage;
}
constexpr uint32_t num_batteries = 12;

size_t get_largest_possible_joltage_index(const std::string& bank, uint32_t battery_index,
    uint32_t start_index) {
    uint32_t num_remaining = bank.size() - start_index;
    auto max_id = std::max_element(bank.begin() + start_index, bank.begin() + start_index +
                                   (num_remaining - (num_batteries - battery_index)));
    return std::distance(bank.begin(), max_id);
}

uint64_t get_largest_possible_joltage2(const std::string& bank) {
    uint64_t largest_possible_joltage = 0;
    uint32_t start_index = 0;
    for (uint32_t i = 1; i <= num_batteries; i++) {
        size_t index = get_largest_possible_joltage_index(bank, i, start_index);
        start_index = index + 1;
        largest_possible_joltage = largest_possible_joltage * 10 + 
                                        static_cast<uint64_t>(bank[index] - '0');
    }
    return largest_possible_joltage;
}



int main() {
    std::string bank;
    uint64_t res_a = 0;
    uint64_t res_b = 0;
    while (std::cin >> bank) {
        res_a += get_largest_possible_joltage(bank);
        res_b += get_largest_possible_joltage2(bank);
    }

    std::cout << res_a << std::endl;
    std::cout << res_b << std::endl;

    return 0;
}