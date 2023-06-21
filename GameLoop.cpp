#include "GameLoop.hpp"

void gameplay() {
    std::ifstream mapFile(getMapsDirectory() + "Map_1.dat");
    boost::archive::binary_iarchive boostArchive(mapFile);
    GameMap* gameMap;
    boostArchive >> gameMap;

    Player player(0, 0, S);
    SDL_Renderer* gRenderer = Graphics::getInstance().getRenderer();
    SDL_Event e;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            if (e.type == SDL_KEYDOWN) {
                Direction targetDirection = player.getDirection();
                switch (e.key.keysym.sym) {

                    case SDLK_ESCAPE:
                        quit = true;
                        break;

                    case SDLK_w:
                        if (gameMap->getTile(player.getX(), player.getY())->isWalled(targetDirection)) {
                            break;
                        }
                        player.moveForward();
                        break;

                    case SDLK_a:
                        targetDirection--;
                        if (gameMap->getTile(player.getX(), player.getY())->isWalled(targetDirection)) {
                            break;
                        }
                        player.moveLeft();
                        break;

                    case SDLK_s:
                        targetDirection++;
                        targetDirection++;
                        if (gameMap->getTile(player.getX(), player.getY())->isWalled(targetDirection)) {
                            break;
                        }
                        player.moveBackward();
                        break;

                    case SDLK_d:
                        targetDirection++;
                        if (gameMap->getTile(player.getX(), player.getY())->isWalled(targetDirection)) {
                            break;
                        }
                        player.moveRight();
                        break;

                    case SDLK_q:
                        player.turnLeft();
                        break;

                    case SDLK_e:
                        player.turnRight();
                        break;
                }
            }
            gameMap->renderVisibleArea(player);
        }
    }
    delete gameMap;

    gameMap = nullptr;
}