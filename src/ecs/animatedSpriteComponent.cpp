#include "../../include/ecs/animatedSpriteComponent.hpp"

namespace dojo {

    AnimatedSpriteComponent::AnimatedSpriteComponent(const std::string& _animationPath, int _textureRows, int _numFrames, const std::string& _name) : Component() {
        m_Animations = std::map<std::string, Animation>();
        addAnimation(_name, _animationPath, _textureRows, _numFrames);
        setAnimation(_name);

        std::vector<float> vertices = std::vector<float> {
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 0.0f,   1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 0.0f,   1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        };

        m_renderInfo = RenderInfo::genNewRenderInfo(vertices);
        m_flip = glm::ivec2(1);
    }

    AnimatedSpriteComponent::~AnimatedSpriteComponent() {
        std::map<std::string, Animation>::iterator it;
        for (it = m_Animations.begin(); it != m_Animations.end(); it++){
            delete (it->second.texture);
        }
    }

    void AnimatedSpriteComponent::addAnimation(const std::string& _name, const std::string& _animationPath, int _textureRows, int _numFrames) {
        Animation newAnimation = Animation();

        newAnimation.name = _name;
        newAnimation.texture = new Texture(_animationPath);
        newAnimation.currentFrame = 0;
        newAnimation.textureRows = _textureRows;
        newAnimation.numFrames = _numFrames;
        newAnimation.repeatFrames = 0;

        m_Animations[_name] = newAnimation;

    }

    void AnimatedSpriteComponent::flipx() {
        m_flip.x *= -1;
    }
    void AnimatedSpriteComponent::flipy() {
        m_flip.y *= -1;
    }
    glm::vec2 AnimatedSpriteComponent::getFlip() {
        return m_flip;
    }

    bool AnimatedSpriteComponent::setAnimation(const std::string& _name) {
        if (!m_Animations.contains(_name)) {
            return false;
        }
        m_CurrentAnimation = &(m_Animations[_name]);
        return true;
    }

    bool AnimatedSpriteComponent::nextFrame() {
        if (m_CurrentAnimation->currentFrame >= m_CurrentAnimation->numFrames - 1) {
            return false;
        }
        m_CurrentAnimation->currentFrame++;
        return true;
    }

    void AnimatedSpriteComponent::resetAnimation() {
        m_CurrentAnimation->currentFrame = 0;
    }

    Texture* AnimatedSpriteComponent::getTexture() {
        return m_CurrentAnimation->texture;;
    }

    int AnimatedSpriteComponent::getNumFrames() {
        return m_CurrentAnimation->numFrames;
    }

    int AnimatedSpriteComponent::getCurrentFrameNum() {
        return m_CurrentAnimation->currentFrame;
    }

    int AnimatedSpriteComponent::getCurrentAnimationRows() {
        return m_CurrentAnimation->textureRows;
    }

    std::string AnimatedSpriteComponent::currentAnimation() {
        return m_CurrentAnimation->name;
    }
}

