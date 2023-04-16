#include "headers/gameObject2DAnimated.hpp"

namespace dojo {

    GameObject2DAnimated::GameObject2DAnimated(const std::string& _texturePath) :GameObject2D(_texturePath){
        m_Animations = std::map<std::string, Animation>();
    }

    GameObject2DAnimated::~GameObject2DAnimated() {
        std::map<std::string, Animation>::iterator it;
        for (it = m_Animations.begin(); it != m_Animations.end(); it++){
            delete (it->second.texture);
        }
    }

    void GameObject2DAnimated::addAnimation(const std::string& _name, const std::string& _animationPath, int _textureRows, int _numFrames) {
        Animation newAnimation = Animation();

        newAnimation.texture = new Texture(_animationPath);
        newAnimation.currentFrame = 0;
        newAnimation.textureRows = _textureRows;
        newAnimation.numFrames = _numFrames;
        newAnimation.repeatFrames = 0;

        m_Animations[_name] = newAnimation;

    }

    void GameObject2DAnimated::setAnimation(const std::string& _name) {
        m_CurrentAnimation = &(m_Animations[_name]);
    }

    bool GameObject2DAnimated::nextFrame() {
        if (m_CurrentAnimation->currentFrame >= m_CurrentAnimation->numFrames - 1) {
            return false;
        }
        m_CurrentAnimation->currentFrame++;
        return true;
    }

    void GameObject2DAnimated::resetAnimation() {
        m_CurrentAnimation->currentFrame = 0;
    }

    unsigned int GameObject2DAnimated::getTextureHandle() {
        return m_CurrentAnimation->texture->getHandle();
    }

    int GameObject2DAnimated::getNumFrames() {
        return m_CurrentAnimation->numFrames;
    }

    int GameObject2DAnimated::getCurrentFrameNum() {
        return m_CurrentAnimation->currentFrame;
    }

    int GameObject2DAnimated::getCurrentAnimationRows() {
        return m_CurrentAnimation->textureRows;
    }
}
