#include "objectCreator.hpp"
#include <filesystem>

ObjectCreator::ObjectCreator(dojo::Renderer* _renderer) {
    m_renderer = _renderer;
    m_selectionMenu = std::make_unique<dojo::MenuItem>(glm::vec2(0.2), glm::vec2(0.6), "nice", m_renderer);
    m_selectionButtons = std::vector<SelectionButton>();
    genMenu(getFiles("./"));
}

ObjectCreator::~ObjectCreator() {
    for (size_t i = 0; i < m_selectionButtons.size(); i++) {
        delete m_selectionButtons[i].button;
    }
}

std::pair<dojo::GameObject*, ObjectWrapper::Type> ObjectCreator::createObject() {
    bool hasSelected = false;
    std::string selection;
    dojo::Window* window = m_renderer->getWindow();

    std::vector<std::string> files = getFiles("./");

    unsigned int mouseCallback  = bindMouseCallback(window, &selection, &hasSelected);
    window->pollEvents();

    while(!hasSelected) {
        window->pollEvents();
        m_renderer->clear();
        //m_renderer->draw(m_selectionMenu.get());
        for (size_t i = 0; i < m_selectionButtons.size(); i++) {
            m_renderer->draw(m_selectionButtons[i].button);
        }
        window->flipBuffers();
    }

    window->removeMouseCallback(mouseCallback);

    if (selection.ends_with(".obj")) {
        m_renderer->draw(m_selectionButtons[0].button);
        std::cout << "accessed renderer: " << m_renderer << std::endl;
        return std::pair(new dojo::GameObject3D(selection), ObjectWrapper::Type::OBJECT_3D);
    } else {
        m_renderer->draw(m_selectionButtons[0].button);
        std::cout << "accessed renderer: " << m_renderer << std::endl;
        return std::pair(new dojo::GameObject2DStatic(selection), ObjectWrapper::Type::OBJECT_2D_STATIC);
    }
}

std::vector<std::string> ObjectCreator::getFiles(const std::string& _dirpath) {
    std::vector<std::string> files;
    std::filesystem::directory_iterator dirIterator(_dirpath);

    for (const auto& file : dirIterator) {
        if (!file.is_directory()) {
            files.push_back(file.path());
        }
    }

    return files;
}

void ObjectCreator::genMenu(std::vector<std::string> _files) {
    m_selectionButtons.clear();
    float x, y;
    float gap = 0.05;
    float offsetx = 0.2;
    float offsety = 0.2;
    x = 0.6 / 4 - gap;
    y = (0.6 / _files.size()) * 2;
    std::cout << x << " " << y << std::endl;
    for (size_t i = 0; i < _files.size(); i++) {
        std::cout << _files[i] << std::endl;
        //dojo::MenuItem* newButton = new dojo::MenuItem(glm::vec2(x + (i % 4) * gap, y + gap), glm::vec2(x, y), _files[i], m_renderer);
        dojo::MenuItem* newButton = new dojo::MenuItem(glm::vec2(offsetx + (i % 4) * x, offsety + i/4 * y), glm::vec2(x, y), _files[i], m_renderer);
        m_selectionButtons.push_back({newButton, _files[i]});
    }
}

unsigned int ObjectCreator::bindMouseCallback(dojo::Window* _window, std::string* _selection, bool* _hasSelected) {
    dojo::MouseCallbackFunction callback;
    callback = [this, _window, _hasSelected, _selection] (int _button, int _action, int _mods) {
        glm::vec2 dim = _window->getDimensions();
        glm::vec2 mousePos = _window->getMousePos();
        for (size_t i = 0; i < m_selectionButtons.size(); i++) {
            if (m_selectionButtons[i].button->isMouseOver(dim, mousePos)) {
                *_selection = m_selectionButtons[i].filepath;
                *_hasSelected = true;
            }
        }
    };

    return _window->setCustomMouseCallback(callback);
}
