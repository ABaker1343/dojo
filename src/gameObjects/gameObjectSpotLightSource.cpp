#include "headers/gameObject.hpp"
#include "headers/gameObjectSpotLightSource.hpp"
#include <glm/ext/matrix_clip_space.hpp>

namespace dojo {

GameObjectSpotLightSource::GameObjectSpotLightSource(glm::vec3 target, glm::vec3 color,
    glm::vec3 pos, glm::vec3 scale) : GameObject(pos, scale) {

    lightColor = color;
    m_target = target;

    float near_plane = 0.1f, far_plane = 50.f;
    //lightProjectionTransform = glm::ortho(-10.f, 10.f, -10.f, 10.f, near_plane, far_plane);
    lightProjectionTransform = glm::perspective(glm::radians(90.f), 1600.f/900.f, near_plane, far_plane);

    lightViewTransform = glm::lookAt(pos, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
}

GameObjectSpotLightSource::~GameObjectSpotLightSource() {

}

glm::mat4 GameObjectSpotLightSource::getProjectionTransform() {
    return lightProjectionTransform;
}

glm::mat4 GameObjectSpotLightSource::getViewTransform() {
    //glm::vec3 eye = getPos();
    //eye.x = eye.x * - 1;
    //eye.y = eye.y * - 1;
    //eye.z = eye.z * - 1;
    return glm::lookAt(getPos(), m_target, glm::vec3(0.f, 1.f, 0.f));
    //return lightViewTransform;
}

}
