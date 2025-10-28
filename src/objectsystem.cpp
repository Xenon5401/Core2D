#include "objectsystem.hpp"

void ObjectSystem::Circle_add(vector<Circle>& tab_Circle, float x, float y, float radius,
                    float vx, float vy,
                    float ax, float ay,
                    float mass, int affectedByGravity){
        Circle c;
        c.radius = radius;
        c.phys.x = x;
        c.phys.y = y;
        c.phys.vx = vx;
        c.phys.vy = vy;
        c.phys.ax = ax;
        c.phys.ay = ay;
        c.phys.mass = mass;
        c.phys.affectedByGravity = affectedByGravity;
        tab_Circle.push_back(c);
    };
    
void ObjectSystem::Polygone_add(vector<Polygone>& tab_Polygone, float x, float y, 
                     float vx, float vy,
                     float ax, float ay,
                     float mass, int affectedByGravity){
    Polygone p;
    p.phys.x = x;
    p.phys.y = y;
    p.phys.vx = vx;
    p.phys.vy = vy;
    p.phys.ax = ax;
    p.phys.ay = ay;
    p.phys.mass = mass;
    p.phys.affectedByGravity = affectedByGravity;
    p.frition = 0.8f;
        
    // c est plus les dimenson de base car après on rajoute l offset dans le render
    p.points.push_back({0, 0});
    p.points.push_back({0, 50});
    p.points.push_back({50, 50});
    p.points.push_back({50, 0});

    tab_Polygone.push_back(p);
}