#include "game.hpp"
#include "inputmanager.hpp"
#include "time.hpp"

#include <glad/gl.h>

#include <iostream>
#include <string>

bool Game::init()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!initWindowAndContext())
    {
        return false;
    }

    if (!initGlad())
    {
        return false;
    }

    const char* basePath = SDL_GetBasePath();
    if (basePath == nullptr)
    {
        std::cerr << "SDL_GetBasePath failed: " << SDL_GetError() << std::endl;
        return false;
    }

    const std::string shaderDirectory = std::string(basePath) + "assets/shaders/";

    if (!m_renderer.Init(shaderDirectory))
    {
        std::cerr << "Renderer2D initialization failed" << std::endl;
        return false;
    }

    m_renderer.SetViewport(kDefaultWidth, kDefaultHeight);

    m_isRunning = true;

    std::cout << "Game initialized. Window " << kDefaultWidth
              << "x" << kDefaultHeight << std::endl;
    std::cout << "ESC = quit" << std::endl;

    return true;
}

bool Game::initWindowAndContext()
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    m_window = SDL_CreateWindow(
        "GEP 26S",
        kDefaultWidth,
        kDefaultHeight,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (m_window == nullptr)
    {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        return false;
    }

    m_glContext = SDL_GL_CreateContext(m_window);
    if (m_glContext == nullptr)
    {
        std::cerr << "SDL_GL_CreateContext failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
        return false;
    }

    if (!SDL_GL_MakeCurrent(m_window, m_glContext))
    {
        std::cerr << "SDL_GL_MakeCurrent failed: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_GL_SetSwapInterval(1);

    return true;
}

bool Game::initGlad()
{
    const int version = gladLoadGL(reinterpret_cast<GLADloadfunc>(SDL_GL_GetProcAddress));

    if (version == 0)
    {
        std::cerr << "gladLoadGL failed: could not load OpenGL function pointers"
                  << std::endl;

        SDL_GL_DestroyContext(m_glContext);
        m_glContext = nullptr;

        SDL_DestroyWindow(m_window);
        m_window = nullptr;

        SDL_Quit();
        return false;
    }

    std::cout << "GLAD loaded OpenGL "
              << GLAD_VERSION_MAJOR(version) << "."
              << GLAD_VERSION_MINOR(version) << std::endl;

    const GLubyte* renderer  = glGetString(GL_RENDERER);
    const GLubyte* glVersion = glGetString(GL_VERSION);

    if (renderer != nullptr && glVersion != nullptr)
    {
        std::cout << "Renderer: " << reinterpret_cast<const char*>(renderer) << std::endl;
        std::cout << "Version:  " << reinterpret_cast<const char*>(glVersion) << std::endl;
    }

    return true;
}

void Game::run()
{
    while (m_isRunning)
    {
        Time::Instance().Tick();

        processEvents();
        update(Time::Instance().GetDeltaTime());
        render();
    }
}

void Game::shutdown()
{
    m_renderer.Shutdown();

    if (m_glContext != nullptr)
    {
        SDL_GL_DestroyContext(m_glContext);
        m_glContext = nullptr;
    }

    if (m_window != nullptr)
    {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }

    SDL_Quit();
    std::cout << "Game shut down cleanly." << std::endl;
}

void Game::processEvents()
{
    InputManager::Instance().Update();

    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            std::cout << "SDL_EVENT_QUIT received." << std::endl;
            m_isRunning = false;
        }
        else if (event.type == SDL_EVENT_WINDOW_RESIZED)
        {
            int width  = 0;
            int height = 0;
            SDL_GetWindowSize(m_window, &width, &height);
            std::cout << "Window resized to " << width << "x" << height << std::endl;

            m_renderer.SetViewport(width, height);
        }

        InputManager::Instance().ProcessEvent(event);
    }
}

void Game::update(float deltaTime)
{
    (void)deltaTime;

    const InputManager& input = InputManager::Instance();

    if (input.IsKeyDown(SDLK_ESCAPE))
    {
        std::cout << "Escape pressed." << std::endl;
        m_isRunning = false;
    }
}

void Game::render()
{
    m_renderer.Begin();
    m_renderer.DrawQuad();
    m_renderer.End();

    SDL_GL_SwapWindow(m_window);
}
