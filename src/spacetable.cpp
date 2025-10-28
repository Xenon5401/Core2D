#include "spacetable.hpp"


// fonction de hash des cordoneer pour crée la key
long long space_table::hash(int x, int y) const {
    return ((long long)x << 32) ^ (long long)(y & 0xffffffff); // Bitwise pair hashing
};

    // fonction de retour de coordonée retourne 2 int avec pair
std::pair<int, int> space_table::getCellCoords(float x, float y) const {
    return {
        static_cast<int>(std::floor(x / cellSize)),
        static_cast<int>(std::floor(y / cellSize))
    };
};

    // pour ajouter au unordered_map avec un hash assosier 
void space_table::insert( Circle* e) {
    auto [cx, cy] = getCellCoords(e -> phys.x,e -> phys.y);
    cells[hash(cx, cy)].push_back(e);
};

std::vector<Circle*> space_table::query(const Vec2& pos, float radius) const {
    std::vector<Circle*> result;
    int minX = std::floor((pos.x - radius) / cellSize);
    int maxX = std::floor((pos.x + radius) / cellSize);
    int minY = std::floor((pos.y - radius) / cellSize);
    int maxY = std::floor((pos.y + radius) / cellSize);

    for (int x = minX; x <= maxX; ++x) {
        for (int y = minY; y <= maxY; ++y) {
            auto it = cells.find(hash(x, y));
            if (it != cells.end()) {
                for (auto* e : it->second)
                    result.push_back(e);
            }
        }
    }
    return result;
}

void space_table::clear() { cells.clear(); } // pour clear la unordered_map