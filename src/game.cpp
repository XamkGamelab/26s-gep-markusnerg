#include "game.hpp"
#include "inputmanager.hpp"

#include <SDL3_image/SDL_image.h>

#include <iostream>
#include <string>

bool Game::init()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!SDL_CreateWindowAndRenderer(
            "GEP 26S",
            kDefaultWidth,
            kDefaultHeight,
            SDL_WINDOW_RESIZABLE,
            &m_window,
            &m_renderer))
    {
        std::cerr << "SDL_CreateWindowAndRenderer failed: "
                  << SDL_GetError() << std::endl;
        return false;
    }

    const char* basePath = SDL_GetBasePath();
    if (basePath == nullptr)
    {
        std::cerr << "SDL_GetBasePath failed: " << SDL_GetError() << std::endl;
        return false;
    }

    const std::string imagePath = std::string(basePath) + "assets/awesomeface.png";

    m_texture = IMG_LoadTexture(m_renderer, imagePath.c_str());
    if (m_texture == nullptr)
    {
        std::cerr << "IMG_LoadTexture failed for " << imagePath
                  << ": " << SDL_GetError() << std::endl;
        return false;
    }

    if (!SDL_GetTextureSize(m_texture, &m_imageW, &m_imageH))
    {
        std::cerr << "SDL_GetTextureSize failed: " << SDL_GetError() << std::endl;
        return false;
    }

    m_imageX = (static_cast<float>(kDefaultWidth)  - m_imageW) * 0.5f;
    m_imageY = (static_cast<float>(kDefaultHeight) - m_imageH) * 0.5f;

    m_lastTicks = SDL_GetTicks();
    m_isRunning = true;

    std::cout << "Game initialized. Window " << kDefaultWidth
              << "x" << kDefaultHeight << std::endl;
    std::cout << "Image " << m_imageW << "x" << m_imageH
              << " loaded from " << imagePath << std::endl;
    std::cout << "WASD = move, ESC = quit" << std::endl;

    return true;
}

void Game::run()
{
    while (m_isRunning)
    {
        const Uint64 now       = SDL_GetTicks();
        const float  deltaTime = static_cast<float>(now - m_lastTicks) / 1000.0f;
        m_lastTicks = now;

        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::shutdown()
{
    if (m_texture != nullptr)
    {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }

    if (m_renderer != nullptr)
    {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
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

            clampImageToWindow();
        }

        InputManager::Instance().ProcessEvent(event);
    }
}

void Game::update(float deltaTime)
{
    const InputManager& input = InputManager::Instance();

    if (input.IsKeyDown(SDLK_ESCAPE))
    {
        std::cout << "Escape pressed." << std::endl;
        m_isRunning = false;
        return;
    }

    const float distance = kMoveSpeed * deltaTime;

    if (input.IsKeyPressed(SDLK_W)) { m_imageY -= distance; }
    if (input.IsKeyPressed(SDLK_S)) { m_imageY += distance; }
    if (input.IsKeyPressed(SDLK_A)) { m_imageX -= distance; }
    if (input.IsKeyPressed(SDLK_D)) { m_imageX += distance; }

    clampImageToWindow();

    if (input.IsMouseDown(SDL_BUTTON_LEFT))
    {
        std::cout << "Left mouse button down." << std::endl;
    }
    if (input.IsMouseUp(SDL_BUTTON_LEFT))
    {
        std::cout << "Left mouse button up." << std::endl;
    }
}

void Game::clampImageToWindow()
{
    int outputW = 0;
    int outputH = 0;

    if (!SDL_GetRenderOutputSize(m_renderer, &outputW, &outputH))
    {
        return;
    }

    const float maxX = static_cast<float>(outputW) - m_imageW;
    const float maxY = static_cast<float>(outputH) - m_imageH;

    if (m_imageX < 0.0f) { m_imageX = 0.0f; }
    if (m_imageY < 0.0f) { m_imageY = 0.0f; }
    if (m_imageX > maxX) { m_imageX = maxX; }
    if (m_imageY > maxY) { m_imageY = maxY; }
}

void Game::render()
{
    SDL_SetRenderDrawColor(m_renderer, 30, 30, 40, 255);
    SDL_RenderClear(m_renderer);

    const SDL_FRect destination{ m_imageX, m_imageY, m_imageW, m_imageH };

    SDL_RenderTexture(m_renderer, m_texture, nullptr, &destination);

    SDL_RenderPresent(m_renderer);
}
