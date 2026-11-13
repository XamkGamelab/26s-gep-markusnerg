#pragma once

#include "renderer2d.hpp"
#include "scene.hpp"

#include <SDL3/SDL.h>

#include <string>

class Game
{
public:
    bool init();
    void run();
    void shutdown();

private:
    bool initWindowAndContext();
    bool initGlad();
    void buildScene();
    void demonstrateSystems();

    void processEvents();
    void update(float deltaTime);
    void updateCamera(float deltaTime);
    void render();

    SDL_Window*   m_window    = nullptr;
    SDL_GLContext m_glContext = nullptr;

    Renderer2D m_renderer;
    Scene      m_scene{ "MainScene" };

    std::string m_assetDirectory;
    std::string m_facePath;

    bool m_isRunning = false;

    static constexpr int   kDefaultWidth  = 1280;
    static constexpr int   kDefaultHeight = 720;
    static constexpr float kCameraSpeed   = 3.0f;
};
