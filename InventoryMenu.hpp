#pragma once

#include "Item.hpp"
#include "Menu.hpp"

class InventoryMenu : public Menu {

    private:

    public:
        InventoryMenu();
        InventoryMenu(const SDL_Rect&, const std::string&);
        ~InventoryMenu();

        void render() const;
};
