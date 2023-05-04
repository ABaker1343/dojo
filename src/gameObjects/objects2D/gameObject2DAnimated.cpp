#include "../../../include/gameObjects/objects2D/gameObject2DAnimated.hpp"

namespace dojo {

    GameObject2DAnimated::GameObject2DAnimated(const std::string& _animationPath, int _textureRows, int _numFrames, const std::string& _name) : GameObject2D() {
        m_Animations = std::map<std::string, Animation>();
        addAnimation(_name, _animationPath, _textureRows, _numFrames);
        setAnimation(_name);
    }

    GameObject2DAnimated::~GameObject2DAnimated() {
        std::map<std::string, Animation>::iterator it;
        for (it = m_Animations.begin(); it != m_Animations.end(); it++){
            delete (it->second.texture);
        }
    }

    std::unique_ptr<GameObject> GameObject2DAnimated::clone() {
        return std::make_unique<GameObject2DAnimated>( *this );
    }

    void GameObject2DAnimated::addAnimation(const std::string& _name, const std::string& _animationPath, int _textureRows, int _numFrames) {
        Animation newAnimation = Animation();

        newAnimation.name = _name;
        newAnimation.texture = new Texture(_animationPath);
        newAnimation.currentFrame = 0;
        newAnimation.textureRows = _textureRows;
        newAnimation.numFrames = _numFrames;
        newAnimation.repeatFrames = 0;

        m_Animations[_name] = newAnimation;

    }

    bool GameObject2DAnimated::setAnimation(const std::string& _name) {
        if (!m_Animations.contains(_name)) {
            return false;
        }
        m_CurrentAnimation = &(m_Animations[_name]);
        return true;
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

    Texture* GameObject2DAnimated::getTexture() {
        return m_CurrentAnimation->texture;;
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

    std::string GameObject2DAnimated::currentAnimation() {
        return m_CurrentAnimation->name;
    }
}
