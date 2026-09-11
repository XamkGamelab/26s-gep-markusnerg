#pragma once

#include <SDL3/SDL.h>

// Sovelluksen elinkaari: init() -> run() -> shutdown().
class Game
{
public:
    // Alustaa SDL:n ja luo ikkunan. Palauttaa false, jos jokin epäonnistui.
    bool init();

    // Pääsilmukka. Palaa vasta kun poistumisehto täyttyy.
    void run();

    // Vapauttaa resurssit. Turvallinen kutsua vaikka init() olisi epäonnistunut.
    void shutdown();

private:
    void handleEvent(const SDL_Event& event);
    void onKeyDown(SDL_Keycode key);
    void onWindowResized();
    void render();

    SDL_Window* m_window = nullptr;
    bool m_isRunning = false;

    // Nykyinen taustaväri. Alkutila on tumma harmaa.
    Uint8 m_red   = 30;
    Uint8 m_green = 30;
    Uint8 m_blue  = 30;

    static constexpr int kDefaultWidth  = 1280;
    static constexpr int kDefaultHeight = 720;
};
