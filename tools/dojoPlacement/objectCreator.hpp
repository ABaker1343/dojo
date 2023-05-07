#ifndef __HEADER_OBJECT_CREATOR
#define __HEADER_OBJECT_CREATOR

#include <dojo/dojo.hpp>
#include <memory>
#include <filesystem>

#include "objectWrapper.hpp"

class ObjectCreator {
    public:
        ObjectCreator(dojo::Renderer* _renderer);
        ~ObjectCreator();
        
        std::pair<dojo::GameObject*, ObjectWrapper::Type> createObject();


    private:

        struct SelectionButton {
            dojo::MenuItem* button;
            std::string filepath;
        };

    private:
        std::unique_ptr<dojo::MenuItem> m_selectionMenu;
        dojo::Renderer* m_renderer;
        std::vector<SelectionButton> m_selectionButtons;
        
        std::vector<std::string> getFiles(const std::string& _dirpath);
        void genMenu(std::vector<std::string> _files);
        unsigned int bindMouseCallback(dojo::Window* _window, std::string* _selection, bool* _hasSelected);

};

#endif
