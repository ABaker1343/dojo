#include "menu.hpp"

Menu::Menu(dojo::Renderer* _renderer) {
    m_renderer = _renderer;
    dojo::Texture* buttonTexture = new dojo::Texture(glm::vec2(1000, 200));
    m_renderer->drawToTexture(buttonTexture, "play game", 0, 0, 1);
    playButton = new dojo::MenuItem(glm::vec2(0.3, 0.45), glm::vec2(0.5, 0.1), buttonTexture);
}
Menu::~Menu() {
    delete playButton;
}

void Menu::run() {
    bool running = false;
    // dont delete these as they are not owned by this function
    dojo::Window* windowPointer = m_renderer->getWindow();
    dojo::MenuItem* itemPointer = playButton;
    bool* runningPointer = &running;

    windowPointer->useDefaultCursorPosCallback(true);
        unsigned int callback = windowPointer->setCustomMouseCallback([runningPointer, windowPointer, itemPointer](int _button, int _action, int _mods) mutable {
                if (_button == GLFW_MOUSE_BUTTON_1 && _action == GLFW_PRESS) {
                    glm::vec2 mousePos = windowPointer->getMousePos();
                    glm::vec2 windowSize = windowPointer->getDimensions();
                    if (itemPointer->isMouseOver(windowSize, mousePos)) {
                        *runningPointer = false;
                    }
                }
            });

    running = true;
    while (running) {
        m_renderer->clear();
        m_renderer->setShader("menuShader");
        m_renderer->draw(playButton);
        windowPointer->pollEvents();
        windowPointer->flipBuffers();
    }

    windowPointer->removeMouseCallback(callback);
}
