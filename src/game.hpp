#pragma once

#include "renderer2d.hpp"
#include "texture.hpp"

#include <SDL3/SDL.h>

class Game
{
public:
    bool init();
    void run();
    void shutdown();

private:
    bool initWindowAndContext();
    bool initGlad();

    void processEvents();
    void update(float deltaTime);
    void render();

    SDL_Window*   m_window    = nullptr;
    SDL_GLContext m_glContext = nullptr;

    Renderer2D m_renderer;
    Texture    m_texture;

    bool m_isRunning = false;

    static constexpr int kDefaultWidth  = 1280;
    static constexpr int kDefaultHeight = 720;
};
