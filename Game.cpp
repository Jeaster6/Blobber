#include "Game.hpp"

void gameplay() {

    GameMap *gameMap;
    GameMap *visibleArea = new GameMap(15, 7);

    std::ifstream ifs(getMapsDirectory() + "Map_1.dat");
    boost::archive::binary_iarchive ia(ifs);
    ia >> gameMap;

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
    int playerX = 0;
    int playerY = 0;
    int playerDirection = 0;
    int gameWidth = 0;
    int screenWidth = 0;
    int screenHeight = 0;
    double fov = 0;
    bool quit = false;

    SDL_RenderClear(gRenderer);

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

            switch (playerDirection) {
                case 0:
                    for (int i=0; i<15; i++) {
                        for (int j=0; j<7; j++) {
                            if (playerY-j<0 || playerX+i-7>=gameMap->getWidth() || playerX+i-7<0) {
                                visibleArea->setTile(i, j, true, true, true, true);
                            }
                            else {
                                visibleArea->setTile(i, j, gameMap->getTile(playerX+i-7, playerY-j));
                            }
                        }
                    }
                    break;

                case 1:
                    for (int i=0; i<15; i++) {
                        for (int j=0; j<7; j++) {
                            if (playerX+j>=gameMap->getWidth() || playerY+i-7>=gameMap->getHeight() || playerY+i-7<0) {
                                visibleArea->setTile(i, j, true, true, true, true);
                            }
                            else {
                                visibleArea->setTile(i, j, gameMap->getTile(playerX+j, playerY+i-7));
                            }
                        }
                    }
                    break;

                case 2:
                    for (int i=0; i<15; i++) {
                        for (int j=0; j<7; j++) {
                            if (playerY+j>=gameMap->getHeight() || playerX-i+7>=gameMap->getWidth() || playerX-i+7<0) {
                                visibleArea->setTile(i, j, true, true, true, true);
                            }
                            else {
                                visibleArea->setTile(i, j, gameMap->getTile(playerX-i+7, playerY+j));
                            }
                        }
                    }
                    break;

                case 3:
                    for (int i=0; i<15; i++) {
                        for (int j=0; j<7; j++) {
                            if (playerX-j<0 || playerY-i+7>=gameMap->getHeight() || playerY-i+7<0) {
                                visibleArea->setTile(i, j, true, true, true, true);
                            }
                            else {
                                visibleArea->setTile(i, j, gameMap->getTile(playerX-j, playerY-i+7));
                            }
                        }
                    }
                    break;
                }

            if (e.type==SDL_KEYDOWN) {

                switch (e.key.keysym.sym) {

                    case SDLK_ESCAPE:
                        quit=true;
                        break;

                    case SDLK_w:
                        if (playerDirection==0 && !visibleArea->getTile(7, 0)->isWalled('N')) {
                            playerY--;
                        }
                        if (playerDirection==1 && !visibleArea->getTile(7, 0)->isWalled('E')) {
                            playerX++;
                        }
                        if (playerDirection==2 && !visibleArea->getTile(7, 0)->isWalled('S')) {
                            playerY++;
                        }
                        if (playerDirection==3 && !visibleArea->getTile(7, 0)->isWalled('W')) {
                            playerX--;
                        }
                        break;

                    case SDLK_a:
                        if (playerDirection==0 && !visibleArea->getTile(7, 0)->isWalled('W')) {
                            playerX--;
                        }
                        if (playerDirection==1 && !visibleArea->getTile(7, 0)->isWalled('N')) {
                            playerY--;
                        }
                        if (playerDirection==2 && !visibleArea->getTile(7, 0)->isWalled('E')) {
                            playerX++;
                        }
                        if (playerDirection==3 && !visibleArea->getTile(7, 0)->isWalled('S')) {
                            playerY++;
                        }
                        break;

                    case SDLK_s:
                        if (playerDirection==0 && !visibleArea->getTile(7, 0)->isWalled('S')) {
                            playerY++;
                        }
                        if (playerDirection==1 && !visibleArea->getTile(7, 0)->isWalled('W')) {
                            playerX--;
                        }
                        if (playerDirection==2 && !visibleArea->getTile(7, 0)->isWalled('N')) {
                            playerY--;
                        }
                        if (playerDirection==3 && !visibleArea->getTile(7, 0)->isWalled('E')) {
                            playerX++;
                        }
                        break;

                    case SDLK_d:
                        if (playerDirection==0 && !visibleArea->getTile(7, 0)->isWalled('E')) {
                            playerX++;
                        }
                        if (playerDirection==1 && !visibleArea->getTile(7, 0)->isWalled('S')) {
                            playerY++;
                        }
                        if (playerDirection==2 && !visibleArea->getTile(7, 0)->isWalled('W')) {
                            playerX--;
                        }
                        if (playerDirection==3 && !visibleArea->getTile(7, 0)->isWalled('N')) {
                            playerY--;
                        }
                        break;

                    case SDLK_q:
                        if (playerDirection>0) {
                            playerDirection--;
                        }
                        else {
                            playerDirection=3;
                        }
                        break;

                    case SDLK_e:
                        if (playerDirection<3) {
                            playerDirection++;
                        }
                        else {
                            playerDirection=0;
                        }
                        break;
                }
            }

            SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
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

                switch (playerDirection) {

                    case 0:
                        for (int i=-j-1; i<=0; i++) {

                            if (visibleArea->getTile(i+7, j)->isWalled('W')) {
                                DestR.x=(int)((gameWidth-tileWidth*pow(fov, j))/2+i*tileWidth*pow(fov, j));
                                DestR.w=(int)(abs(i)*(tileWidth*pow(fov, j)-tileWidth*pow(fov, j+1))+(tileWidth*pow(fov, j)-tileWidth*pow(fov, j+1))/2+1);
                                DestR.h=(int)(tileHeight*pow(fov, j));
                                DestR.y=(int)((screenHeight-tileHeight*pow(fov, j))/2);
                                SDL_RenderCopyEx(gRenderer, wallTexture, NULL, &DestR, 0.0, NULL, SDL_FLIP_NONE);
                            }

                            if (visibleArea->getTile(i+7, j)->isWalled('N')) {
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

                            if (visibleArea->getTile(i+7, j)->isWalled('N')) {
                                DestR.x=(int)((gameWidth-tileWidth*pow(fov, j+1))/2+i*tileWidth*pow(fov, j+1));
                                DestR.w=(int)(tileWidth*pow(fov, j+1));
                                DestR.h=(int)(tileHeight*pow(fov, j+1));
                                DestR.y=(int)((screenHeight-tileHeight*pow(fov, j+1))/2);
                                SDL_RenderCopyEx(gRenderer, frontTexture, NULL, &DestR, 0.0, NULL, SDL_FLIP_NONE);
                            }

                            if (visibleArea->getTile(i+7, j)->isWalled('E')) {
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

                    case 1:

                        for (int i=-j-1; i<=0; i++) {

                            if (visibleArea->getTile(i+7, j)->isWalled('N')) {
                                DestR.x=(int)((gameWidth-tileWidth*pow(fov, j))/2+i*tileWidth*pow(fov, j));
                                DestR.w=(int)(abs(i)*(tileWidth*pow(fov, j)-tileWidth*pow(fov, j+1))+(tileWidth*pow(fov, j)-tileWidth*pow(fov, j+1))/2+1);
                                DestR.h=(int)(tileHeight*pow(fov, j));
                                DestR.y=(int)((screenHeight-tileHeight*pow(fov, j))/2);
                                SDL_RenderCopyEx(gRenderer, wallTexture, NULL, &DestR, 0.0, NULL, SDL_FLIP_NONE);
                            }

                            if (visibleArea->getTile(i+7, j)->isWalled('E')) {
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

                            if (visibleArea->getTile(i+7, j)->isWalled('E')) {
                                DestR.x=(int)((gameWidth-tileWidth*pow(fov, j+1))/2+i*tileWidth*pow(fov, j+1));
                                DestR.w=(int)(tileWidth*pow(fov, j+1));
                                DestR.h=(int)(tileHeight*pow(fov, j+1));
                                DestR.y=(int)((screenHeight-tileHeight*pow(fov, j+1))/2);
                                SDL_RenderCopyEx(gRenderer, frontTexture, NULL, &DestR, 0.0, NULL, SDL_FLIP_NONE);
                            }

                            if (visibleArea->getTile(i+7, j)->isWalled('S')) {
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

                    case 2:

                        for (int i=-j-1; i<=0; i++) {

                            if (visibleArea->getTile(i+7, j)->isWalled('E')) {
                                DestR.x=(int)((gameWidth-tileWidth*pow(fov, j))/2+i*tileWidth*pow(fov, j));
                                DestR.w=(int)(abs(i)*(tileWidth*pow(fov, j)-tileWidth*pow(fov, j+1))+(tileWidth*pow(fov, j)-tileWidth*pow(fov, j+1))/2+1);
                                DestR.h=(int)(tileHeight*pow(fov, j));
                                DestR.y=(int)((screenHeight-tileHeight*pow(fov, j))/2);
                                SDL_RenderCopyEx(gRenderer, wallTexture, NULL, &DestR, 0.0, NULL, SDL_FLIP_NONE);
                            }

                            if (visibleArea->getTile(i+7, j)->isWalled('S')) {
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

                            if (visibleArea->getTile(i+7, j)->isWalled('S')) {
                                DestR.x=(int)((gameWidth-tileWidth*pow(fov, j+1))/2+i*tileWidth*pow(fov, j+1));
                                DestR.w=(int)(tileWidth*pow(fov, j+1));
                                DestR.h=(int)(tileHeight*pow(fov, j+1));
                                DestR.y=(int)((screenHeight-tileHeight*pow(fov, j+1))/2);
                                SDL_RenderCopyEx(gRenderer, frontTexture, NULL, &DestR, 0.0, NULL, SDL_FLIP_NONE);
                            }

                            if (visibleArea->getTile(i+7, j)->isWalled('W')) {
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

                    case 3:

                        for (int i=-j-1; i<=0; i++) {

                            if (visibleArea->getTile(i+7, j)->isWalled('S')) {
                                DestR.x=(int)((gameWidth-tileWidth*pow(fov, j))/2+i*tileWidth*pow(fov, j));
                                DestR.w=(int)(abs(i)*(tileWidth*pow(fov, j)-tileWidth*pow(fov, j+1))+(tileWidth*pow(fov, j)-tileWidth*pow(fov, j+1))/2+1);
                                DestR.h=(int)(tileHeight*pow(fov, j));
                                DestR.y=(int)((screenHeight-tileHeight*pow(fov, j))/2);
                                SDL_RenderCopyEx(gRenderer, wallTexture, NULL, &DestR, 0.0, NULL, SDL_FLIP_NONE);
                            }

                            if (visibleArea->getTile(i+7, j)->isWalled('W')) {
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

                            if (visibleArea->getTile(i+7, j)->isWalled('W')) {
                                DestR.x=(int)((gameWidth-tileWidth*pow(fov, j+1))/2+i*tileWidth*pow(fov, j+1));
                                DestR.w=(int)(tileWidth*pow(fov, j+1));
                                DestR.h=(int)(tileHeight*pow(fov, j+1));
                                DestR.y=(int)((screenHeight-tileHeight*pow(fov, j+1))/2);
                                SDL_RenderCopyEx(gRenderer, frontTexture, NULL, &DestR, 0.0, NULL, SDL_FLIP_NONE);
                            }

                            if (visibleArea->getTile(i+7, j)->isWalled('N')) {
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
