#include "renderstrategy.hpp"


void RenderStrategy::Circle_Render(SDL_Renderer* renderer, const std::vector<Circle>& tab_Circle) const {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // couleur rouge

        for (const auto& e : tab_Circle) {
            int cx = static_cast<int>(e.phys.x);
            int cy = static_cast<int>(e.phys.y);
            int r  = static_cast<int>(e.radius);

            int x = 0;
            int y = r;
            int d = 3 - 2 * r;

            auto drawLineSym = [&](int xc, int yc, int x, int y) {
                // Tracer des lignes horizontales pour remplir le cercle
                SDL_RenderDrawLine(renderer, xc - x, yc + y, xc + x, yc + y);
                SDL_RenderDrawLine(renderer, xc - x, yc - y, xc + x, yc - y);
                SDL_RenderDrawLine(renderer, xc - y, yc + x, xc + y, yc + x);
                SDL_RenderDrawLine(renderer, xc - y, yc - x, xc + y, yc - x);
            };

            while (y >= x) {
                drawLineSym(cx, cy, x, y);
                x++;
                if (d > 0) {
                    y--;
                    d = d + 4 * (x - y) + 10;
                } else {
                    d = d + 4 * x + 6;
                }
            }
        }
    }

void RenderStrategy::Polygone_render (SDL_Renderer* renderer, const std::vector<Polygone>& tab_Polygone) const { // rendu d'un polygone de base
        for (const auto& e : tab_Polygone) {
            size_t n = e.points.size();
            if (n < 3) continue; // impossible de dessiner sans au moins 3 points

            // Créer tous les sommets
            std::vector<SDL_Vertex> verts;
            verts.reserve(n);
            for (const auto& f : e.points) {
                SDL_Vertex v;
                v.position = { e.phys.x + f.x, e.phys.y + f.y };;
                v.color = {255, 255, 0, 255};  // couleur
                v.tex_coord = {0, 0};
                verts.push_back(v);
            }

            // Définir les indices de triangulation
            std::vector<int> indices;
            if (n == 4) {
                // carré ou rectangle simplifi le calcule évite triangulation inutile
                indices = {0, 1, 2, 0, 2, 3};
            } else {
                // triangulation simple (fan)
                for (size_t i = 1; i + 1 < n; ++i) {
                    indices.push_back(0);
                    indices.push_back(static_cast<int>(i));
                    indices.push_back(static_cast<int>(i + 1));
                }
            }

            // Rendu du polygone
            SDL_RenderGeometry(renderer, nullptr, verts.data(), static_cast<int>(verts.size()), indices.data(), static_cast<int>(indices.size()));
        }
    }