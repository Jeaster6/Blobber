#include "GameLoop.hpp"

void gameplay(const std::string& saveFile) {
    SDL_Event event;
    bool quit = false;
    GameState game;
    std::string quickSaveFile = "quick.sav";

    if (saveFile != "") {
        game.loadGame(saveFile);
    }

    Graphics::getInstance().init();
    Graphics::getInstance().loadMapTextures(game.getMap());

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {

                    case SDLK_ESCAPE:
                        quit = true;
                        break;

                    case SDLK_w:
                        if (game.movePlayerForward()) {
                            Graphics::getInstance().animateForwardMovement(game);
                        }
                        break;

                    case SDLK_s:
                        if (game.movePlayerBackward()) {
                            Graphics::getInstance().animateBackwardMovement(game);
                        }
                        break;

                    case SDLK_a:
                        if (game.movePlayerLeft()) {
                            Graphics::getInstance().animateSidestepLeft(game);
                        }
                        break;

                    case SDLK_d:
                        if (game.movePlayerRight()) {
                            Graphics::getInstance().animateSidestepRight(game);
                        }
                        break;

                    case SDLK_q:
                        game.turnPlayerLeft();
                        Graphics::getInstance().animateLeftRotation(game);
                        break;

                    case SDLK_e:
                        game.turnPlayerRight();
                        Graphics::getInstance().animateRightRotation(game);
                        break;

                    case SDLK_r:
                        game.saveGame(quickSaveFile);
                        break;

                    case SDLK_l:
                        game.loadGame(quickSaveFile);
                        break;
                }
            }

            Graphics::getInstance().renderPlayerView(game);
        }
    }
}
