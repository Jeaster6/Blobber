#pragma once

#include <string>
#include <vector>
#include "Button.hpp"
#include "Item.hpp"
#include "Menu.hpp"

enum Mode { Looting, Party };

class InventoryMenu : public Menu {

    private:
        Mode mode;
        std::vector<Button> buttons;

    public:
        InventoryMenu();
        InventoryMenu(const SDL_Rect&, const std::string&);
        ~InventoryMenu();

        void render(const std::vector<std::string>&, const std::vector<std::string>&);
        void open(Mode);
        Mode getMode() const;
        bool clickButton(int, int, int, int, int) const;
};
