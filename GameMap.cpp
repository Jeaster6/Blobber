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
    SDL_Renderer* renderer = Graphics::getInstance().getRenderer();
    SDL_Surface* surface = nullptr;

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            std::unordered_set tileTextures = map[i][j].getTextures();
            for (std::string tileTexture: tileTextures) {
                auto it = textures.find(tileTexture);
                if (it == textures.end()) {
                    surface = IMG_Load((getTexturesDirectory() + tileTexture + ".png").c_str());
                    textures.insert({ tileTexture, SDL_CreateTextureFromSurface(renderer, surface) });
                    SDL_FreeSurface(surface);
                    surface = nullptr;
                }
            }
        }
    }
}

//calculates and draws textures, which are in players vision cone

void GameMap::makeScreenSnapshot(Player player) {
    generateScreenTexture(player, previousScreenTexture);
}

void GameMap::generateScreenTexture(Player player, SDL_Texture* targetTexture) {
    int color = 0;
    int screenWidth = Graphics::getInstance().getScreenWidth();
    int screenHeight = Graphics::getInstance().getScreenHeight();
    double fov = Graphics::getInstance().getFOV();
    int gameWidth = (int)(3 * screenWidth / 4);
    int tileWidth = (int)(gameWidth);
    int tileHeight = (int)(1.3 * screenHeight);

    SDL_Renderer* renderer = Graphics::getInstance().getRenderer();
    SDL_SetRenderTarget(renderer, targetTexture);
    SDL_Rect targetArea = { 0, 0, 0, 0 };
    SDL_RenderClear(renderer);

    targetArea.x = 0;
    targetArea.y = (int)((screenHeight + tileHeight * pow(fov, 7)) / 2);
    targetArea.w = gameWidth;
    targetArea.h = (int)((screenHeight - tileHeight * pow(fov, 7)) / 2);
    SDL_RenderCopyEx(renderer, textures.find("basicFloor")->second, nullptr, &targetArea, 0.0, nullptr, SDL_FLIP_NONE);

    targetArea.y = 0;
    SDL_RenderCopyEx(renderer, textures.find("basicFloor")->second, nullptr, &targetArea, 0.0, nullptr, SDL_FLIP_VERTICAL);

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

            //rotates coordinates to be relative to the direction the player is facing

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
                targetArea.x = (int)(gameWidth * 0.5 + (i - 0.5) * tileWidth * pow(fov, j));
                targetArea.y = (int)((screenHeight - tileHeight * pow(fov, j)) * 0.5);
                targetArea.w = (int)((abs(i) + 0.5) * (tileWidth * pow(fov, j) - tileWidth * pow(fov, j + 1)) + 1);
                targetArea.h = (int)(tileHeight * pow(fov, j));
                SDL_RenderCopyEx(renderer, textures.find(map[x][y].getWallType(leftHandSide) + "_side")->second, nullptr, &targetArea, 0.0, nullptr, SDL_FLIP_NONE);
            }

            if (map[x][y].isWalled(player.getDirection())) {
                targetArea.x = (int)(gameWidth * 0.5 + (i - 0.5) * tileWidth * pow(fov, j + 1));
                targetArea.y = (int)((screenHeight - tileHeight * pow(fov, j + 1)) * 0.5);
                targetArea.w = (int)(tileWidth * pow(fov, j + 1) + 1);
                targetArea.h = (int)(tileHeight * pow(fov, j + 1));
                SDL_RenderCopyEx(renderer, textures.find(map[x][y].getWallType(player.getDirection()) + "_front")->second, nullptr, &targetArea, 0.0, nullptr, SDL_FLIP_NONE);
            }

            if (map[x][y].containsObject()) {
                targetArea.x = (int)(gameWidth * 0.5 + (i - 0.5) * tileWidth * pow(fov, j + 1));
                targetArea.y = (int)((screenHeight - tileHeight * pow(fov, j + 1)) * 0.5);
                targetArea.w = (int)(tileWidth * pow(fov, j + 1));
                targetArea.h = (int)(tileHeight * pow(fov, j + 1));
                SDL_RenderCopyEx(renderer, textures.find(map[x][y].getObjectType())->second, nullptr, &targetArea, 0.0, nullptr, SDL_FLIP_NONE);
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
                targetArea.x = (int)(gameWidth * 0.5 + (i + 0.5) * tileWidth * pow(fov, j + 1));
                targetArea.y = (int)((screenHeight - tileHeight * pow(fov, j)) * 0.5);
                targetArea.w = (int)((abs(i) + 0.5) * (tileWidth * pow(fov, j) - tileWidth * pow(fov, j + 1)) + 1);
                targetArea.h = (int)(tileHeight * pow(fov, j));
                SDL_RenderCopyEx(renderer, textures.find(map[x][y].getWallType(rightHandSide) + "_side")->second, nullptr, &targetArea, 0.0, nullptr, SDL_FLIP_HORIZONTAL);
            }

            if (map[x][y].isWalled(player.getDirection())) {
                targetArea.x = (int)(gameWidth * 0.5 + (i - 0.5) * tileWidth * pow(fov, j + 1));
                targetArea.y = (int)((screenHeight - tileHeight * pow(fov, j + 1)) * 0.5);
                targetArea.w = (int)(tileWidth * pow(fov, j + 1) + 1);
                targetArea.h = (int)(tileHeight * pow(fov, j + 1));
                SDL_RenderCopyEx(renderer, textures.find(map[x][y].getWallType(player.getDirection()) + "_front")->second, nullptr, &targetArea, 0.0, nullptr, SDL_FLIP_NONE);
            }

            if (map[x][y].containsObject()) {
                targetArea.x = (int)(gameWidth * 0.5 + (i - 0.5) * tileWidth * pow(fov, j + 1));
                targetArea.y = (int)((screenHeight - tileHeight * pow(fov, j + 1)) * 0.5);
                targetArea.w = (int)(tileWidth * pow(fov, j + 1));
                targetArea.h = (int)(tileHeight * pow(fov, j + 1));
                SDL_RenderCopyEx(renderer, textures.find(map[x][y].getObjectType())->second, nullptr, &targetArea, 0.0, nullptr, SDL_FLIP_NONE);
            }
        }
    }

    SDL_SetRenderTarget(renderer, nullptr);
}

void GameMap::animateLeftRotation(Player player) {
    int screenWidth = Graphics::getInstance().getScreenWidth();
    int screenHeight = Graphics::getInstance().getScreenHeight();
    int gameWidth = (int)(3 * screenWidth / 4);
    int animationFrames = 16;
    SDL_Renderer* renderer = Graphics::getInstance().getRenderer();
    SDL_Rect targetArea = { 0, 0, 0, 0 };
    SDL_Rect sourceArea = { 0, 0, 0, 0 };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);

    generateScreenTexture(player, currentScreenTexture);

    for (int i = 1; i < animationFrames; i++) {
        targetArea = { 0, 0, screenWidth, screenHeight };
        SDL_RenderFillRect(renderer, &targetArea);

        sourceArea = { 0, 0, (gameWidth * abs(i - animationFrames)) / animationFrames, screenHeight };
        targetArea = { (gameWidth * i) / animationFrames, 0, (gameWidth * abs(i - animationFrames)) / animationFrames, screenHeight };
        SDL_RenderCopyEx(renderer, previousScreenTexture, &sourceArea, &targetArea, 0.0, nullptr, SDL_FLIP_NONE);

        sourceArea = { (gameWidth * abs(i - animationFrames)) / animationFrames, 0, (gameWidth * i) / animationFrames, screenHeight };
        targetArea = { 0, 0, (gameWidth * i) / animationFrames, screenHeight };
        SDL_RenderCopyEx(renderer, currentScreenTexture, &sourceArea, &targetArea, 0.0, nullptr, SDL_FLIP_NONE);

        targetArea = { gameWidth, 0, screenWidth - gameWidth, screenHeight };
        SDL_RenderFillRect(renderer, &targetArea);
        targetArea = { 0, 0, gameWidth, screenHeight / 7 };
        SDL_RenderFillRect(renderer, &targetArea);
        SDL_RenderPresent(renderer);

        SDL_Delay(200 / animationFrames);
    }
}

void GameMap::animateRightRotation(Player player) {
    int screenWidth = Graphics::getInstance().getScreenWidth();
    int screenHeight = Graphics::getInstance().getScreenHeight();
    int gameWidth = (int)(3 * screenWidth / 4);
    int animationFrames = 16;
    SDL_Renderer* renderer = Graphics::getInstance().getRenderer();
    SDL_Rect targetArea = { 0, 0, 0, 0 };
    SDL_Rect sourceArea = { 0, 0, 0, 0 };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);

    generateScreenTexture(player, currentScreenTexture);

    for (int i = 1; i < animationFrames; i++) {
        targetArea = { 0, 0, screenWidth, screenHeight };
        SDL_RenderFillRect(renderer, &targetArea);

        sourceArea = { (gameWidth * i) / animationFrames, 0, (gameWidth * abs(i - animationFrames)) / animationFrames, screenHeight };
        targetArea = { 0, 0, (gameWidth * abs(i - animationFrames)) / animationFrames, screenHeight };
        SDL_RenderCopyEx(renderer, previousScreenTexture, &sourceArea, &targetArea, 0.0, nullptr, SDL_FLIP_NONE);

        sourceArea = { 0, 0, (gameWidth * i) / animationFrames, screenHeight };
        targetArea = { (gameWidth * abs(i - animationFrames)) / animationFrames, 0, (gameWidth * i) / animationFrames, screenHeight };
        SDL_RenderCopyEx(renderer, currentScreenTexture, &sourceArea, &targetArea, 0.0, nullptr, SDL_FLIP_NONE);

        targetArea = { gameWidth, 0, screenWidth - gameWidth, screenHeight };
        SDL_RenderFillRect(renderer, &targetArea);
        targetArea = { 0, 0, gameWidth, screenHeight / 7 };
        SDL_RenderFillRect(renderer, &targetArea);
        SDL_RenderPresent(renderer);

        SDL_Delay(200 / animationFrames);
    }
}

void GameMap::animateForwardMovement(Player player) {
    int screenWidth = Graphics::getInstance().getScreenWidth();
    int screenHeight = Graphics::getInstance().getScreenHeight();
    int gameWidth = (int)(3 * screenWidth / 4);
    int animationFrames = 16;
    double fov = Graphics::getInstance().getFOV();
    SDL_Renderer* renderer = Graphics::getInstance().getRenderer();
    SDL_Rect targetArea = { 0, 0, 0, 0 };
    SDL_Rect sourceArea = { 0, 0, 0, 0 };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);

    for (int i = 1; i < animationFrames; i++) {
        targetArea = { 0, 0, screenWidth, screenHeight };
        SDL_RenderFillRect(renderer, &targetArea);

        sourceArea = { (int)((gameWidth * (1 - fov) * i) / (animationFrames * 2)), (int)((screenHeight * (1 - fov) * i) / (animationFrames * 2)), (int)(gameWidth - ((gameWidth * i * (1 - fov)) / animationFrames)), (int)(screenHeight - ((screenHeight * i * (1 - fov)) / animationFrames)) };
        targetArea = { 0, 0, gameWidth, screenHeight };
        SDL_RenderCopyEx(renderer, previousScreenTexture, &sourceArea, &targetArea, 0.0, nullptr, SDL_FLIP_NONE);

        targetArea = { gameWidth, 0, screenWidth - gameWidth, screenHeight };
        SDL_RenderFillRect(renderer, &targetArea);
        targetArea = { 0, 0, gameWidth, screenHeight / 7 };
        SDL_RenderFillRect(renderer, &targetArea);
        SDL_RenderPresent(renderer);

        SDL_Delay(200 / animationFrames);
    }
}

void GameMap::animateBackwardMovement(Player player) {
    int screenWidth = Graphics::getInstance().getScreenWidth();
    int screenHeight = Graphics::getInstance().getScreenHeight();
    int gameWidth = (int)(3 * screenWidth / 4);
    int animationFrames = 16;
    double fov = Graphics::getInstance().getFOV();
    SDL_Renderer* renderer = Graphics::getInstance().getRenderer();
    SDL_Rect targetArea = { 0, 0, 0, 0 };
    SDL_Rect sourceArea = { 0, 0, 0, 0 };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);

    generateScreenTexture(player, currentScreenTexture);

    for (int i = animationFrames - 1; i > 0; i--) {
        targetArea = { 0, 0, screenWidth, screenHeight };
        SDL_RenderFillRect(renderer, &targetArea);

        sourceArea = { (int)((gameWidth * (1 - fov) * i) / (animationFrames * 2)), (int)((screenHeight * (1 - fov) * i) / (animationFrames * 2)), (int)(gameWidth - ((gameWidth * i * (1 - fov)) / animationFrames)), (int)(screenHeight - ((screenHeight * i * (1 - fov)) / animationFrames)) };
        targetArea = { 0, 0, gameWidth, screenHeight };
        SDL_RenderCopyEx(renderer, currentScreenTexture, &sourceArea, &targetArea, 0.0, nullptr, SDL_FLIP_NONE);

        targetArea = { gameWidth, 0, screenWidth - gameWidth, screenHeight };
        SDL_RenderFillRect(renderer, &targetArea);
        targetArea = { 0, 0, gameWidth, screenHeight / 7 };
        SDL_RenderFillRect(renderer, &targetArea);
        SDL_RenderPresent(renderer);

        SDL_Delay(200 / animationFrames);
    }
}

void GameMap::animateSidestepLeft(Player player) {
    int screenWidth = Graphics::getInstance().getScreenWidth();
    int screenHeight = Graphics::getInstance().getScreenHeight();
    int gameWidth = (int)(3 * screenWidth / 4);
    int animationFrames = 16;
    double fov = Graphics::getInstance().getFOV();
    SDL_Renderer* renderer = Graphics::getInstance().getRenderer();
    SDL_Rect targetArea = { 0, 0, 0, 0 };
    SDL_Rect sourceArea = { 0, 0, 0, 0 };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);

    generateScreenTexture(player, currentScreenTexture);

    for (int i = 1; i < animationFrames; i++) {
        targetArea = { 0, 0, screenWidth, screenHeight };
        SDL_RenderFillRect(renderer, &targetArea);

        sourceArea = { (int)(gameWidth * (1 - fov) * 0.5), 0, (int)((gameWidth * (1 - fov) * 0.5) + ((gameWidth * fov * abs(i - animationFrames)) / animationFrames)), screenHeight };
        targetArea = { (int)((gameWidth * (1 - fov) * 0.5) + ((gameWidth * fov * i) / animationFrames)), 0, (int)((gameWidth * (1 - fov) * 0.5) + ((gameWidth * fov * abs(i - animationFrames)) / animationFrames)), screenHeight };
        SDL_RenderCopyEx(renderer, previousScreenTexture, &sourceArea, &targetArea, 0.0, nullptr, SDL_FLIP_NONE);

        sourceArea = { (int)((gameWidth * fov * abs(i - animationFrames)) / animationFrames), 0, (int)((gameWidth * (1 - fov) * 0.5) + ((gameWidth * fov * i) / animationFrames)), screenHeight };
        targetArea = { 0, 0, (int)((gameWidth * (1 - fov) * 0.5) + ((gameWidth * fov * i) / animationFrames)), screenHeight };
        SDL_RenderCopyEx(renderer, currentScreenTexture, &sourceArea, &targetArea, 0.0, nullptr, SDL_FLIP_NONE);

        targetArea = { gameWidth, 0, screenWidth - gameWidth, screenHeight };
        SDL_RenderFillRect(renderer, &targetArea);
        targetArea = { 0, 0, gameWidth, screenHeight / 7 };
        SDL_RenderFillRect(renderer, &targetArea);
        SDL_RenderPresent(renderer);

        SDL_Delay(200 / animationFrames);
    }
}

void GameMap::animateSidestepRight(Player player) {
    int screenWidth = Graphics::getInstance().getScreenWidth();
    int screenHeight = Graphics::getInstance().getScreenHeight();
    int gameWidth = (int)(3 * screenWidth / 4);
    int animationFrames = 16;
    double fov = Graphics::getInstance().getFOV();
    SDL_Renderer* renderer = Graphics::getInstance().getRenderer();
    SDL_Rect targetArea = { 0, 0, 0, 0 };
    SDL_Rect sourceArea = { 0, 0, 0, 0 };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);

    generateScreenTexture(player, currentScreenTexture);

    for (int i = animationFrames - 1; i > 0; i--) {
        targetArea = { 0, 0, screenWidth, screenHeight };
        SDL_RenderFillRect(renderer, &targetArea);

        sourceArea = { (int)((gameWidth * fov * abs(i - animationFrames)) / animationFrames), 0, (int)((gameWidth * (1 - fov) * 0.5) + ((gameWidth * fov * i) / animationFrames)), screenHeight };
        targetArea = { 0, 0, (int)((gameWidth * (1 - fov) * 0.5) + ((gameWidth * fov * i) / animationFrames)), screenHeight };
        SDL_RenderCopyEx(renderer, previousScreenTexture, &sourceArea, &targetArea, 0.0, nullptr, SDL_FLIP_NONE);

        sourceArea = { (int)(gameWidth * (1 - fov) * 0.5), 0, (int)((gameWidth * (1 - fov) * 0.5) + ((gameWidth * fov * abs(i - animationFrames)) / animationFrames)), screenHeight };
        targetArea = { (int)((gameWidth * (1 - fov) * 0.5) + ((gameWidth * fov * i) / animationFrames)), 0, (int)((gameWidth * (1 - fov) * 0.5) + ((gameWidth * fov * abs(i - animationFrames)) / animationFrames)), screenHeight };
        SDL_RenderCopyEx(renderer, currentScreenTexture, &sourceArea, &targetArea, 0.0, nullptr, SDL_FLIP_NONE);

        targetArea = { gameWidth, 0, screenWidth - gameWidth, screenHeight };
        SDL_RenderFillRect(renderer, &targetArea);
        targetArea = { 0, 0, gameWidth, screenHeight / 7 };
        SDL_RenderFillRect(renderer, &targetArea);
        SDL_RenderPresent(renderer);

        SDL_Delay(200 / animationFrames);
    }
}

void GameMap::renderVisibleArea(Player player) {
    int screenWidth = Graphics::getInstance().getScreenWidth();
    int screenHeight = Graphics::getInstance().getScreenHeight();
    int gameWidth = (int)(3 * screenWidth / 4);
    SDL_Renderer* renderer = Graphics::getInstance().getRenderer();
    SDL_Rect targetArea = { 0, 0, 0, 0 };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);

    generateScreenTexture(player, currentScreenTexture);

    targetArea = { 0, 0, screenWidth, screenHeight };
    SDL_RenderFillRect(renderer, &targetArea);
    SDL_RenderCopy(renderer, currentScreenTexture, nullptr, nullptr);
    targetArea = { gameWidth, 0, screenWidth - gameWidth, screenHeight };
    SDL_RenderFillRect(renderer, &targetArea);
    targetArea = { 0, 0, gameWidth, screenHeight / 7 };
    SDL_RenderFillRect(renderer, &targetArea);
    SDL_RenderPresent(renderer);
}

GameMap::GameMap(int width, int height) {
    previousScreenTexture = SDL_CreateTexture(Graphics::getInstance().getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, Graphics::getInstance().getScreenWidth(), Graphics::getInstance().getScreenHeight());
    currentScreenTexture = SDL_CreateTexture(Graphics::getInstance().getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, Graphics::getInstance().getScreenWidth(), Graphics::getInstance().getScreenHeight());
    this->width = width;
    this->height = height;
    savedTiles.clear();
    map = new Tile*[width];
    for (int i = 0; i < width; i++) {
        map[i] = new Tile[height];
    }
}

GameMap::GameMap(int width, int height, std::vector <Tile> savedTiles) {
    previousScreenTexture = SDL_CreateTexture(Graphics::getInstance().getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, Graphics::getInstance().getScreenWidth(), Graphics::getInstance().getScreenHeight());
    currentScreenTexture = SDL_CreateTexture(Graphics::getInstance().getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, Graphics::getInstance().getScreenWidth(), Graphics::getInstance().getScreenHeight());
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
    previousScreenTexture = nullptr;
    SDL_DestroyTexture(previousScreenTexture);

    currentScreenTexture = nullptr;
    SDL_DestroyTexture(currentScreenTexture);

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
