#include "game.hpp"
#include <bits/chrono.h>
#include <chrono>

Game::Game() {
    m_window = new dojo::Window(1280, 720, "Walker");
    m_renderer = new dojo::Renderer(m_window);
    m_renderer->setShaderPath("../../src/shaders/");
    m_renderer->loadShaders();

    m_camera = new dojo::Camera();
    m_camera->setPos(glm::vec3(0, 3, 10));
    m_camera->lookAt(glm::vec3(0, 3, 0));

    m_menu = new Menu(m_renderer);
    m_walker = new Walker();
    m_walker->m_object->setScale(glm::vec2(1.7));
    m_walker->m_object->setPos(glm::vec3(0, 0.9, 0));

    dojo::Texture* gotexture = new dojo::Texture(glm::ivec2(1000, 200));
    m_renderer->textToTexture(gotexture, "YOU DIED", glm::vec3(1.0, 0, 0), glm::vec4(0));
    m_gameOverText = new dojo::MenuItem(glm::vec2(0.3, 0.55), glm::vec2(0.4, 0.1), gotexture);

    m_background = new dojo::GameObject2DStatic("Background.png");
    m_background->setScale(glm::vec2(15));
    m_background->setPos(glm::vec3(0, 11.5, -1));

    m_background2 = new dojo::GameObject2DStatic("Background.png");
    m_background2->setScale(glm::vec2(15));
    m_background2->setPos(m_background->getPos().x + m_background->getScale().x * 2, m_background->getPos().y, m_background->getPos().z);
}

Game::~Game() {
    delete m_menu;
    delete m_background;
    delete m_background2;
    delete m_gameOverText;
    delete m_walker;
    delete m_camera;
    delete m_renderer;
    delete m_window;
}

void Game::run() {
    m_menu->run();

    runMainLoop();
}

void Game::runMainLoop() {
    m_running = true;
    m_window->useDefaultKeyCallback(true);
    m_walker->m_state = Walker::IDLE;
    m_walker->m_object->setAnimation("idle");
    std::cout << "flip " << m_walker->m_object->getFlip().x;

    float movementSpeed = 0.003;
    float animationTime = 1000.f/4.f;

    auto lastFrame = std::chrono::steady_clock::now();
    auto lastAnimation = std::chrono::steady_clock::now();

    m_renderer->setShader("2DAnimated");

    while(m_running) {
        auto frameStart = std::chrono::steady_clock::now();
        unsigned int deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(frameStart - lastFrame).count();
        unsigned int deltaAnimation = std::chrono::duration_cast<std::chrono::milliseconds>(frameStart - lastAnimation).count();

        m_window->pollEvents();
        m_window->flipBuffers();

        if (m_window->KEYS[GLFW_KEY_D]) {
            glm::vec2 toMove = glm::vec2(0);
            toMove.x = movementSpeed * deltaTime;
            m_walker->move(toMove);
            if (m_walker->m_prevState != Walker::WALKING) {
                m_walker->m_prevState = Walker::WALKING;
                m_walker->m_object->setAnimation("walking");
            }
        } else if (m_walker->m_state != Walker::WALKING && m_walker->m_prevState == Walker::WALKING) {
            m_running = false;
        }

        if (animationTime < deltaAnimation) {
            if (!m_walker->m_object->nextFrame()) {
                m_walker->m_object->resetAnimation();
            }
            lastAnimation = std::chrono::steady_clock::now();
        }

        lastFrame = std::chrono::steady_clock::now();
        
        if (m_walker->m_object->getPos().x > m_camera->getPosition().x + 3) {
            glm::vec3 curr = m_camera->getPosition();
            glm::vec3 move = glm::vec3(movementSpeed * deltaTime, 0, 0);
            m_camera->setPos(curr + move);
        }
        if (m_walker->m_object->getPos().x > m_background->getPos().x + m_background->getScale().x * 2) {
            m_background->setPos(m_background2->getPos());
            m_background2->setPos(m_background->getPos().x + m_background->getScale().x * 2, m_background->getPos().y, m_background->getPos().z);
        }

        // render
        m_renderer->clear();
        m_renderer->setShader("2DStatic");
        m_renderer->draw(m_camera, m_background);
        m_renderer->draw(m_camera, m_background2);
        m_renderer->setShader("2DAnimated");
        m_renderer->draw(m_camera, m_walker->m_object);
    }

    m_walker->m_object->setAnimation("die");
    m_walker->m_object->resetAnimation();

    lastFrame = std::chrono::steady_clock::now();
    lastAnimation = std::chrono::steady_clock::now();
    auto startTime = std::chrono::steady_clock::now();

    while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - startTime).count() < 4) {
        auto thisFrame = std::chrono::steady_clock::now();
        auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(thisFrame - lastFrame).count();
        auto deltaAnimation = std::chrono::duration_cast<std::chrono::milliseconds>(thisFrame - lastAnimation).count();

        if (animationTime < deltaAnimation) {
            m_walker->m_object->nextFrame();
            lastAnimation = std::chrono::steady_clock::now();
        }
        if (!(m_camera->getPosition().y > m_background->getPos().y + m_background->getScale().y * 0.75) ) { 
            glm::vec3 newPos = m_camera->getPosition() + glm::vec3(0, movementSpeed * deltaTime * 0.3, 0);
            m_camera->setPos(newPos);
        }

        lastFrame = std::chrono::steady_clock::now();

        m_renderer->clear();
        m_renderer->setShader("2DStatic");
        m_renderer->draw(m_camera, m_background);
        m_renderer->draw(m_camera, m_background2);
        m_renderer->setShader("2DAnimated");
        m_renderer->draw(m_camera, m_walker->m_object);
        m_renderer->setShader("menuShader");
        m_renderer->draw(m_gameOverText);
        m_window->flipBuffers();
    }


}
