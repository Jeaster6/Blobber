#include "GameLoop.hpp"

bool gameplay(const std::string& saveFile) {
    bool quit = false;
    bool ALTF4 = false;
    int currentUIState = 0; // used to process response from UI
    SDL_Event event;
    GameState game = GameState(saveFile);
    UserInterface gameUI;
    std::string quickSaveFile = "quick.sav";

    while (!quit && !ALTF4) {
        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {

                case SDL_QUIT:
                    ALTF4 = true;
                    break;

                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {

                        case SDLK_ESCAPE:
                            if (currentUIState == 0) {
                                currentUIState = gameUI.openModalWindow();
                            }
                            else {
                                currentUIState = gameUI.closeAllWindows();
                            }
                            break;

                        case SDLK_w:
                            if (currentUIState == 0) {
                                game.movePlayerForward();
                            }
                            break;

                        case SDLK_s:
                            if (currentUIState == 0) {
                                game.movePlayerBackward();
                            }
                            break;

                        case SDLK_a:
                            if (currentUIState == 0) {
                                game.movePlayerLeft();
                            }
                            break;

                        case SDLK_d:
                            if (currentUIState == 0) {
                                game.movePlayerRight();
                            }
                            break;

                        case SDLK_q:
                            if (currentUIState == 0) {
                                game.turnPlayerLeft();
                            }
                            break;

                        case SDLK_e:
                            if (currentUIState == 0) {
                                game.turnPlayerRight();
                            }
                            break;

                        case SDLK_F5:
                            if (currentUIState == 0) {
                                game.saveGame(quickSaveFile);
                            }
                            break;

                        case SDLK_F9:
                            if (currentUIState == 0) {
                                game.loadGame(quickSaveFile);
                                // reset UI state just in case
                                gameUI = UserInterface();
                                currentUIState = 0;
                            }
                            break;

                        case SDLK_RETURN:
                            if (currentUIState == 1) {
                                quit = true;
                            }
                            break;
                    }
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    currentUIState = gameUI.processMouseInput(event, game);
                    if (currentUIState == -1) {
                        quit = true;
                    }
                    break;
            }
        }

        game.renderPlayerView();
        gameUI.render();
    }

    return ALTF4;
}
