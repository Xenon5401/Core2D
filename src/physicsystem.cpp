#include "physicsystem.hpp"

void PhysicsSystem::Circle_Render_phy(std::vector<Circle> &tab_Circle, space_table &grid)
{
    // vide la grille et on réinsère toutes les entités
    grid.clear();
    for (auto &e : tab_Circle)
    {
        grid.insert(&e); // on passe un pointeur sûr
    }

    // On met à jour la physique de chaque cercle
    for (auto &e : tab_Circle)
    {
        if (e.phys.affectedByGravity)
        {
            e.phys.ay = gravity;
            e.phys.vy += e.phys.ay * e.phys.mass;
            e.phys.vx += e.phys.ax;
            e.phys.x += e.phys.vx;
            e.phys.y += e.phys.vy;

            // Collision avec les bords
            if (e.phys.y + e.radius > window_height)
            {
                e.phys.y = window_height - e.radius;
                e.phys.vy *= -0.8f;
            }

            if (e.phys.x + e.radius > window_largeur)
            {
                e.phys.x = window_largeur - e.radius;
                e.phys.vx *= -0.8f;
            }
            else if (e.phys.x - e.radius < 0)
            {
                e.phys.x = e.radius;
                e.phys.vx *= -0.8f;
            }

            // Collision avec les autres cercles via la grille
            auto neighbors = grid.query({e.phys.x, e.phys.y}, e.radius * 2.0f); // chercher les voisins proches
            // ps c est vector avec tout ces compariote a proximitée

            for (auto *a : neighbors)
            {
                if (a == &e)
                    continue; // ne pas se comparer avec soi-même

                float dx = e.phys.x - a->phys.x;
                float dy = e.phys.y - a->phys.y;
                float dist2 = dx * dx + dy * dy;
                float minDist = e.radius + a->radius;

                if (dist2 < minDist * minDist)
                {
                    float dist = sqrtf(dist2);
                    if (dist < 0.001f)
                        dist = 0.001f;

                    float nx = dx / dist;
                    float ny = dy / dist;
                    float overlap = 0.5f * (minDist - dist);

                    // Correction de position
                    e.phys.x += nx * overlap;
                    e.phys.y += ny * overlap;
                    a->phys.x -= nx * overlap;
                    a->phys.y -= ny * overlap;

                    // Réponse vitesse (rebond)
                    float k = 0.9f;
                    float va = a->phys.vx * nx + a->phys.vy * ny;
                    float ve = e.phys.vx * nx + e.phys.vy * ny;
                    float impulse = (va - ve) * k;

                    a->phys.vx -= impulse * nx;
                    a->phys.vy -= impulse * ny;
                    e.phys.vx += impulse * nx;
                    e.phys.vy += impulse * ny;
                }
            }
        }
    }
}

void PhysicsSystem::Polygone_Render_phy(std::vector<Polygone> &tab_Polygone)
{
    for (auto &e : tab_Polygone)
    {
        if (!e.phys.affectedByGravity)
            continue;

        e.phys.ay = gravity;
        e.phys.vy += e.phys.ay * e.phys.mass;
        e.phys.vx += e.phys.ax;
        e.phys.x += e.phys.vx;
        e.phys.y += e.phys.vy;

        // Calculer la bas de la box du polygone
        float max_y = e.phys.y;
        float min_y = e.phys.y;
        float max_x = e.phys.x;
        float min_x = e.phys.x;

        for (auto &a : e.points)
        {
            float global_y = e.phys.y + a.y;
            float global_x = e.phys.x + a.x;
            if (global_y > max_y)
                max_y = global_y;
            if (global_y < min_y)
                min_y = global_y;
            if (global_x > max_x)
                max_x = global_x;
            if (global_x < min_x)
                min_x = global_x;
        }

        // Collision bas
        if (max_y > window_height)
        {
            e.phys.vy *= -0.8f;
            e.phys.y -= (max_y - window_height);
        }

        // Collision haut
        if (min_y < 0)
        {
            e.phys.vy *= -0.8f;
            e.phys.y -= min_y;
        }

        // Collision gauche
        if (min_x < 0)
        {
            e.phys.vx *= -0.8f;
            e.phys.x -= min_x;
        }

        // Collision droite
        if (max_x > window_largeur)
        {
            e.phys.vx *= -0.8f;
            e.phys.x -= (max_x - window_largeur);
        }

        for (auto &e1 : tab_Polygone) // vérifier les collisions avec les autres polygones
        {
            if (&e1 == &e)
                continue; // ne pas se comparer avec soi-même

            // Construire les polygones en coordonnées globales
            std::vector<Point> polyA;
            std::vector<Point> polyB;
            polyA.reserve(e.points.size());
            polyB.reserve(e1.points.size());

            for (auto &p : e.points)
                polyA.push_back(Point{e.phys.x + p.x, e.phys.y + p.y});
            for (auto &q : e1.points)
                polyB.push_back(Point{e1.phys.x + q.x, e1.phys.y + q.y});

            // Utiliser GJK (convex polygons) pour détecter une collision
            bool coll = gjk_collision(polyA, polyB);
            if (coll)
            {
                // Simple réponse de collision non physique (préserve stabilité):
                // On inverse légèrement les vitesses pour simuler un rebond et on décale
                // légèrement les objets pour éviter qu'ils restent coincés.
                e.phys.vx *= -0.8f;
                e.phys.vy *= -0.8f;
                e1.phys.vx *= -0.8f;
                e1.phys.vy *= -0.8f;

                // Décalage minimal le long du vecteur entre centres
                float dx = (e.phys.x) - (e1.phys.x);
                float dy = (e.phys.y) - (e1.phys.y);
                float d2 = dx * dx + dy * dy;
                if (d2 < 1e-6f)
                {
                    // très proche ou même centre identique: appliquer un petit offset arbitraire
                    e.phys.x += 0.1f;
                    e.phys.y += 0.1f;
                    e1.phys.x -= 0.1f;
                    e1.phys.y -= 0.1f;
                }
                else
                {
                    float d = sqrtf(d2);
                    float nx = dx / d;
                    float ny = dy / d;
                    // nudge chacun d'une très petite distance
                    float nudge = 0.5f;
                    e.phys.x += nx * nudge;
                    e.phys.y += ny * nudge;
                    e1.phys.x -= nx * nudge;
                    e1.phys.y -= ny * nudge;
                }
            }
        }
    };
}

bool PhysicsSystem::check_in_trig(Point p, Point t0, Point t1, Point t2)
{
    float R1 = (t1.x - t0.x) * (p.y - t0.y) - (t1.y - t0.y) * (p.x - t0.x);
    float R2 = (t2.x - t1.x) * (p.y - t1.y) - (t2.y - t1.y) * (p.x - t1.x);
    float R3 = (t0.x - t2.x) * (p.y - t2.y) - (t0.y - t2.y) * (p.x - t2.x);

    // Si tous les signes sont identiques ou nuls → le point est dedans
    return (R1 >= 0 && R2 >= 0 && R3 >= 0) || (R1 <= 0 && R2 <= 0 && R3 <= 0); // deux possibliter le test en sens horaire ou antihoraire
}

Point PhysicsSystem::get_max_dir(const vector<Point> &shape, Vec2 dir)
{
    float max, t;
    Point r = shape[0]; // dans le cas ou il serait déja le max
    max = scale({shape[0].x, shape[0].y}, dir);
    for (auto &e : shape)
    {
        t = scale({e.x, e.y}, dir);
        if (t > max)
        {
            max = t;
            r = e;
        }
    }
    return r;
}

// --- GJK helpers (file-local) ---

static inline float dot(const Vec2 &a, const Vec2 &b)
{
    return a.x * b.x + a.y * b.y;
}

static inline Vec2 vec_sub(const Point &a, const Point &b)
{
    return Vec2{a.x - b.x, a.y - b.y};
}

static inline Point point_sub(const Point &a, const Point &b)
{
    return Point{a.x - b.x, a.y - b.y};
}

// triple product: (a x b) x c but implemented to return a vector in 2D
static inline Vec2 tripleProduct(const Vec2 &a, const Vec2 &b, const Vec2 &c)
{
    // returns b * (a.c) - c * (a.b)
    float ac = a.x * c.x + a.y * c.y;
    float ab = a.x * b.x + a.y * b.y;
    return Vec2{b.x * ac - c.x * ab, b.y * ac - c.y * ab};
}

// support point in Minkowski difference A - B along direction d
static Point support(const vector<Point> &A, const vector<Point> &B, Vec2 d)
{
    // farthest point in A along d
    float bestA = -INFINITY;
    Point pA = A[0];
    for (auto &pt : A)
    {
        float v = pt.x * d.x + pt.y * d.y;
        if (v > bestA)
        {
            bestA = v;
            pA = pt;
        }
    }

    // farthest point in B along -d
    Vec2 nd = Vec2{-d.x, -d.y};
    float bestB = -INFINITY;
    Point pB = B[0];
    for (auto &pt : B)
    {
        float v = pt.x * nd.x + pt.y * nd.y;
        if (v > bestB)
        {
            bestB = v;
            pB = pt;
        }
    }

    return point_sub(pA, pB);
}

// Handle simplex; returns true if origin is contained
static bool handleSimplex(vector<Point> &simplex, Vec2 &dir)
{
    if (simplex.size() == 2)
    {
        // line segment: simplex[0] = B, simplex[1] = A (A is newest)
        Point A = simplex[1];
        Point B = simplex[0];
        Vec2 AB = vec_sub(B, A);
        Vec2 AO = Vec2{-A.x, -A.y};

        // perpendicular to AB towards origin
        Vec2 perp = tripleProduct(AB, AO, AB);
        // If perp is near zero (collinear), pick a perpendicular vector
        if (fabs(perp.x) < 1e-6f && fabs(perp.y) < 1e-6f)
        {
            perp = Vec2{-AB.y, AB.x};
            if (dot(perp, AO) < 0)
                perp = Vec2{ -perp.x, -perp.y };
        }

        dir = perp;
        return false;
    }

    // triangle case: simplex[0]=C, simplex[1]=B, simplex[2]=A (A newest)
    Point A = simplex[2];
    Point B = simplex[1];
    Point C = simplex[0];

    Vec2 AO = Vec2{-A.x, -A.y};
    Vec2 AB = vec_sub(B, A);
    Vec2 AC = vec_sub(C, A);

    // perpendicular to AB towards origin
    Vec2 ABperp = tripleProduct(AC, AB, AB);
    // perpendicular to AC towards origin
    Vec2 ACperp = tripleProduct(AB, AC, AC);

    if (dot(ABperp, AO) > 0)
    {
        // remove C
        simplex.erase(simplex.begin()); // remove C (index 0)
        dir = ABperp;
        return false;
    }
    else if (dot(ACperp, AO) > 0)
    {
        // remove B
        simplex.erase(simplex.begin() + 1); // remove B (index 1)
        dir = ACperp;
        return false;
    }

    // Origin is inside triangle
    return true;
}

bool PhysicsSystem::gjk_collision(const std::vector<Point> &A, const std::vector<Point> &B)
{
    if (A.empty() || B.empty())
        return false;

    // initial direction: from centroid of B to centroid of A
    Vec2 dir{0,0};
    for (auto &p : A) { dir.x += p.x; dir.y += p.y; }
    dir.x /= (float)A.size(); dir.y /= (float)A.size();
    Vec2 cb{0,0};
    for (auto &p : B) { cb.x += p.x; cb.y += p.y; }
    cb.x /= (float)B.size(); cb.y /= (float)B.size();
    dir.x = dir.x - cb.x; dir.y = dir.y - cb.y;
    if (dir.x == 0 && dir.y == 0)
        dir = Vec2{1, 0};

    vector<Point> simplex;

    // first support
    Point p = support(A, B, dir);
    simplex.push_back(p);

    // direction towards origin
    dir = Vec2{-p.x, -p.y};

    const int maxIter = 64;
    for (int iter = 0; iter < maxIter; ++iter)
    {
        Point newPt = support(A, B, dir);

        // If the new point didn't pass the origin in direction dir => no collision
        if ((newPt.x * dir.x + newPt.y * dir.y) <= 0)
            return false;

        // add to simplex
        simplex.push_back(newPt);

        // handle simplex
        if (handleSimplex(simplex, dir))
            return true;
        
        // otherwise loop with updated dir
    }

    // reached iteration limit, assume no collision
    return false;
}

