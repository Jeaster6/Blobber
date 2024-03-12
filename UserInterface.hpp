#pragma once

#include <vector>
#include <iostream>
#include <algorithm>
#include <SDL.h>
#include "Button.hpp"
#include "ModalMenu.hpp"
#include "GameState.hpp"
#include "InventoryMenu.hpp"

class UserInterface {

    private:
        Button exitButton;
        ModalMenu modal;
        InventoryMenu inventory;
        Button forward;
        Button backward;
        Button left;
        Button right;

    public:
        UserInterface();
        ~UserInterface();

        void render();
        int processMouseInput(const SDL_Event&, GameState&);
        int openModalWindow();
        int openInventoryWindow();
        int closeAllWindows();
};
