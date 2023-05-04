#include "cameraWrapper.hpp"

CameraWrapper::CameraWrapper::CameraWrapper(dojo::Renderer* _renderer) {
    m_camera = std::make_unique<dojo::Camera>();
    m_camera->setPos(glm::vec3(0, 0, 5));
    m_camera->lookAt(glm::vec3(0));

    dojo::Texture* infotexture = new dojo::Texture(glm::vec2(500, 200));
    m_info = std::make_unique<dojo::MenuItem>(glm::vec2(0.75, 0), glm::vec2(0.25, 0.05), infotexture);

    updateInfo(_renderer);
}

void CameraWrapper::CameraWrapper::updateInfo(dojo::Renderer* _renderer) {
    std::stringstream stream;
    glm::vec3 pos = m_camera->getPosition();
    stream << "CameraPos: " << pos.x << ", " << pos.y << ", " << pos.z;
    _renderer->textToTexture(m_info->getTexture(), stream.str(), glm::vec4(0.7, 0.7, 1, 1));
}

void CameraWrapper::move(float _amount, dojo::Camera::CameraDir _dir, dojo::Renderer* _renderer) {
    m_camera->move(_amount, _dir);
    updateInfo(_renderer);
}
