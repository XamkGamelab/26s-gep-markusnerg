#include "inputmanager.hpp"

InputManager& InputManager::Instance()
{
    static InputManager instance;
    return instance;
}

void InputManager::Update()
{
    m_keysDown.clear();
    m_keysUp.clear();
    m_mouseDown.clear();
    m_mouseUp.clear();
}

void InputManager::ProcessEvent(const SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_EVENT_KEY_DOWN:
        if (!event.key.repeat)
        {
            m_keysDown.insert(event.key.key);
            m_keysPressed.insert(event.key.key);
        }
        break;

    case SDL_EVENT_KEY_UP:
        m_keysUp.insert(event.key.key);
        m_keysPressed.erase(event.key.key);
        break;

    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        m_mouseDown.insert(event.button.button);
        m_mousePressed.insert(event.button.button);
        break;

    case SDL_EVENT_MOUSE_BUTTON_UP:
        m_mouseUp.insert(event.button.button);
        m_mousePressed.erase(event.button.button);
        break;

    default:
        break;
    }
}

bool InputManager::IsKeyDown(SDL_Keycode key) const
{
    return m_keysDown.contains(key);
}

bool InputManager::IsKeyUp(SDL_Keycode key) const
{
    return m_keysUp.contains(key);
}

bool InputManager::IsKeyPressed(SDL_Keycode key) const
{
    return m_keysPressed.contains(key);
}

bool InputManager::IsMouseDown(unsigned int mouseBtnIdx) const
{
    return m_mouseDown.contains(mouseBtnIdx);
}

bool InputManager::IsMouseUp(unsigned int mouseBtnIdx) const
{
    return m_mouseUp.contains(mouseBtnIdx);
}

bool InputManager::IsMousePressed(unsigned int mouseBtnIdx) const
{
    return m_mousePressed.contains(mouseBtnIdx);
}
