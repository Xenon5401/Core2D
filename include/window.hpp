#pragma once
#include <SDL2/SDL.h>
#include <string>

class Window {
public:
    SDL_Window* window;
    SDL_Renderer* renderer;
    int width, height;

    Window(int w, int h, const std::string& title);
    ~Window();

    bool isValid() const;
    void clear(Uint8 r, Uint8 g, Uint8 b);
    void present();
};
