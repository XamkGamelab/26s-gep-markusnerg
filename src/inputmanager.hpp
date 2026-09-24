#pragma once

#include <SDL3/SDL.h>

#include <unordered_set>

class InputManager
{
public:
    static InputManager& Instance();

    InputManager(const InputManager&)            = delete;
    InputManager& operator=(const InputManager&) = delete;
    InputManager(InputManager&&)                 = delete;
    InputManager& operator=(InputManager&&)      = delete;

    void Update();
    void ProcessEvent(const SDL_Event& event);

    bool IsKeyDown(SDL_Keycode key) const;
    bool IsKeyUp(SDL_Keycode key) const;
    bool IsKeyPressed(SDL_Keycode key) const;

    bool IsMouseDown(unsigned int mouseBtnIdx) const;
    bool IsMouseUp(unsigned int mouseBtnIdx) const;
    bool IsMousePressed(unsigned int mouseBtnIdx) const;

private:
    InputManager()  = default;
    ~InputManager() = default;

    std::unordered_set<SDL_Keycode> m_keysDown;
    std::unordered_set<SDL_Keycode> m_keysUp;
    std::unordered_set<SDL_Keycode> m_keysPressed;

    std::unordered_set<unsigned int> m_mouseDown;
    std::unordered_set<unsigned int> m_mouseUp;
    std::unordered_set<unsigned int> m_mousePressed;
};
