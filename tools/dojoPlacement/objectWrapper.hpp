#include <dojo/dojo.hpp>
#include <memory>
#include <sstream>

class ObjectWrapper {
    public:
        ObjectWrapper(const std::string& _texturePath, glm::vec2 _menuPos, dojo::Renderer* _renderer);
        ~ObjectWrapper();

        void setPos(glm::vec3 _pos);
        void setScale(glm::vec3 _scale);
        glm::vec3 getPos();
        glm::vec3 getScale();
        dojo::GameObject2DStatic* getObject();
        dojo::MenuItem* getInfoItem();

    private:
        std::unique_ptr<dojo::GameObject2DStatic> m_object;
        std::unique_ptr<dojo::MenuItem> m_info;
        std::string m_path;
        dojo::Renderer* m_renderer;

        void updateInfo();


};
