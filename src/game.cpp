#include "game.hpp"

#include <iostream>

bool Game::init()
{
    // SDL3:ssa SDL_Init palauttaa boolin, ei int:iä kuten SDL2:ssa.
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return false;
    }

    // SDL3:n SDL_CreateWindow ei enää ota x/y-koordinaatteja.
    m_window = SDL_CreateWindow(
        "GEP 26S",
        kDefaultWidth,
        kDefaultHeight,
        SDL_WINDOW_RESIZABLE);

    if (m_window == nullptr)
    {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        return false;
    }

    m_isRunning = true;

    std::cout << "Game initialized. Window " << kDefaultWidth
              << "x" << kDefaultHeight << std::endl;
    std::cout << "WASD = color, ESC = quit" << std::endl;

    render();
    return true;
}

void Game::run()
{
    SDL_Event event;

    while (m_isRunning)
    {
        // Tyhjennetään koko tapahtumajono ennen piirtoa.
        while (SDL_PollEvent(&event))
        {
            handleEvent(event);
        }

        render();

        // Pieni tauko, jottei silmukka polta prosessoria täysillä.
        SDL_Delay(16);
    }
}

void Game::shutdown()
{
    if (m_window != nullptr)
    {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }

    SDL_Quit();
    std::cout << "Game shut down cleanly." << std::endl;
}

void Game::handleEvent(const SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_EVENT_QUIT:
        std::cout << "SDL_EVENT_QUIT received." << std::endl;
        m_isRunning = false;
        break;

    case SDL_EVENT_KEY_DOWN:
        // SDL3: näppäinkoodi on event.key.key (SDL2:ssa event.key.keysym.sym).
        // event.key.repeat suodattaa pois automaattitoiston.
        if (!event.key.repeat)
        {
            onKeyDown(event.key.key);
        }
        break;

    case SDL_EVENT_WINDOW_RESIZED:
        onWindowResized();
        break;

    default:
        break;
    }
}

void Game::onKeyDown(SDL_Keycode key)
{
    switch (key)
    {
    case SDLK_ESCAPE:
        std::cout << "Escape pressed." << std::endl;
        m_isRunning = false;
        break;

    case SDLK_W:
        m_red = 220; m_green = 40;  m_blue = 40;
        std::cout << "Color: RED" << std::endl;
        break;

    case SDLK_A:
        m_red = 40;  m_green = 200; m_blue = 60;
        std::cout << "Color: GREEN" << std::endl;
        break;

    case SDLK_S:
        m_red = 50;  m_green = 90;  m_blue = 220;
        std::cout << "Color: BLUE" << std::endl;
        break;

    case SDLK_D:
        m_red = 230; m_green = 200; m_blue = 50;
        std::cout << "Color: YELLOW" << std::endl;
        break;

    default:
        break;
    }
}

void Game::onWindowResized()
{
    int width  = 0;
    int height = 0;

    if (!SDL_GetWindowSize(m_window, &width, &height))
    {
        std::cerr << "SDL_GetWindowSize failed: " << SDL_GetError() << std::endl;
        return;
    }

    std::cout << "Window resized to " << width << "x" << height << std::endl;

    // Vanha ikkunapinta on koon muutoksessa mitätöity. Seuraava
    // SDL_GetWindowSurface render()-metodissa hakee uuden, oikean kokoisen.
    render();
}

void Game::render()
{
    // Pinta on haettava joka kerta uudelleen: SDL vapauttaa vanhan,
    // kun ikkunan kokoa muutetaan.
    SDL_Surface* surface = SDL_GetWindowSurface(m_window);

    if (surface == nullptr)
    {
        std::cerr << "SDL_GetWindowSurface failed: " << SDL_GetError() << std::endl;
        return;
    }

    // Muunnetaan RGB-arvo pinnan omaan pikseliformaattiin.
    const Uint32 color = SDL_MapSurfaceRGB(surface, m_red, m_green, m_blue);

    // nullptr rect = täytetään koko pinta.
    SDL_FillSurfaceRect(surface, nullptr, color);

    // Siirretään framebufferin sisältö näytölle.
    SDL_UpdateWindowSurface(m_window);
}
