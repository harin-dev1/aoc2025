#include <iostream>
#include <string>
#include <cstdint>

int main() {
    std::string rotation;
    int position = 50;
    uint64_t res_a = 0;
    uint64_t res_b = 0;
    while (std::cin >> rotation) {
        int rotation_value = std::stoi(rotation.substr(1));
        if (rotation[0] == 'L') {
            position -= rotation_value;
        } else {
            position += rotation_value;
        }
        res_b += abs(position) / 100;
        if (position <= 0 && abs(position) != rotation_value) {
            res_b += 1;
        }
        position = (position % 100 + 100) % 100;
        if (position == 0) {
            res_a++;
        }
    }
    std::cout << res_a << std::endl;
    std::cout << res_b << std::endl;

    return 0;
}