#include "GameLoop.hpp"

void gameplay() {
    std::ifstream mapFile(getMapsDirectory() + "Map_1.dat");
    boost::archive::binary_iarchive boostArchive(mapFile);
    GameMap* gameMap;
    boostArchive >> gameMap;

    Player player(0, 0, S);
    Direction targetDirection;
    SDL_Event e;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            if (e.type == SDL_KEYDOWN) {
                targetDirection = player.getDirection();
                gameMap->makeScreenSnapshot(player);

                switch (e.key.keysym.sym) {

                    case SDLK_ESCAPE:
                        quit = true;
                        break;

                    case SDLK_w:
                        if (gameMap->getTile(player.getX(), player.getY())->isWalled(targetDirection)) {
                            break;
                        }
                        player.moveForward();
                        gameMap->animateForwardMovement(player);
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
                        gameMap->animateBackwardMovement(player);
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
                        gameMap->animateLeftRotation(player);
                        break;

                    case SDLK_e:
                        player.turnRight();
                        gameMap->animateRightRotation(player);
                        break;
                }
            }

            gameMap->renderVisibleArea(player);
        }
    }

    delete gameMap;
    gameMap = nullptr;
}