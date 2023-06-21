#include "GameMap.hpp"

Tile* GameMap::getTile(int x, int y) {
    return &map[x][y];
}

void GameMap::setTileWall(int x, int y, Direction direction, const std::string& wallType) {
    map[x][y].setWall(direction, wallType);
}

void GameMap::setTile(int x, int y, Tile* targetTile) {
    map[x][y].setTile(targetTile);
}

void GameMap::setTileWalls(int x, int y, const std::string& northWall, const std::string& eastWall, const std::string& southWall, const std::string& westWall, const std::string& floor, const std::string& ceiling, MapObject* mapObject) {
    map[x][y].setTile(northWall, eastWall, southWall, westWall, floor, ceiling, mapObject);
}

int GameMap::getHeight() {
    return height;
}
int GameMap::getWidth() {
    return width;
}

void GameMap::saveToVector() {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            savedTiles.push_back(map[i][j]);
        }
    }
}

void GameMap::loadFromVector() {
    for (int i = width - 1; i >= 0; i--) {
        for (int j = height - 1; j >= 0; j--) {
            map[i][j].setTile(&savedTiles.back());
            savedTiles.pop_back();
        }
    }
    loadTextures();
}

//get texture sets from all tiles and add them into the texture map if not already added

void GameMap::loadTextures() {
    SDL_Renderer* gRenderer = Graphics::getInstance().getRenderer();
    SDL_Surface* surface = nullptr;

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            std::unordered_set tileTextures = map[i][j].getTextures();
            for (std::string tileTexture: tileTextures) {
                auto it = textures.find(tileTexture);
                if (it == textures.end()) {
                    surface = IMG_Load((getTexturesDirectory() + tileTexture + ".png").c_str());
                    textures.insert({ tileTexture, SDL_CreateTextureFromSurface(gRenderer, surface) });
                    SDL_FreeSurface(surface);
                    surface = nullptr;
                }
            }
        }
    }
}

//calculates and draws textures, which are in players vision cone

void GameMap::renderVisibleArea(Player player) {
    int color = 0;
    int screenWidth = Graphics::getInstance().getScreenWidth();
    int screenHeight = Graphics::getInstance().getScreenHeight();
    double fov = Graphics::getInstance().getFOV();
    int gameWidth = (int)(3 * screenWidth / 4);
    int tileWidth = (int)(gameWidth);
    int tileHeight = (int)(1.3 * screenHeight);

    SDL_Renderer* gRenderer = Graphics::getInstance().getRenderer();
    SDL_RenderClear(gRenderer);
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
    SDL_Rect tileRect = { 0, 0, screenWidth, screenHeight };
    SDL_RenderFillRect(gRenderer, &tileRect);

    SDL_Rect DestR;
    DestR.w = gameWidth;
    DestR.h = (int)((screenHeight - tileHeight * pow(fov, 7)) / 2);
    DestR.x = 0;
    DestR.y = (int)((screenHeight + tileHeight * pow(fov, 7)) / 2);
    SDL_RenderCopyEx(gRenderer, textures.find("basicFloor")->second, nullptr, &DestR, 0.0, nullptr, SDL_FLIP_NONE);

    DestR.y = 0;
    SDL_RenderCopyEx(gRenderer, textures.find("basicFloor")->second, nullptr, &DestR, 0.0, nullptr, SDL_FLIP_VERTICAL);

    Direction leftHandSide = player.getDirection();
    leftHandSide--;
    Direction rightHandSide = player.getDirection();
    rightHandSide++;
    int x = 0;
    int y = 0;

    for (int j = 6; j >= 0; j--) {
        color = (int)(255 * pow(0.55, j));
        SDL_SetTextureColorMod(textures.find("basicWall_side")->second, color, color, color);
        SDL_SetTextureColorMod(textures.find("basicWall_front")->second, color, color, color);

        for (int i = -j - 1; i <= 0; i++) {

            //rotates coordinates to be relative to the direction the player is looking

            switch (player.getDirection()) {

                case Direction::N:
                    x = player.getX() + i;
                    y = player.getY() - j;
                    break;

                case Direction::E:
                    x = player.getX() + j;
                    y = player.getY() + i;
                    break;

                case Direction::S:
                    x = player.getX() - i;
                    y = player.getY() + j;
                    break;

                case Direction::W:
                    x = player.getX() - j;
                    y = player.getY() - i;
                    break;
            }

            if (x < 0 || y < 0 || x >= width || y >= height) {
                continue;
            }

            if (map[x][y].isWalled(leftHandSide)) {
                DestR.x = (int)(gameWidth * 0.5 + (i - 0.5) * tileWidth * pow(fov, j));
                DestR.w = (int)((abs(i) + 0.5) * (tileWidth * pow(fov, j) - tileWidth * pow(fov, j + 1)) + 1);
                DestR.h = (int)(tileHeight * pow(fov, j));
                DestR.y = (int)((screenHeight - tileHeight * pow(fov, j)) * 0.5);
                SDL_RenderCopyEx(gRenderer, textures.find(map[x][y].getWallType(leftHandSide) + "_side")->second, nullptr, &DestR, 0.0, nullptr, SDL_FLIP_NONE);
            }

            if (map[x][y].isWalled(player.getDirection())) {
                DestR.x = (int)(gameWidth * 0.5 + (i - 0.5) * tileWidth * pow(fov, j + 1));
                DestR.w = (int)(tileWidth * pow(fov, j + 1) + 1);
                DestR.h = (int)(tileHeight * pow(fov, j + 1));
                DestR.y = (int)((screenHeight - tileHeight * pow(fov, j + 1)) * 0.5);
                SDL_RenderCopyEx(gRenderer, textures.find(map[x][y].getWallType(player.getDirection()) + "_front")->second, nullptr, &DestR, 0.0, nullptr, SDL_FLIP_NONE);
            }

            if (map[x][y].containsObject()) {
                DestR.x = (int)(gameWidth * 0.5 + (i - 0.5) * tileWidth * pow(fov, j + 1));
                DestR.w = (int)(tileWidth * pow(fov, j + 1));
                DestR.h = (int)(tileHeight * pow(fov, j + 1));
                DestR.y = (int)((screenHeight - tileHeight * pow(fov, j + 1)) * 0.5);
                SDL_RenderCopyEx(gRenderer, textures.find(map[x][y].getObjectType())->second, nullptr, &DestR, 0.0, nullptr, SDL_FLIP_NONE);
            }
        }

        for (int i = j + 1; i >= 0; i--) {
            switch (player.getDirection()) {

                case Direction::N:
                    x = player.getX() + i;
                    y = player.getY() - j;
                    break;

                case Direction::E:
                    x = player.getX() + j;
                    y = player.getY() + i;
                    break;

                case Direction::S:
                    x = player.getX() - i;
                    y = player.getY() + j;
                    break;

                case Direction::W:
                    x = player.getX() - j;
                    y = player.getY() - i;
                    break;
            }

            if (x < 0 || y < 0 || x >= width || y >= height) {
                continue;
            }

            if (map[x][y].isWalled(rightHandSide)) {
                DestR.x = (int)(gameWidth * 0.5 + (i + 0.5) * tileWidth * pow(fov, j + 1));
                DestR.w = (int)((abs(i) + 0.5) * (tileWidth * pow(fov, j) - tileWidth * pow(fov, j + 1)) + 1);
                DestR.h = (int)(tileHeight * pow(fov, j));
                DestR.y = (int)((screenHeight - tileHeight * pow(fov, j)) * 0.5);
                SDL_RenderCopyEx(gRenderer, textures.find(map[x][y].getWallType(rightHandSide) + "_side")->second, nullptr, &DestR, 0.0, nullptr, SDL_FLIP_HORIZONTAL);
            }

            if (map[x][y].isWalled(player.getDirection())) {
                DestR.x = (int)(gameWidth * 0.5 + (i - 0.5) * tileWidth * pow(fov, j + 1));
                DestR.w = (int)(tileWidth * pow(fov, j + 1) + 1);
                DestR.h = (int)(tileHeight * pow(fov, j + 1));
                DestR.y = (int)((screenHeight - tileHeight * pow(fov, j + 1)) * 0.5);
                SDL_RenderCopyEx(gRenderer, textures.find(map[x][y].getWallType(player.getDirection()) + "_front")->second, nullptr, &DestR, 0.0, nullptr, SDL_FLIP_NONE);
            }

            if (map[x][y].containsObject()) {
                DestR.x = (int)(gameWidth * 0.5 + (i - 0.5) * tileWidth * pow(fov, j + 1));
                DestR.w = (int)(tileWidth * pow(fov, j + 1));
                DestR.h = (int)(tileHeight * pow(fov, j + 1));
                DestR.y = (int)((screenHeight - tileHeight * pow(fov, j + 1)) * 0.5);
                SDL_RenderCopyEx(gRenderer, textures.find(map[x][y].getObjectType())->second, nullptr, &DestR, 0.0, nullptr, SDL_FLIP_NONE);
            }
        }
    }

    tileRect = { gameWidth, 0, screenWidth - gameWidth, screenHeight };
    SDL_RenderFillRect(gRenderer, &tileRect);
    tileRect = { 0, 0, gameWidth, screenHeight/7 };
    SDL_RenderFillRect(gRenderer, &tileRect);

    SDL_RenderPresent(gRenderer);
}

GameMap::GameMap(int width, int height) {
    this->width = width;
    this->height = height;
    savedTiles.clear();
    map = new Tile*[width];
    for (int i = 0; i < width; i++) {
        map[i] = new Tile[height];
    }
}

GameMap::GameMap(int width, int height, std::vector <Tile> savedTiles) {
    this->width = width;
    this->height = height;
    map = new Tile * [width];
    for (int i = 0; i < width; i++) {
        map[i] = new Tile[height];
    }
    this->savedTiles = savedTiles;
    loadFromVector();
}

GameMap::~GameMap() {
    for (int i = 0; i < width; i++) {
        delete[] map[i];
        map[i] = nullptr;
    }
    delete[] map;
    map = nullptr;

    for (auto i = textures.begin(); i != textures.end(); i++) {
        SDL_DestroyTexture(i->second);
        i->second = nullptr;
    }
}
