#pragma once

#include "camera.hpp"
#include "renderer2d.hpp"
#include "texture.hpp"
#include "transform.hpp"

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
    void initScene();

    void processEvents();
    void update(float deltaTime);
    void updateCamera(float deltaTime);
    void render();

    SDL_Window*   m_window    = nullptr;
    SDL_GLContext m_glContext = nullptr;

    Renderer2D m_renderer;
    Camera     m_camera;
    Texture    m_texture;

    Transform m_leftQuad;
    Transform m_rightQuad;
    Transform m_smallQuad;

    float m_elapsed = 0.0f;

    bool m_isRunning = false;

    static constexpr int   kDefaultWidth  = 1280;
    static constexpr int   kDefaultHeight = 720;
    static constexpr float kCameraSpeed   = 3.0f;
    static constexpr float kRotationSpeed = 60.0f;
};
