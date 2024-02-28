#include "GameLoop.hpp"

void gameplay(const std::string& saveFile) {
    SDL_Event event;
    bool quit = false;
    GameState game;
    UserInterface gameUI;
    std::string quickSaveFile = "quick.sav";

    if (saveFile != "") {
        game.loadGame(saveFile);
    }

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {

                case SDL_QUIT:
                    quit = true;
                    break;

                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {

                        case SDLK_ESCAPE:
                            quit = true;
                            break;

                        case SDLK_w:
                            game.movePlayerForward();
                            break;

                        case SDLK_s:
                            game.movePlayerBackward();
                            break;

                        case SDLK_a:
                            game.movePlayerLeft();
                            break;

                        case SDLK_d:
                            game.movePlayerRight();
                            break;

                        case SDLK_q:
                            game.turnPlayerLeft();
                            break;

                        case SDLK_e:
                            game.turnPlayerRight();
                            break;

                        case SDLK_r:
                            game.saveGame(quickSaveFile);
                            break;

                        case SDLK_l:
                            game.loadGame(quickSaveFile);
                            break;
                    }
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    gameUI.processMouseInput(event);
                    break;
            }

            game.renderPlayerView();
            gameUI.render();
        }
    }
}
