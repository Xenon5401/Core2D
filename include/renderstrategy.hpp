#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "entity.hpp"

class RenderStrategy {
public:
    void Circle_Render(SDL_Renderer* renderer, const std::vector<Circle>& tab_Circle) const;
    void Polygone_render(SDL_Renderer* renderer, const std::vector<Polygone>& tab_Polygone) const; // rendu d'un polygone de base
};