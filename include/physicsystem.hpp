#include <vector>
#include "entity.hpp"
#include "spacetable.hpp"
#include <iostream>

class PhysicsSystem{
    public:

    void Circle_Render_phy(std::vector<Circle>& tab_Circle, space_table& grid);
    void Polygone_Render_phy(std::vector<Polygone>& tab_Polygone);

    private:
    float gravity = 1;
    
    bool check_in_trig(Point p, Point t0, Point t1, Point t2);

    float cross(const Vec2& a, const Vec2& b) {
        return a.x * b.y - a.y * b.x;
    }

    Point get_max_dir(const vector<Point> &shape, Vec2 dir);

    float scale(Vec2 a, Vec2 b)
    {
        return a.x * b.x + a.y * b.y;
    }

    // GJK collision detection (convex polygons)
    bool gjk_collision(const std::vector<Point> &A, const std::vector<Point> &B);
};