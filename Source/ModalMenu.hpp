#pragma once

#include "Menu.hpp"
#include "Button.hpp"
#include "Graphics.hpp"

class ModalMenu : public Menu {

    private:
        Button yesButton;
        Button noButton;
    
    public:
        ModalMenu();
        ModalMenu(const SDL_Rect&, const std::string&);
        ~ModalMenu();

        void render() const;
        bool clickYes(int, int, int, int) const;
        bool clickNo(int, int, int, int) const;
};
