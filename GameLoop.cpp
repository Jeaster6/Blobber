#include "GameLoop.hpp"

void gameplay() {

    GameMap *gameMap;
    GameMap *visibleArea = new GameMap(15, 7);

    std::ifstream ifs(getMapsDirectory() + "Map_1.dat");
    boost::archive::binary_iarchive ia(ifs);
    ia >> gameMap;

    Player player(0, 0, S);

    SDL_Renderer* gRenderer = Graphics::getInstance().getRenderer();
    SDL_Surface* sideWallSurface = NULL;
    SDL_Surface* frontWallSurface = NULL;
    SDL_Surface* floorSurface = NULL;
    SDL_Surface* objectSurface = NULL;
    SDL_Texture* wallTexture = NULL;
    SDL_Texture* floorTexture = NULL;
    SDL_Texture* frontTexture = NULL;
    SDL_Texture* objectTexture = NULL;
    SDL_Event e;
    SDL_Rect DestR;

    int color;
    int tileWidth;
    int tileHeight;
    int gameWidth = 0;
    int screenWidth = 0;
    int screenHeight = 0;
    double fov = 0;
    bool quit = false;

	frontWallSurface=IMG_Load((getTexturesDirectory() + "Front.png").c_str());
	frontTexture=SDL_CreateTextureFromSurface(gRenderer, frontWallSurface);
	sideWallSurface=IMG_Load((getTexturesDirectory() + "Wall.png").c_str());
	wallTexture=SDL_CreateTextureFromSurface(gRenderer, sideWallSurface);
	floorSurface=IMG_Load((getTexturesDirectory() + "Floor.png").c_str());
	floorTexture=SDL_CreateTextureFromSurface(gRenderer, floorSurface);
	objectSurface=IMG_Load((getTexturesDirectory() + "Object.png").c_str());
	objectTexture=SDL_CreateTextureFromSurface(gRenderer, objectSurface);

    SDL_QueryTexture(frontTexture, NULL, NULL, &tileWidth, &tileHeight);
    screenWidth = Graphics::getInstance().getScreenWidth();
    screenHeight = Graphics::getInstance().getScreenHeight();
    fov = Graphics::getInstance().getFOV();
    gameWidth = (int)(3 * screenWidth / 4);
    tileWidth=(int)(tileWidth*gameWidth/2880);
    tileHeight=(int)(1.3*tileHeight*screenHeight/2160);

    while (!quit) {

        while (SDL_PollEvent(&e)!=0) {

            if (e.type==SDL_QUIT) {
                quit = true;
            }

            switch (player.getDirection()) {
                case Direction::N:
                    for (int i=0; i<15; i++) {
                        for (int j=0; j<7; j++) {
                            if (player.getY() - j<0 || player.getX() + i - 7 >= gameMap->getWidth() || player.getX() + i - 7<0) {
                                visibleArea->setTile(i, j, true, true, true, true);
                            }
                            else {
                                visibleArea->setTile(i, j, gameMap->getTile(player.getX() +i-7, player.getY() -j));
                            }
                        }
                    }
                    break;

                case Direction::E:
                    for (int i=0; i<15; i++) {
                        for (int j=0; j<7; j++) {
                            if (player.getX() +j>=gameMap->getWidth() || player.getY() +i-7>=gameMap->getHeight() || player.getY() +i-7<0) {
                                visibleArea->setTile(i, j, true, true, true, true);
                            }
                            else {
                                visibleArea->setTile(i, j, gameMap->getTile(player.getX() +j, player.getY() +i-7));
                            }
                        }
                    }
                    break;

                case Direction::S:
                    for (int i=0; i<15; i++) {
                        for (int j=0; j<7; j++) {
                            if (player.getY() +j>=gameMap->getHeight() || player.getX() -i+7>=gameMap->getWidth() || player.getX() -i+7<0) {
                                visibleArea->setTile(i, j, true, true, true, true);
                            }
                            else {
                                visibleArea->setTile(i, j, gameMap->getTile(player.getX() -i+7, player.getY() +j));
                            }
                        }
                    }
                    break;

                case Direction::W:
                    for (int i=0; i<15; i++) {
                        for (int j=0; j<7; j++) {
                            if (player.getX() -j<0 || player.getY() -i+7>=gameMap->getHeight() || player.getY() -i+7<0) {
                                visibleArea->setTile(i, j, true, true, true, true);
                            }
                            else {
                                visibleArea->setTile(i, j, gameMap->getTile(player.getX() -j, player.getY() -i+7));
                            }
                        }
                    }
                    break;
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

            SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
            SDL_RenderClear(gRenderer);
            SDL_Rect tileRect={0, 0, gameWidth, screenHeight};
            SDL_RenderFillRect(gRenderer, &tileRect);

            DestR.w=gameWidth;
            DestR.h=(int)((screenHeight-tileHeight*pow(fov, 7))/2);
            DestR.x=0;
            DestR.y=(int)((screenHeight+tileHeight*pow(fov, 7))/2);
            SDL_RenderCopyEx(gRenderer, floorTexture, NULL, &DestR, 0.0, NULL, SDL_FLIP_NONE);

            DestR.y=0;
            SDL_RenderCopyEx(gRenderer, floorTexture, NULL, &DestR, 0.0, NULL, SDL_FLIP_VERTICAL);

            for (int j=6; j>=0; j--) {

                color=(int)(255*pow(0.55, j));
                SDL_SetTextureColorMod(wallTexture, color, color, color);
                SDL_SetTextureColorMod(frontTexture, color, color, color);

                switch (player.getDirection()) {

                    case Direction::N:
                        for (int i=-j-1; i<=0; i++) {

                            if (visibleArea->getTile(i+7, j)->isWalled(Direction::W)) {
                                DestR.x=(int)((gameWidth-tileWidth*pow(fov, j))/2+i*tileWidth*pow(fov, j));
                                DestR.w=(int)(abs(i)*(tileWidth*pow(fov, j)-tileWidth*pow(fov, j+1))+(tileWidth*pow(fov, j)-tileWidth*pow(fov, j+1))/2+1);
                                DestR.h=(int)(tileHeight*pow(fov, j));
                                DestR.y=(int)((screenHeight-tileHeight*pow(fov, j))/2);
                                SDL_RenderCopyEx(gRenderer, wallTexture, NULL, &DestR, 0.0, NULL, SDL_FLIP_NONE);
                            }

                            if (visibleArea->getTile(i+7, j)->isWalled(Direction::N)) {
                                DestR.x=(int)((gameWidth-tileWidth*pow(fov, j+1))/2+i*tileWidth*pow(fov, j+1));
                                DestR.w=(int)(tileWidth*pow(fov, j+1));
                                DestR.h=(int)(tileHeight*pow(fov, j+1));
                                DestR.y=(int)((screenHeight-tileHeight*pow(fov, j+1))/2);
                                SDL_RenderCopyEx(gRenderer, frontTexture, NULL, &DestR, 0.0, NULL, SDL_FLIP_NONE);
                            }

                            if (visibleArea->getTile(i+7, j)->containsObject()) {
                                DestR.x=(int)((gameWidth-tileWidth*pow(fov, j+1))/2+i*tileWidth*pow(fov, j+1));
                                DestR.w=(int)(tileWidth*pow(fov, j+1));
                                DestR.h=(int)(tileHeight*pow(fov, j+1));
                                DestR.y=(int)((screenHeight-tileHeight*pow(fov, j+1))/2);
                                SDL_RenderCopyEx(gRenderer, objectTexture, NULL, &DestR, 0.0, NULL, SDL_FLIP_NONE);
                            }
                        }

                        for (int i=j+1; i>=0; i--) {

                            if (visibleArea->getTile(i+7, j)->isWalled(Direction::N)) {
                                DestR.x=(int)((gameWidth-tileWidth*pow(fov, j+1))/2+i*tileWidth*pow(fov, j+1));
                                DestR.w=(int)(tileWidth*pow(fov, j+1));
                                DestR.h=(int)(tileHeight*pow(fov, j+1));
                                DestR.y=(int)((screenHeight-tileHeight*pow(fov, j+1))/2);
                                SDL_RenderCopyEx(gRenderer, frontTexture, NULL, &DestR, 0.0, NULL, SDL_FLIP_NONE);
                            }

                            if (visibleArea->getTile(i+7, j)->isWalled(Direction::E)) {
                                DestR.x=(int)((gameWidth+tileWidth*pow(fov, j+1))/2+i*tileWidth*pow(fov, j+1));
                                DestR.w=(int)(abs(i)*(tileWidth*pow(fov, j)-tileWidth*pow(fov, j+1))+(tileWidth*pow(fov, j)-tileWidth*pow(fov, j+1))/2+1);
                                DestR.h=(int)(tileHeight*pow(fov, j));
                                DestR.y=(int)((screenHeight-tileHeight*pow(fov, j))/2);
                                SDL_RenderCopyEx(gRenderer, wallTexture, NULL, &DestR, 0.0, NULL, SDL_FLIP_HORIZONTAL);
                            }

                            if (visibleArea->getTile(i+7, j)->containsObject()) {
                                DestR.x=(int)((gameWidth-tileWidth*pow(fov, j+1))/2+i*tileWidth*pow(fov, j+1));
                                DestR.w=(int)(tileWidth*pow(fov, j+1));
                                DestR.h=(int)(tileHeight*pow(fov, j+1));
                                DestR.y=(int)((screenHeight-tileHeight*pow(fov, j+1))/2);
                                SDL_RenderCopyEx(gRenderer, objectTexture, NULL, &DestR, 0.0, NULL, SDL_FLIP_NONE);
                            }
                        }
                        break;

                    case Direction::E:

                        for (int i=-j-1; i<=0; i++) {

                            if (visibleArea->getTile(i+7, j)->isWalled(Direction::N)) {
                                DestR.x=(int)((gameWidth-tileWidth*pow(fov, j))/2+i*tileWidth*pow(fov, j));
                                DestR.w=(int)(abs(i)*(tileWidth*pow(fov, j)-tileWidth*pow(fov, j+1))+(tileWidth*pow(fov, j)-tileWidth*pow(fov, j+1))/2+1);
                                DestR.h=(int)(tileHeight*pow(fov, j));
                                DestR.y=(int)((screenHeight-tileHeight*pow(fov, j))/2);
                                SDL_RenderCopyEx(gRenderer, wallTexture, NULL, &DestR, 0.0, NULL, SDL_FLIP_NONE);
                            }

                            if (visibleArea->getTile(i+7, j)->isWalled(Direction::E)) {
                                DestR.x=(int)((gameWidth-tileWidth*pow(fov, j+1))/2+i*tileWidth*pow(fov, j+1));
                                DestR.w=(int)(tileWidth*pow(fov, j+1));
                                DestR.h=(int)(tileHeight*pow(fov, j+1));
                                DestR.y=(int)((screenHeight-tileHeight*pow(fov, j+1))/2);
                                SDL_RenderCopyEx(gRenderer, frontTexture, NULL, &DestR, 0.0, NULL, SDL_FLIP_NONE);
                            }

                            if (visibleArea->getTile(i+7, j)->containsObject()) {
                                DestR.x=(int)((gameWidth-tileWidth*pow(fov, j+1))/2+i*tileWidth*pow(fov, j+1));
                                DestR.w=(int)(tileWidth*pow(fov, j+1));
                                DestR.h=(int)(tileHeight*pow(fov, j+1));
                                DestR.y=(int)((screenHeight-tileHeight*pow(fov, j+1))/2);
                                SDL_RenderCopyEx(gRenderer, objectTexture, NULL, &DestR, 0.0, NULL, SDL_FLIP_NONE);
                            }
                        }

                        for (int i=j+1; i>=0; i--) {

                            if (visibleArea->getTile(i+7, j)->isWalled(Direction::E)) {
                                DestR.x=(int)((gameWidth-tileWidth*pow(fov, j+1))/2+i*tileWidth*pow(fov, j+1));
                                DestR.w=(int)(tileWidth*pow(fov, j+1));
                                DestR.h=(int)(tileHeight*pow(fov, j+1));
                                DestR.y=(int)((screenHeight-tileHeight*pow(fov, j+1))/2);
                                SDL_RenderCopyEx(gRenderer, frontTexture, NULL, &DestR, 0.0, NULL, SDL_FLIP_NONE);
                            }

                            if (visibleArea->getTile(i+7, j)->isWalled(Direction::S)) {
                                DestR.x=(int)((gameWidth+tileWidth*pow(fov, j+1))/2+i*tileWidth*pow(fov, j+1));
                                DestR.w=(int)(abs(i)*(tileWidth*pow(fov, j)-tileWidth*pow(fov, j+1))+(tileWidth*pow(fov, j)-tileWidth*pow(fov, j+1))/2+1);
                                DestR.h=(int)(tileHeight*pow(fov, j));
                                DestR.y=(int)((screenHeight-tileHeight*pow(fov, j))/2);
                                SDL_RenderCopyEx(gRenderer, wallTexture, NULL, &DestR, 0.0, NULL, SDL_FLIP_HORIZONTAL);
                            }

                            if (visibleArea->getTile(i+7, j)->containsObject()) {
                                DestR.x=(int)((gameWidth-tileWidth*pow(fov, j+1))/2+i*tileWidth*pow(fov, j+1));
                                DestR.w=(int)(tileWidth*pow(fov, j+1));
                                DestR.h=(int)(tileHeight*pow(fov, j+1));
                                DestR.y=(int)((screenHeight-tileHeight*pow(fov, j+1))/2);
                                SDL_RenderCopyEx(gRenderer, objectTexture, NULL, &DestR, 0.0, NULL, SDL_FLIP_NONE);
                            }
                        }
                        break;

                    case Direction::S:

                        for (int i=-j-1; i<=0; i++) {

                            if (visibleArea->getTile(i+7, j)->isWalled(Direction::E)) {
                                DestR.x=(int)((gameWidth-tileWidth*pow(fov, j))/2+i*tileWidth*pow(fov, j));
                                DestR.w=(int)(abs(i)*(tileWidth*pow(fov, j)-tileWidth*pow(fov, j+1))+(tileWidth*pow(fov, j)-tileWidth*pow(fov, j+1))/2+1);
                                DestR.h=(int)(tileHeight*pow(fov, j));
                                DestR.y=(int)((screenHeight-tileHeight*pow(fov, j))/2);
                                SDL_RenderCopyEx(gRenderer, wallTexture, NULL, &DestR, 0.0, NULL, SDL_FLIP_NONE);
                            }

                            if (visibleArea->getTile(i+7, j)->isWalled(Direction::S)) {
                                DestR.x=(int)((gameWidth-tileWidth*pow(fov, j+1))/2+i*tileWidth*pow(fov, j+1));
                                DestR.w=(int)(tileWidth*pow(fov, j+1));
                                DestR.h=(int)(tileHeight*pow(fov, j+1));
                                DestR.y=(int)((screenHeight-tileHeight*pow(fov, j+1))/2);
                                SDL_RenderCopyEx(gRenderer, frontTexture, NULL, &DestR, 0.0, NULL, SDL_FLIP_NONE);
                            }

                            if (visibleArea->getTile(i+7, j)->containsObject()) {
                                DestR.x=(int)((gameWidth-tileWidth*pow(fov, j+1))/2+i*tileWidth*pow(fov, j+1));
                                DestR.w=(int)(tileWidth*pow(fov, j+1));
                                DestR.h=(int)(tileHeight*pow(fov, j+1));
                                DestR.y=(int)((screenHeight-tileHeight*pow(fov, j+1))/2);
                                SDL_RenderCopyEx(gRenderer, objectTexture, NULL, &DestR, 0.0, NULL, SDL_FLIP_NONE);
                            }
                        }

                        for (int i=j+1; i>=0; i--) {

                            if (visibleArea->getTile(i+7, j)->isWalled(Direction::S)) {
                                DestR.x=(int)((gameWidth-tileWidth*pow(fov, j+1))/2+i*tileWidth*pow(fov, j+1));
                                DestR.w=(int)(tileWidth*pow(fov, j+1));
                                DestR.h=(int)(tileHeight*pow(fov, j+1));
                                DestR.y=(int)((screenHeight-tileHeight*pow(fov, j+1))/2);
                                SDL_RenderCopyEx(gRenderer, frontTexture, NULL, &DestR, 0.0, NULL, SDL_FLIP_NONE);
                            }

                            if (visibleArea->getTile(i+7, j)->isWalled(Direction::W)) {
                                DestR.x=(int)((gameWidth+tileWidth*pow(fov, j+1))/2+i*tileWidth*pow(fov, j+1));
                                DestR.w=(int)(abs(i)*(tileWidth*pow(fov, j)-tileWidth*pow(fov, j+1))+(tileWidth*pow(fov, j)-tileWidth*pow(fov, j+1))/2+1);
                                DestR.h=(int)(tileHeight*pow(fov, j));
                                DestR.y=(int)((screenHeight-tileHeight*pow(fov, j))/2);
                                SDL_RenderCopyEx(gRenderer, wallTexture, NULL, &DestR, 0.0, NULL, SDL_FLIP_HORIZONTAL);
                            }

                            if (visibleArea->getTile(i+7, j)->containsObject()) {
                                DestR.x=(int)((gameWidth-tileWidth*pow(fov, j+1))/2+i*tileWidth*pow(fov, j+1));
                                DestR.w=(int)(tileWidth*pow(fov, j+1));
                                DestR.h=(int)(tileHeight*pow(fov, j+1));
                                DestR.y=(int)((screenHeight-tileHeight*pow(fov, j+1))/2);
                                SDL_RenderCopyEx(gRenderer, objectTexture, NULL, &DestR, 0.0, NULL, SDL_FLIP_NONE);
                            }
                        }
                        break;

                    case Direction::W:

                        for (int i=-j-1; i<=0; i++) {

                            if (visibleArea->getTile(i+7, j)->isWalled(Direction::S)) {
                                DestR.x=(int)((gameWidth-tileWidth*pow(fov, j))/2+i*tileWidth*pow(fov, j));
                                DestR.w=(int)(abs(i)*(tileWidth*pow(fov, j)-tileWidth*pow(fov, j+1))+(tileWidth*pow(fov, j)-tileWidth*pow(fov, j+1))/2+1);
                                DestR.h=(int)(tileHeight*pow(fov, j));
                                DestR.y=(int)((screenHeight-tileHeight*pow(fov, j))/2);
                                SDL_RenderCopyEx(gRenderer, wallTexture, NULL, &DestR, 0.0, NULL, SDL_FLIP_NONE);
                            }

                            if (visibleArea->getTile(i+7, j)->isWalled(Direction::W)) {
                                DestR.x=(int)((gameWidth-tileWidth*pow(fov, j+1))/2+i*tileWidth*pow(fov, j+1));
                                DestR.w=(int)(tileWidth*pow(fov, j+1));
                                DestR.h=(int)(tileHeight*pow(fov, j+1));
                                DestR.y=(int)((screenHeight-tileHeight*pow(fov, j+1))/2);
                                SDL_RenderCopyEx(gRenderer, frontTexture, NULL, &DestR, 0.0, NULL, SDL_FLIP_NONE);
                            }

                            if (visibleArea->getTile(i+7, j)->containsObject()) {
                                DestR.x=(int)((gameWidth-tileWidth*pow(fov, j+1))/2+i*tileWidth*pow(fov, j+1));
                                DestR.w=(int)(tileWidth*pow(fov, j+1));
                                DestR.h=(int)(tileHeight*pow(fov, j+1));
                                DestR.y=(int)((screenHeight-tileHeight*pow(fov, j+1))/2);
                                SDL_RenderCopyEx(gRenderer, objectTexture, NULL, &DestR, 0.0, NULL, SDL_FLIP_NONE);
                            }
                        }

                        for (int i=j+1; i>=0; i--) {

                            if (visibleArea->getTile(i+7, j)->isWalled(Direction::W)) {
                                DestR.x=(int)((gameWidth-tileWidth*pow(fov, j+1))/2+i*tileWidth*pow(fov, j+1));
                                DestR.w=(int)(tileWidth*pow(fov, j+1));
                                DestR.h=(int)(tileHeight*pow(fov, j+1));
                                DestR.y=(int)((screenHeight-tileHeight*pow(fov, j+1))/2);
                                SDL_RenderCopyEx(gRenderer, frontTexture, NULL, &DestR, 0.0, NULL, SDL_FLIP_NONE);
                            }

                            if (visibleArea->getTile(i+7, j)->isWalled(Direction::N)) {
                                DestR.x=(int)((gameWidth+tileWidth*pow(fov, j+1))/2+i*tileWidth*pow(fov, j+1));
                                DestR.w=(int)(abs(i)*(tileWidth*pow(fov, j)-tileWidth*pow(fov, j+1))+(tileWidth*pow(fov, j)-tileWidth*pow(fov, j+1))/2+1);
                                DestR.h=(int)(tileHeight*pow(fov, j));
                                DestR.y=(int)((screenHeight-tileHeight*pow(fov, j))/2);
                                SDL_RenderCopyEx(gRenderer, wallTexture, NULL, &DestR, 0.0, NULL, SDL_FLIP_HORIZONTAL);
                            }

                            if (visibleArea->getTile(i+7, j)->containsObject()) {
                                DestR.x=(int)((gameWidth-tileWidth*pow(fov, j+1))/2+i*tileWidth*pow(fov, j+1));
                                DestR.w=(int)(tileWidth*pow(fov, j+1));
                                DestR.h=(int)(tileHeight*pow(fov, j+1));
                                DestR.y=(int)((screenHeight-tileHeight*pow(fov, j+1))/2);
                                SDL_RenderCopyEx(gRenderer, objectTexture, NULL, &DestR, 0.0, NULL, SDL_FLIP_NONE);
                            }
                        }
                        break;
                }
            }

            tileRect={gameWidth, 0, screenWidth-gameWidth, screenHeight};
            SDL_RenderFillRect(gRenderer, &tileRect);
            tileRect={0, 0, gameWidth, 200};
            SDL_RenderFillRect(gRenderer, &tileRect);
        }

        SDL_RenderPresent(gRenderer);
    }

    delete gameMap;

	SDL_FreeSurface(frontWallSurface);
	frontWallSurface=NULL;

	SDL_FreeSurface(sideWallSurface);
	sideWallSurface=NULL;

	SDL_FreeSurface(floorSurface);
	floorSurface=NULL;

	SDL_FreeSurface(objectSurface);
	objectSurface=NULL;

    delete visibleArea;
    visibleArea=NULL;
}
