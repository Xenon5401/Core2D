#include "entity.hpp"
#include <vector>

class ObjectSystem{
    public:
    void Circle_add(vector<Circle>& tab_Circle, float x, float y, float radius,
                    float vx = 0, float vy = 0,
                    float ax = 0, float ay = 0,
                    float mass = 1.0f, int affectedByGravity = 1);
    
    void Polygone_add(vector<Polygone>& tab_Polygone, float x, float y, 
                     float vx = 0, float vy = 0,
                     float ax = 0, float ay = 0,
                     float mass = 1.0f, int affectedByGravity = 1);
};