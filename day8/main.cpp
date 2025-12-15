#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <unordered_map>
#include <regex>
#include <set>

struct position {
    uint64_t x;
    uint64_t y;
    uint64_t z;
};

double get_distance(const position& p1, const position& p2) {
    return std::sqrt(static_cast<double>((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z)));
}

struct distance_info {
    uint32_t i;
    uint32_t j;
    double distance;
};

class union_find {
    std::vector<uint32_t> id;
    std::vector<uint32_t> sz;
    uint32_t count;
public:

    union_find(uint32_t size) {
        count = size;
        id.resize(size);
        sz.resize(size);
        for (uint32_t i = 0; i < size; i++) {
            id[i] = i;
            sz[i] = 1;
        }
    }

    uint32_t find(uint32_t x) {
        while (id[x] != x) {
            x = id[x];
        }
        return x;
    }

    bool connected(uint32_t x, uint32_t y) {
        return find(x) == find(y);
    }

    uint32_t get_count() {
        return count;
    }

    void unite(uint32_t x, uint32_t y) {
        uint32_t root_x = find(x);
        uint32_t root_y = find(y);
        if (root_x != root_y) {
            if (sz[root_x] > sz[root_y]) {
                id[root_y] = root_x;
                sz[root_x] += sz[root_y];
            }
            else {
                id[root_x] = root_y;
                sz[root_y] += sz[root_x];
            }
            count--;
        }
    }
};

int main() {
    std::string input;
    std::vector<position> positions;
    while (std::getline(std::cin, input)) {
        uint64_t x, y, z;
        std::regex regex(R"((\d+),(\d+),(\d+))");
        std::smatch match;
        if (std::regex_match(input, match, regex)) {
            x = std::stoul(match[1]);
            y = std::stoul(match[2]);
            z = std::stoul(match[3]);
        }
        positions.push_back(position{x, y, z});
    }

    std::vector<distance_info> distance_infos;
    for (uint32_t i = 0; i < positions.size(); i++) {
        for (uint32_t j = i + 1; j < positions.size(); j++) {
            distance_infos.push_back(distance_info{i, j, get_distance(positions[i], positions[j])});
        }
    }

    auto cmp = [](const distance_info& a, const distance_info& b) {
        return a.distance > b.distance;
    };
    std::make_heap(distance_infos.begin(), distance_infos.end(), cmp);
    union_find uf(positions.size());
    uint32_t count = 0;
    while (count < 1000) {
        auto min_distance_info = distance_infos.front();
        std::pop_heap(distance_infos.begin(), distance_infos.end(), cmp);
        distance_infos.pop_back();
        if (!uf.connected(min_distance_info.i, min_distance_info.j)) {
            uf.unite(min_distance_info.i, min_distance_info.j);
        }
        count++;
    }
    std::unordered_map<uint32_t, uint32_t> count_map;
    for (uint32_t i = 0; i < positions.size(); i++) {
        count_map[uf.find(i)]++;
    }
    std::vector<uint32_t> count_vec;
    for (const auto& count : count_map) {
        count_vec.push_back(count.second);
    }
    std::sort(count_vec.begin(), count_vec.end(), std::greater<uint32_t>());
    uint32_t res_a = 1;
    for (uint32_t i = 0; i < 3; i++) {
        res_a *= count_vec[i];
    }
    std::cout << res_a << std::endl;

    uint32_t last_merged_i, last_merged_j;
    while (uf.get_count() > 1) {
        auto min_distance_info = distance_infos.front();
        std::pop_heap(distance_infos.begin(), distance_infos.end(), cmp);
        distance_infos.pop_back();
        if (!uf.connected(min_distance_info.i, min_distance_info.j)) {
            uf.unite(min_distance_info.i, min_distance_info.j);
            last_merged_i = min_distance_info.i;
            last_merged_j = min_distance_info.j;
        }
    }
    std::cout << positions[last_merged_i].x * positions[last_merged_j].x << "\n";
    return 0;
}