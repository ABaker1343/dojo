#ifndef __HEADER_OBJECT_WRAPPER
#define __HEADER_OBJECT_WRAPPER

#include <dojo/dojo.hpp>
#include <memory>
#include <sstream>

std::ostream& operator<< (std::ostream& _stream, glm::vec3 _vec);

class ObjectWrapper {
    
    public:

    enum Type {
        OBJECT_2D_STATIC,
        OBJECT_2D_ANIMATED,
        OBJECT_3D,
    };

    public:
        ObjectWrapper(const std::string& _texturePath, glm::vec2 _menuPos, dojo::Renderer* _renderer);
        ObjectWrapper(dojo::GameObject* _object, Type _type, glm::vec2 _menu, dojo::Renderer* _renderer);
        ~ObjectWrapper();

        void setPos(glm::vec3 _pos);
        void setScale(glm::vec3 _scale);
        glm::vec3 getPos();
        glm::vec3 getScale();
        dojo::GameObject* getObject();
        dojo::MenuItem* getInfoItem();
        void setColor(glm::vec4 _color);
        Type getType();

    private:
        dojo::GameObject* m_object;
        Type m_type;
        std::unique_ptr<dojo::MenuItem> m_info;
        std::string m_path;
        dojo::Renderer* m_renderer;
        glm::vec4 m_infoColor;

        void updateInfo();


};

#endif
