#pragma once
#include <vector>
using namespace std;

const int window_largeur = 1920;
const int window_height = 1000;

typedef struct {
    float x;
    float y;
} Point;  // Point ou vecteur2D qui repésante un point (x,y)

typedef struct {
    float x, y;
} Vec2;   // Vecteur de base 2

typedef struct {
    float x,y;       // position
    float vx, vy;    // vitesse
    float ax, ay;    // accélération
    float mass;      // masse
    int affectedByGravity; // 1 = sensible à la gravité, 0 = non
} Physics_obj; // propriété Physics d'un objet

typedef struct {
    float radius;
    float frition;
    Physics_obj phys;
} Circle; // Un cercle avec c'est propriété Physics et sont rayons

typedef struct {
    float frition;
    vector<Point> points;
    Physics_obj phys;
} Polygone; // Polygone de base contient un tableaux de point de ces coté et les c'est propriété Physics
