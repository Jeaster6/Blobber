#include "GameLoop.hpp"

void gameplay(const std::string& saveFile) {
    SDL_Event e;
    bool quit = false;
    GameState* game = nullptr;
    std::string quickSaveFile = "quick.sav";

    if (saveFile == "") {
        game = new GameState();
    }
    else {
        game = loadGame(saveFile);
    }

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {

                    case SDLK_ESCAPE:
                        quit = true;
                        break;

                    case SDLK_w:
                        game->movePlayerForward();
                        break;

                    case SDLK_s:
                        game->movePlayerBackward();
                        break;

                    case SDLK_a:
                        game->movePlayerLeft();
                        break;

                    case SDLK_d:
                        game->movePlayerRight();
                        break;

                    case SDLK_q:
                        game->turnPlayerLeft();
                        break;

                    case SDLK_e:
                        game->turnPlayerRight();
                        break;

                    case SDLK_r:
                        game->quickSave(quickSaveFile);
                        break;

                    case SDLK_l:
                        delete game;
                        game = loadGame(quickSaveFile);
                        break;
                }
            }

            game->renderPlayerView();
        }
    }

    delete game;
}

static GameState* loadGame(const std::string& saveFile) {
    GameState* game;
    std::ifstream ifs(getSaveFileDirectory() + saveFile);
    boost::archive::binary_iarchive boostArchive(ifs);
    boostArchive >> game;
    return game;
}
