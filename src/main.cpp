#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <iostream>
#include "window.hpp"
#include "renderstrategy.hpp"
#include "spacetable.hpp"
#include "objectsystem.hpp"
#include "physicsystem.hpp"

using namespace std;

class Scene {
public:
    std::vector<Circle> circles;
    std::vector<Polygone> polygones;
    RenderStrategy renderer;
    PhysicsSystem physics;
    ObjectSystem object;
};

int main() {
    string title = "Ma Fenêtre SDL";
    Window win(window_largeur, window_height, title); // création de la fenêtre

    if (!win.isValid()) {
        cerr << "Impossible de créer la fenêtre ou le renderer !" << endl;
        return 1;
    }

    bool quit = false;
    SDL_Event event;

    Scene main;

    // Créer la grille spatiale pour les collisions
    space_table grid;
    grid.cellSize = 50.0f; // taille de cellule à ajuster selon tes cercles
    
    // Ajouter quelques polygones pour tester le rendu
    main.object.Polygone_add(main.polygones, 200, 300, 10, 0, 0, 0, 1.0f, 1);
    main.object.Polygone_add(main.polygones, 200, 600, -10, 0, 0, 0, 1.0f, 1);

    while (!quit) {
        // Gestion des événements
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                }
                if (event.key.keysym.sym == SDLK_a) {
                    main.object.Circle_add(main.circles, rand() % window_largeur, 0, rand() % 30 + 10, 0, 0);
                }
                if (event.key.keysym.sym == SDLK_8) {
                    main.circles[0].phys.vy = -50.0f; // donner un coup de saut au premier cercle
                }
            }
        }

        // On change la couleur de fond
        win.clear(0, 0, 0);
        
        // Physique + collisions optimisées avec space_table
        main.physics.Circle_Render_phy(main.circles, grid);
        main.physics.Polygone_Render_phy(main.polygones);

        // Rendu
        main.renderer.Circle_Render(win.renderer, main.circles);
        main.renderer.Polygone_render(win.renderer, main.polygones);

        win.present();

        SDL_Delay(16); // ~60 FPS
    }
    return 0;
}