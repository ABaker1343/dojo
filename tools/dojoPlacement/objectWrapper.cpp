#include "objectWrapper.hpp"
#include <dojo/gameObjects/texture.hpp>
#include <dojo/window/renderer.hpp>
#include <memory>

std::ostream& operator<< (std::ostream& _stream, glm::vec3 _vec) {
    _stream << _vec.x << ", " << _vec.y << ", " << _vec.z;
    return _stream;
}

ObjectWrapper::ObjectWrapper(const std::string& _texturePath, glm::vec2 _menuPos, dojo::Renderer* _renderer) {
    m_renderer = _renderer;
    m_object = std::make_unique<dojo::GameObject2DStatic>(_texturePath);
    dojo::Texture* infoTexture = new dojo::Texture(glm::vec2(500, 100));
    m_info = std::make_unique<dojo::MenuItem>(_menuPos, glm::vec2(0.25, 0.05), infoTexture);
    m_path = _texturePath;
    updateInfo();
}

ObjectWrapper::~ObjectWrapper() {

}

glm::vec3 ObjectWrapper::getPos() {
    return m_object->getPos();
}
glm::vec3 ObjectWrapper::getScale() {
    return m_object->getScale();
}

void ObjectWrapper::setPos(glm::vec3 _pos) {
    m_object->setPos(_pos);
    updateInfo();
}
void ObjectWrapper::setScale(glm::vec3 _scale) {
    m_object->setScale(_scale);
}

void ObjectWrapper::updateInfo() {
    std::stringstream stream;
    stream << m_path << ": pos: " << m_object->getPos();
    std::string infoString = stream.str();
    std::cout << infoString << std::endl;
    m_renderer->textToTexture(m_info->getTexture(), infoString, glm::vec4(1), glm::vec4(0));
}

dojo::GameObject2DStatic* ObjectWrapper::getObject() {
    return m_object.get();
}

dojo::MenuItem* ObjectWrapper::getInfoItem() {
    return m_info.get();
}
