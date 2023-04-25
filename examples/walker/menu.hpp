#include "../../headers/dojo.hpp"

class Menu {
    public:
    Menu(dojo::Renderer* _renderer);
    ~Menu();
    void run();
    dojo::MenuItem* playButton;

    private:
    dojo::Renderer* m_renderer;
};
