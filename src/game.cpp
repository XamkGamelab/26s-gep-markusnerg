#include "game.hpp"

#include "inputmanager.hpp"
#include "resourcemanager.hpp"
#include "spriterenderer.hpp"
#include "testcomponent.hpp"
#include "time.hpp"

#include <glad/gl.h>

#include <iostream>

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

    m_assetDirectory = std::string(basePath) + "assets/";
    m_facePath       = m_assetDirectory + "awesomeface.png";

    if (!m_renderer.Init(m_assetDirectory + "shaders/"))
    {
        std::cerr << "Renderer2D initialization failed" << std::endl;
        return false;
    }

    m_renderer.SetViewport(kDefaultWidth, kDefaultHeight);
    m_scene.SetViewportSize(kDefaultWidth, kDefaultHeight);

    buildScene();
    demonstrateSystems();

    m_isRunning = true;

    std::cout << "Game initialized. Window " << kDefaultWidth
              << "x" << kDefaultHeight << std::endl;
    std::cout << "WASD = move camera, Q/E = zoom, R = reset camera, "
              << "X = remove GreenBox, ESC = quit" << std::endl;

    return true;
}

void Game::buildScene()
{
    ResourceManager& resources = ResourceManager::Instance();

    Texture* faceTexture = resources.LoadTexture(m_facePath);

    GameObject* background = m_scene.CreateGameObject("Background");
    SpriteRenderer* backgroundSprite = background->AddComponent<SpriteRenderer>();
    backgroundSprite->SetSize(16.0f, 9.0f);
    backgroundSprite->SetColor(0.16f, 0.18f, 0.26f, 1.0f);
    backgroundSprite->SetLayer(-10);

    GameObject* face = m_scene.CreateGameObject("Face");
    face->GetTransform().SetPosition(-2.5f, 0.0f);
    SpriteRenderer* faceSprite = face->AddComponent<SpriteRenderer>();
    faceSprite->SetTexture(faceTexture);
    faceSprite->SetSize(2.0f, 2.0f);
    faceSprite->SetLayer(0);
    face->AddComponent<SpinComponent>(40.0f);
    face->AddComponent<TestComponent>(3.0f);
    face->AddComponent<CounterComponent>();

    GameObject* secondFace = m_scene.CreateGameObject("FaceTwo");
    secondFace->GetTransform().SetPosition(2.5f, 0.0f);
    SpriteRenderer* secondSprite = secondFace->AddComponent<SpriteRenderer>();
    secondSprite->SetTexture(resources.LoadTexture(m_facePath));
    secondSprite->SetSize(2.0f, 2.0f);
    secondSprite->SetLayer(0);
    secondFace->AddComponent<PulseComponent>(1.0f, 0.25f, 2.0f);

    GameObject* tintedFace = m_scene.CreateGameObject("TintedFace");
    tintedFace->GetTransform().SetPosition(0.0f, 1.8f);
    SpriteRenderer* tintedSprite = tintedFace->AddComponent<SpriteRenderer>();
    tintedSprite->SetTexture(faceTexture);
    tintedSprite->SetSize(1.4f, 1.4f);
    tintedSprite->SetColor(1.0f, 0.4f, 0.4f, 1.0f);
    tintedSprite->SetLayer(1);

    GameObject* greenBox = m_scene.CreateGameObject("GreenBox");
    greenBox->GetTransform().SetPosition(0.0f, -1.8f);
    SpriteRenderer* boxSprite = greenBox->AddComponent<SpriteRenderer>();
    boxSprite->SetSize(1.2f, 1.2f);
    boxSprite->SetColor(0.3f, 0.8f, 0.4f, 1.0f);
    boxSprite->SetLayer(1);
    greenBox->AddComponent<SpinComponent>(-70.0f);
}

void Game::demonstrateSystems()
{
    ResourceManager& resources = ResourceManager::Instance();

    std::cout << "--- scene and resources ---" << std::endl;

    std::cout << "Scene '" << m_scene.GetName() << "' contains "
              << m_scene.GetGameObjectCount() << " game objects" << std::endl;

    Texture* first  = resources.LoadTexture(m_facePath);
    Texture* second = resources.LoadTexture(m_facePath);

    std::cout << "Same identifier returns the same texture: "
              << (first == second && first != nullptr ? "yes" : "no") << std::endl;

    std::cout << "Cached textures: " << resources.GetTextureCount()
              << ", cached shaders: " << resources.GetShaderCount() << std::endl;

    const GameObject* face = m_scene.FindGameObject("Face");
    if (face != nullptr)
    {
        std::cout << "Face has " << face->GetComponentCount() << " components" << std::endl;

        std::cout << "GetComponent<CounterComponent>() on Face: "
                  << (face->GetComponent<CounterComponent>() != nullptr ? "found" : "nullptr")
                  << std::endl;
    }

    const GameObject* background = m_scene.FindGameObject("Background");
    if (background != nullptr)
    {
        std::cout << "GetComponent<CounterComponent>() on Background: "
                  << (background->GetComponent<CounterComponent>() != nullptr ? "found" : "nullptr")
                  << std::endl;
    }

    std::cout << "---------------------------" << std::endl;
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
    const GameObject* face = m_scene.FindGameObject("Face");
    if (face != nullptr)
    {
        const CounterComponent* counter = face->GetComponent<CounterComponent>();

        if (counter != nullptr)
        {
            std::cout << "CounterComponent on Face counted "
                      << counter->GetCount() << " updates" << std::endl;
        }
    }

    m_scene.Clear();
    m_renderer.Shutdown();

    ResourceManager::Instance().Clear();

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
            m_scene.SetViewportSize(width, height);
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

    if (input.IsKeyDown(SDLK_X))
    {
        if (m_scene.RemoveGameObject("GreenBox"))
        {
            std::cout << "GreenBox removed, scene now has "
                      << m_scene.GetGameObjectCount() << " game objects" << std::endl;
        }
    }

    m_scene.Update(deltaTime);

    updateCamera(deltaTime);
}

void Game::updateCamera(float deltaTime)
{
    const InputManager& input = InputManager::Instance();

    Camera& camera = m_scene.GetCamera();

    glm::vec3 movement{ 0.0f, 0.0f, 0.0f };

    if (input.IsKeyPressed(SDLK_W)) { movement.y += 1.0f; }
    if (input.IsKeyPressed(SDLK_S)) { movement.y -= 1.0f; }
    if (input.IsKeyPressed(SDLK_A)) { movement.x -= 1.0f; }
    if (input.IsKeyPressed(SDLK_D)) { movement.x += 1.0f; }

    if (movement.x != 0.0f || movement.y != 0.0f)
    {
        camera.Move(movement * kCameraSpeed * deltaTime);
    }

    if (input.IsKeyPressed(SDLK_Q))
    {
        camera.SetZoom(camera.GetZoom() + deltaTime * 2.0f);
    }

    if (input.IsKeyPressed(SDLK_E))
    {
        camera.SetZoom(camera.GetZoom() - deltaTime * 2.0f);
    }

    if (input.IsKeyDown(SDLK_R))
    {
        camera.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        camera.SetZoom(3.0f);
        std::cout << "Camera reset" << std::endl;
    }

    if (input.IsKeyUp(SDLK_W) || input.IsKeyUp(SDLK_A) ||
        input.IsKeyUp(SDLK_S) || input.IsKeyUp(SDLK_D))
    {
        const glm::vec3& position = camera.GetPosition();
        std::cout << "Camera position: " << position.x << ", " << position.y << std::endl;
    }
}

void Game::render()
{
    m_scene.Render(m_renderer);

    SDL_GL_SwapWindow(m_window);
}
