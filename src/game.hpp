#pragma once

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
    bool loadImage();

    void processEvents();
    void update(float deltaTime);
    void render();
    void clampImageToWindow();

    SDL_Window*   m_window    = nullptr;
    SDL_Renderer* m_renderer  = nullptr;
    SDL_Texture*  m_texture   = nullptr;
    SDL_GLContext m_glContext = nullptr;

    bool m_isRunning = false;

    float m_imageX = 0.0f;
    float m_imageY = 0.0f;
    float m_imageW = 0.0f;
    float m_imageH = 0.0f;

    static constexpr int   kDefaultWidth  = 1280;
    static constexpr int   kDefaultHeight = 720;
    static constexpr float kMoveSpeed     = 400.0f;
};
