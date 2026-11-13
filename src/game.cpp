#include "game.hpp"
#include "inputmanager.hpp"
#include "testcomponent.hpp"
#include "time.hpp"

#include <glad/gl.h>

#include <cmath>
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

    const std::string assetDirectory  = std::string(basePath) + "assets/";
    const std::string shaderDirectory = assetDirectory + "shaders/";

    if (!m_renderer.Init(shaderDirectory))
    {
        std::cerr << "Renderer2D initialization failed" << std::endl;
        return false;
    }

    if (!m_texture.LoadFromFile(assetDirectory + "awesomeface.png"))
    {
        std::cerr << "Texture initialization failed" << std::endl;
        return false;
    }

    m_renderer.SetViewport(kDefaultWidth, kDefaultHeight);
    m_camera.SetViewportSize(kDefaultWidth, kDefaultHeight);

    initScene();
    setupGameObjects();
    demonstrateComponents();

    m_isRunning = true;

    std::cout << "Game initialized. Window " << kDefaultWidth
              << "x" << kDefaultHeight << std::endl;
    std::cout << "WASD = move camera, Q/E = zoom, R = reset camera, ESC = quit" << std::endl;

    return true;
}

void Game::initScene()
{
    m_leftQuad.SetPosition(-2.0f, 0.0f);
    m_leftQuad.SetScale(2.0f);
    m_leftQuad.SetRotation(0.0f);

    m_rightQuad.SetPosition(2.0f, 0.0f);
    m_rightQuad.SetScale(1.5f);
    m_rightQuad.SetRotation(0.0f);

    m_smallQuad.SetPosition(0.0f, 1.8f);
    m_smallQuad.SetScale(0.8f);
    m_smallQuad.SetRotation(45.0f);
}

void Game::setupGameObjects()
{
    m_player.AddComponent<TestComponent>(2.0f);
    m_player.AddComponent<CounterComponent>();

    m_prop.AddComponent<TestComponent>(3.0f);
}

void Game::demonstrateComponents()
{
    std::cout << "--- component system ---" << std::endl;

    std::cout << m_player.GetName() << " has "
              << m_player.GetComponentCount() << " components" << std::endl;

    TestComponent* playerTest = m_player.GetComponent<TestComponent>();
    std::cout << "GetComponent<TestComponent>() on " << m_player.GetName()
              << ": " << (playerTest != nullptr ? "found" : "nullptr") << std::endl;

    if (playerTest != nullptr && playerTest->GetOwner() != nullptr)
    {
        std::cout << "  owner reported by component: "
                  << playerTest->GetOwner()->GetName() << std::endl;
    }

    CounterComponent* playerCounter = m_player.GetComponent<CounterComponent>();
    std::cout << "GetComponent<CounterComponent>() on " << m_player.GetName()
              << ": " << (playerCounter != nullptr ? "found" : "nullptr") << std::endl;

    std::cout << m_prop.GetName() << " has "
              << m_prop.GetComponentCount() << " components" << std::endl;

    CounterComponent* propCounter = m_prop.GetComponent<CounterComponent>();
    std::cout << "GetComponent<CounterComponent>() on " << m_prop.GetName()
              << ": " << (propCounter != nullptr ? "found" : "nullptr") << std::endl;

    std::cout << "------------------------" << std::endl;
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
    const CounterComponent* counter = m_player.GetComponent<CounterComponent>();
    if (counter != nullptr)
    {
        std::cout << "CounterComponent on " << m_player.GetName()
                  << " counted " << counter->GetCount() << " updates" << std::endl;
    }

    m_texture.Release();
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
            m_camera.SetViewportSize(width, height);
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

    m_elapsed += deltaTime;

    m_player.Update(deltaTime);
    m_prop.Update(deltaTime);

    m_rightQuad.Rotate(kRotationSpeed * deltaTime);

    const float pulse = 0.8f + 0.3f * std::sin(m_elapsed * 2.0f);
    m_smallQuad.SetScale(pulse);

    updateCamera(deltaTime);
}

void Game::updateCamera(float deltaTime)
{
    const InputManager& input = InputManager::Instance();

    glm::vec3 movement{ 0.0f, 0.0f, 0.0f };

    if (input.IsKeyPressed(SDLK_W)) { movement.y += 1.0f; }
    if (input.IsKeyPressed(SDLK_S)) { movement.y -= 1.0f; }
    if (input.IsKeyPressed(SDLK_A)) { movement.x -= 1.0f; }
    if (input.IsKeyPressed(SDLK_D)) { movement.x += 1.0f; }

    if (movement.x != 0.0f || movement.y != 0.0f)
    {
        m_camera.Move(movement * kCameraSpeed * deltaTime);
    }

    if (input.IsKeyPressed(SDLK_Q))
    {
        m_camera.SetZoom(m_camera.GetZoom() + deltaTime * 2.0f);
    }

    if (input.IsKeyPressed(SDLK_E))
    {
        m_camera.SetZoom(m_camera.GetZoom() - deltaTime * 2.0f);
    }

    if (input.IsKeyDown(SDLK_R))
    {
        m_camera.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        m_camera.SetZoom(3.0f);
        std::cout << "Camera reset" << std::endl;
    }

    if (input.IsKeyUp(SDLK_W) || input.IsKeyUp(SDLK_A) ||
        input.IsKeyUp(SDLK_S) || input.IsKeyUp(SDLK_D))
    {
        const glm::vec3& position = m_camera.GetPosition();
        std::cout << "Camera position: " << position.x << ", " << position.y << std::endl;
    }
}

void Game::render()
{
    m_renderer.Begin(m_camera);

    m_renderer.DrawQuad(m_texture, m_leftQuad);
    m_renderer.DrawQuad(m_texture, m_rightQuad);
    m_renderer.DrawQuad(m_texture, m_smallQuad);

    m_renderer.End();

    SDL_GL_SwapWindow(m_window);
}
