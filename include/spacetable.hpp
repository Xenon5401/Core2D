#pragma once
#include "entity.hpp"
#include <unordered_map>
#include <cmath>


struct space_table
{
    float cellSize;
    std::unordered_map<long long, std::vector<Circle*>> cells;

    long long hash(int x, int y) const;

    std::pair<int, int> getCellCoords(float x, float y) const;

    void insert( Circle* e);

    std::vector<Circle*> query(const Vec2& pos, float radius) const;

    void clear();
};
