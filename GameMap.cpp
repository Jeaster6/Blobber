#include "GameMap.hpp"

Tile& GameMap::getTile(int x, int y) const {
    return map[x][y];
}

void GameMap::setTileWall(int x, int y, Direction direction, const std::string& wallType) {
    map[x][y].setWall(direction, wallType);
}

void GameMap::setFloorType(int x, int y, const std::string& floorType) {
    map[x][y].setFloorType(floorType);
}

void GameMap::setCeilingType(int x, int y, const std::string& ceilingType) {
    map[x][y].setCeilingType(ceilingType);
}

void GameMap::setTile(int x, int y, const Tile& targetTile) {
    map[x][y].setTile(targetTile);
}

void GameMap::setTileParameters(int x, int y, const std::string& northWall, const std::string& eastWall, const std::string& southWall, const std::string& westWall, const std::string& floor, const std::string& ceiling, const MapObject& mapObject, const MapTrigger& mapTrigger, bool explored) {
    map[x][y].setTile(northWall, eastWall, southWall, westWall, floor, ceiling, mapObject, mapTrigger, explored);
}

int GameMap::getHeight() const {
    return height;
}

int GameMap::getWidth() const {
    return width;
}

void GameMap::saveToVector() {
    savedTiles.clear();
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            savedTiles.push_back(map[i][j]);
        }
    }
}

void GameMap::loadFromVector() {
    for (int i = width - 1; i >= 0; i--) {
        for (int j = height - 1; j >= 0; j--) {
            map[i][j].setTile(savedTiles.back());
            savedTiles.pop_back();
        }
    }
    loadTextures();
    savedTiles.clear();
}

// get texture sets from all tiles and add them into the texture map if not already added
void GameMap::loadTextures() {
    SDL_Renderer* renderer = Graphics::getInstance().getRenderer();
    SDL_Surface* surface = nullptr;

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            std::unordered_set tileTextures = map[i][j].getTextures();
            for (std::string tileTexture : tileTextures) {
                auto it = textures.find(tileTexture);
                if (it == textures.end()) {
                    surface = IMG_Load((getEnvironmentTexturesDirectory() + tileTexture + ".png").c_str());
                    textures.insert({ tileTexture, SDL_CreateTextureFromSurface(renderer, surface) });
                    SDL_FreeSurface(surface);
                    surface = nullptr;
                }
            }
        }
    }
}

// calculates and draws textures, which are in players vision cone
void GameMap::makeScreenSnapshot(const Player& player) {
    generateScreenTexture(player, previousScreenTexture);
}

void GameMap::generateScreenTexture(const Player& player, SDL_Texture* targetTexture) {
    generateScreenTexture(player, targetTexture, 0.0f);
}

void GameMap::generateScreenTexture(const Player& player, SDL_Texture* targetTexture, float offset) { // offset determines the point of view (negative value means the player is moving left, positive right and 0.0 is centered)
    float screenWidth = Graphics::getInstance().getScreenWidth();
    float screenHeight = Graphics::getInstance().getScreenHeight();
    float fov = Graphics::getInstance().getFOV();
    float gameWidth = 0.75f * screenWidth;
    float tileWidth = gameWidth;
    float tileHeight = 1.5f * screenHeight;

    SDL_Renderer* renderer = Graphics::getInstance().getRenderer();
    SDL_SetRenderTarget(renderer, targetTexture);
    SDL_Rect targetArea = { 0, 0, 0, 0 };
    SDL_RenderClear(renderer);

    Direction leftHandSide = player.getDirection();
    leftHandSide--;
    Direction rightHandSide = player.getDirection();
    rightHandSide++;
    int viewDistance = 15;
    int distanceFromPlayer = 0;
    int x = 0;
    int y = 0;

    for (int j = viewDistance; j >= 0; j--) {

        // calculate maximum cone of view based on view distance

        for (int i = (-(int)(1 / pow(fov, viewDistance)) / 2) - 1; i <= (offset == 0.0f ? 0 : 1); i++) {

            // rotates coordinates to be relative to the direction the player is facing

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

            distanceFromPlayer = (int)sqrt(i * i + j * j);

            if (!map[x][y].isFullyWalled()) { // ignore tiles, which are part of the wall and therefore not accessible or observable

                if (map[x][y].hasFloor()) {
                    float x1 = (float)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fov, j + 1));
                    float x2 = (float)(x1 + tileWidth * pow(fov, j + 1));
                    float x3 = (float)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fov, j));
                    float x4 = (float)(x3 + tileWidth * pow(fov, j));

                    float y1 = (float)((screenHeight + tileHeight * pow(fov, j + 1)) * 0.5);
                    float y2 = (float)((screenHeight + tileHeight * pow(fov, j + 1)) * 0.5);
                    float y3 = (float)((screenHeight + tileHeight * pow(fov, j)) * 0.5);
                    float y4 = (float)((screenHeight + tileHeight * pow(fov, j)) * 0.5);

                    std::array<std::pair<float, float>, 4> vertexCollection = { { { x1, y1 }, { x2, y2 }, { x3, y3 }, { x4, y4 } } };

                    if (isTextureInView(vertexCollection)) {
                        Graphics::getInstance().renderTextureUsingVertices(textures.find(map[x][y].getFloorType())->second, vertexCollection, distanceFromPlayer);
                    }
                }

                if (map[x][y].hasCeiling()) {
                    float x1 = (float)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fov, j));
                    float x2 = (float)(x1 + tileWidth * pow(fov, j));
                    float x3 = (float)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fov, j + 1));
                    float x4 = (float)(x3 + tileWidth * pow(fov, j + 1));

                    float y1 = (float)((screenHeight - tileHeight * pow(fov, j)) * 0.5);
                    float y2 = (float)((screenHeight - tileHeight * pow(fov, j)) * 0.5);
                    float y3 = (float)((screenHeight - tileHeight * pow(fov, j + 1)) * 0.5);
                    float y4 = (float)((screenHeight - tileHeight * pow(fov, j + 1)) * 0.5);

                    std::array<std::pair<float, float>, 4> vertexCollection = { { { x1, y1 }, { x2, y2 }, { x3, y3 }, { x4, y4 } } };

                    if (isTextureInView(vertexCollection)) {
                        Graphics::getInstance().renderTextureUsingVertices(textures.find(map[x][y].getCeilingType())->second, vertexCollection, distanceFromPlayer);
                    }
                }

                if (map[x][y].isWalled(leftHandSide)) {
                    float x1 = (float)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fov, j));
                    float x2 = (float)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fov, j + 1));
                    float x3 = x1;
                    float x4 = x2;

                    float y1 = (float)((screenHeight - tileHeight * pow(fov, j)) * 0.5);
                    float y2 = (float)((screenHeight - tileHeight * pow(fov, j + 1)) * 0.5);
                    float y3 = (float)(y1 + tileHeight * pow(fov, j));
                    float y4 = (float)(y2 + tileHeight * pow(fov, j + 1));

                    std::array<std::pair<float, float>, 4> vertexCollection = { { { x1, y1 }, { x2, y2 }, { x3, y3 }, { x4, y4 } } };

                    if (isTextureInView(vertexCollection)) {
                        Graphics::getInstance().renderTextureUsingVertices(textures.find(map[x][y].getWallType(leftHandSide))->second, vertexCollection, distanceFromPlayer);
                    }
                }

                if (map[x][y].isWalled(player.getDirection())) {
                    float x1 = (float)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fov, j + 1));
                    float x2 = (float)(x1 + tileWidth * pow(fov, j + 1));
                    float x3 = (float)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fov, j + 1));
                    float x4 = (float)(x3 + tileWidth * pow(fov, j + 1));

                    float y1 = (float)((screenHeight - tileHeight * pow(fov, j + 1)) * 0.5);
                    float y2 = (float)((screenHeight - tileHeight * pow(fov, j + 1)) * 0.5);
                    float y3 = (float)(y1 + tileHeight * pow(fov, j + 1));
                    float y4 = (float)(y2 + tileHeight * pow(fov, j + 1));

                    std::array<std::pair<float, float>, 4> vertexCollection = { { { x1, y1 }, { x2, y2 }, { x3, y3 }, { x4, y4 } } };

                    if (isTextureInView(vertexCollection)) {
                        Graphics::getInstance().renderTextureUsingVertices(textures.find(map[x][y].getWallType(player.getDirection()))->second, vertexCollection, 100);
                    }
                }

                if (map[x][y].containsObject()) {
                    targetArea.x = (int)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fov, j + 1));
                    targetArea.y = (int)((screenHeight - tileHeight * pow(fov, j + 1)) * 0.5);
                    targetArea.w = (int)(tileWidth * pow(fov, j + 1));
                    targetArea.h = (int)(tileHeight * pow(fov, j + 1));
                    SDL_RenderCopyEx(renderer, textures.find(map[x][y].getObject().getObjectType())->second, nullptr, &targetArea, 0.0, nullptr, SDL_FLIP_NONE);
                }
            }
        }

        // calculate maximum cone of view based on view distance

        for (int i = ((int)(1 / pow(fov, viewDistance)) / 2) + 1; i >= (offset == 0.0f ? 0 : -1); i--) {

            // rotates coordinates to be relative to the direction the player is facing

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

            distanceFromPlayer = (int)sqrt(i * i + j * j);

            if (!map[x][y].isFullyWalled()) { // ignore tiles, which are part of the wall and therefore not accessible or observable

                if (map[x][y].hasFloor()) {
                    float x1 = (float)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fov, j + 1));
                    float x2 = (float)(x1 + tileWidth * pow(fov, j + 1));
                    float x3 = (float)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fov, j));
                    float x4 = (float)(x3 + tileWidth * pow(fov, j));

                    float y1 = (float)((screenHeight + tileHeight * pow(fov, j + 1)) * 0.5);
                    float y2 = (float)((screenHeight + tileHeight * pow(fov, j + 1)) * 0.5);
                    float y3 = (float)((screenHeight + tileHeight * pow(fov, j)) * 0.5);
                    float y4 = (float)((screenHeight + tileHeight * pow(fov, j)) * 0.5);

                    std::array<std::pair<float, float>, 4> vertexCollection = { { { x1, y1 }, { x2, y2 }, { x3, y3 }, { x4, y4 } } };

                    if (isTextureInView(vertexCollection)) {
                        Graphics::getInstance().renderTextureUsingVertices(textures.find(map[x][y].getFloorType())->second, vertexCollection, distanceFromPlayer);
                    }
                }

                if (map[x][y].hasCeiling()) {
                    float x1 = (float)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fov, j));
                    float x2 = (float)(x1 + tileWidth * pow(fov, j));
                    float x3 = (float)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fov, j + 1));
                    float x4 = (float)(x3 + tileWidth * pow(fov, j + 1));

                    float y1 = (float)((screenHeight - tileHeight * pow(fov, j)) * 0.5);
                    float y2 = (float)((screenHeight - tileHeight * pow(fov, j)) * 0.5);
                    float y3 = (float)((screenHeight - tileHeight * pow(fov, j + 1)) * 0.5);
                    float y4 = (float)((screenHeight - tileHeight * pow(fov, j + 1)) * 0.5);

                    std::array<std::pair<float, float>, 4> vertexCollection = { { { x1, y1 }, { x2, y2 }, { x3, y3 }, { x4, y4 } } };

                    if (isTextureInView(vertexCollection)) {
                        Graphics::getInstance().renderTextureUsingVertices(textures.find(map[x][y].getCeilingType())->second, vertexCollection, distanceFromPlayer);
                    }
                }

                if (map[x][y].isWalled(rightHandSide)) {
                    float x1 = (float)(gameWidth * 0.5 + (i + 0.5 + offset) * tileWidth * pow(fov, j + 1));
                    float x2 = (float)(gameWidth * 0.5 + (i + 0.5 + offset) * tileWidth * pow(fov, j));
                    float x3 = x1;
                    float x4 = x2;

                    float y1 = (float)((screenHeight - tileHeight * pow(fov, j + 1)) * 0.5);
                    float y2 = (float)((screenHeight - tileHeight * pow(fov, j)) * 0.5);
                    float y3 = (float)(y1 + tileHeight * pow(fov, j + 1));
                    float y4 = (float)(y2 + tileHeight * pow(fov, j));

                    std::array<std::pair<float, float>, 4> vertexCollection = { { { x2, y2 }, { x1, y1 }, { x4, y4 }, { x3, y3 } } };

                    if (isTextureInView(vertexCollection)) {
                        Graphics::getInstance().renderTextureUsingVertices(textures.find(map[x][y].getWallType(rightHandSide))->second, vertexCollection, distanceFromPlayer);
                    }
                }

                if (map[x][y].isWalled(player.getDirection())) {
                    float x1 = (float)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fov, j + 1));
                    float x2 = (float)(x1 + tileWidth * pow(fov, j + 1));
                    float x3 = (float)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fov, j + 1));
                    float x4 = (float)(x3 + tileWidth * pow(fov, j + 1));

                    float y1 = (float)((screenHeight - tileHeight * pow(fov, j + 1)) * 0.5);
                    float y2 = (float)((screenHeight - tileHeight * pow(fov, j + 1)) * 0.5);
                    float y3 = (float)(y1 + tileHeight * pow(fov, j + 1));
                    float y4 = (float)(y2 + tileHeight * pow(fov, j + 1));

                    std::array<std::pair<float, float>, 4> vertexCollection = { { { x1, y1 }, { x2, y2 }, { x3, y3 }, { x4, y4 } } };

                    if (isTextureInView(vertexCollection)) {
                        Graphics::getInstance().renderTextureUsingVertices(textures.find(map[x][y].getWallType(player.getDirection()))->second, vertexCollection, 100);
                    }
                }

                if (map[x][y].containsObject()) {
                    targetArea.x = (int)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fov, j + 1));
                    targetArea.y = (int)((screenHeight - tileHeight * pow(fov, j + 1)) * 0.5);
                    targetArea.w = (int)(tileWidth * pow(fov, j + 1));
                    targetArea.h = (int)(tileHeight * pow(fov, j + 1));
                    SDL_RenderCopyEx(renderer, textures.find(map[x][y].getObject().getObjectType())->second, nullptr, &targetArea, 0.0, nullptr, SDL_FLIP_NONE);
                }
            }
        }
    }

    SDL_SetRenderTarget(renderer, nullptr);
}

void GameMap::animateLeftRotation(const Player& player) {
    int screenWidth = (int)Graphics::getInstance().getScreenWidth();
    int screenHeight = (int)Graphics::getInstance().getScreenHeight();
    int gameWidth = (int)(3 * screenWidth / 4);
    int animationFrames = Graphics::getInstance().getAnimationFrames();
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

        SDL_Delay(Graphics::getInstance().getAnimationDuration() / animationFrames);
    }
}

void GameMap::animateRightRotation(const Player& player) {
    int screenWidth = (int)Graphics::getInstance().getScreenWidth();
    int screenHeight = (int)Graphics::getInstance().getScreenHeight();
    int gameWidth = (int)(3 * screenWidth / 4);
    int animationFrames = Graphics::getInstance().getAnimationFrames();
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

        SDL_Delay(Graphics::getInstance().getAnimationDuration() / animationFrames);
    }
}

void GameMap::animateForwardMovement(const Player& player) {
    int screenWidth = (int)Graphics::getInstance().getScreenWidth();
    int screenHeight = (int)Graphics::getInstance().getScreenHeight();
    int gameWidth = (int)(3 * screenWidth / 4);
    int animationFrames = Graphics::getInstance().getAnimationFrames();
    float fov = Graphics::getInstance().getFOV();
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

        SDL_Delay(Graphics::getInstance().getAnimationDuration() / animationFrames);
    }
}

void GameMap::animateBackwardMovement(const Player& player) {
    int screenWidth = (int)Graphics::getInstance().getScreenWidth();
    int screenHeight = (int)Graphics::getInstance().getScreenHeight();
    int gameWidth = (int)(3 * screenWidth / 4);
    int animationFrames = Graphics::getInstance().getAnimationFrames();
    float fov = Graphics::getInstance().getFOV();
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

        SDL_Delay(Graphics::getInstance().getAnimationDuration() / animationFrames);
    }
}

void GameMap::animateSidestepLeft(const Player& player) {
    int screenWidth = (int)Graphics::getInstance().getScreenWidth();
    int screenHeight = (int)Graphics::getInstance().getScreenHeight();
    int gameWidth = (int)(3 * screenWidth / 4);
    int animationFrames = Graphics::getInstance().getAnimationFrames();
    float fov = Graphics::getInstance().getFOV();
    SDL_Renderer* renderer = Graphics::getInstance().getRenderer();
    SDL_Rect targetArea = { 0, 0, 0, 0 };
    SDL_Rect sourceArea = { 0, 0, 0, 0 };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);

    generateScreenTexture(player, currentScreenTexture);

    for (int i = 1; i < animationFrames; i++) {
        targetArea = { 0, 0, screenWidth, screenHeight };
        SDL_RenderFillRect(renderer, &targetArea);

        generateScreenTexture(player, currentScreenTexture, ((float)+i / animationFrames) - 1);
        sourceArea = { 0, 0, gameWidth, screenHeight };
        targetArea = { 0, 0, gameWidth, screenHeight };
        SDL_RenderCopyEx(renderer, currentScreenTexture, &sourceArea, &targetArea, 0.0, nullptr, SDL_FLIP_NONE);

        targetArea = { gameWidth, 0, screenWidth - gameWidth, screenHeight };
        SDL_RenderFillRect(renderer, &targetArea);
        targetArea = { 0, 0, gameWidth, screenHeight / 7 };
        SDL_RenderFillRect(renderer, &targetArea);
        SDL_RenderPresent(renderer);

        SDL_Delay(Graphics::getInstance().getAnimationDuration() / animationFrames);
    }
}

void GameMap::animateSidestepRight(const Player& player) {
    int screenWidth = (int)Graphics::getInstance().getScreenWidth();
    int screenHeight = (int)Graphics::getInstance().getScreenHeight();
    int gameWidth = (int)(3 * screenWidth / 4);
    int animationFrames = Graphics::getInstance().getAnimationFrames();
    float fov = Graphics::getInstance().getFOV();
    SDL_Renderer* renderer = Graphics::getInstance().getRenderer();
    SDL_Rect targetArea = { 0, 0, 0, 0 };
    SDL_Rect sourceArea = { 0, 0, 0, 0 };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);

    for (int i = 1; i < animationFrames; i++) {
        targetArea = { 0, 0, screenWidth, screenHeight };
        SDL_RenderFillRect(renderer, &targetArea);

        generateScreenTexture(player, currentScreenTexture, ((float)-i / animationFrames) + 1);
        sourceArea = { 0, 0, gameWidth, screenHeight };
        targetArea = { 0, 0, gameWidth, screenHeight };
        SDL_RenderCopyEx(renderer, currentScreenTexture, &sourceArea, &targetArea, 0.0, nullptr, SDL_FLIP_NONE);

        targetArea = { gameWidth, 0, screenWidth - gameWidth, screenHeight };
        SDL_RenderFillRect(renderer, &targetArea);
        targetArea = { 0, 0, gameWidth, screenHeight / 7 };
        SDL_RenderFillRect(renderer, &targetArea);
        SDL_RenderPresent(renderer);

        SDL_Delay(Graphics::getInstance().getAnimationDuration() / animationFrames);
    }
}

void GameMap::renderVisibleArea(const Player& player) {
    int screenWidth = (int)Graphics::getInstance().getScreenWidth();
    int screenHeight = (int)Graphics::getInstance().getScreenHeight();
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

// returns false, if none of the X vertex coordinates are located inside the game view
bool GameMap::isTextureInView(const std::array<std::pair<float, float>, 4>& vertexCollection) {
    int verticesOnScreen = 0;
    for (int i = 0; i < 4; i++) {
        if (vertexCollection[i].first >= 0 && vertexCollection[i].first <= 0.75f * Graphics::getInstance().getScreenWidth()) {
            verticesOnScreen++;
        }
    }
    return verticesOnScreen == 0 ? false : true;
}

void GameMap::saveToFile(const std::string& outputFile) {
    saveToVector();
    std::ofstream ofs(outputFile);
    boost::archive::binary_oarchive boostArchive(ofs);
    boostArchive << *this;
}

void GameMap::loadFromFile(const std::string& inputFile) {
    std::ifstream ifs(getMapsDirectory() + inputFile);
    boost::archive::binary_iarchive boostArchive(ifs);
    boostArchive >> *this;
}

// constructs a map object based on size variables and populates it with data from savedTiles vector
void GameMap::init() {
    map = std::make_shared<std::shared_ptr<Tile[]>[]>(width);
    for (int i = 0; i < width; i++) {
        map[i] = std::make_shared<Tile[]>(height);
    }
    loadFromVector();
}

GameMap::GameMap() {
    previousScreenTexture = SDL_CreateTexture(Graphics::getInstance().getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)Graphics::getInstance().getScreenWidth(), (int)Graphics::getInstance().getScreenHeight());
    currentScreenTexture = SDL_CreateTexture(Graphics::getInstance().getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)Graphics::getInstance().getScreenWidth(), (int)Graphics::getInstance().getScreenHeight());
    height = 0;
    width = 0;
}

GameMap::GameMap(int width, int height) {
    previousScreenTexture = SDL_CreateTexture(Graphics::getInstance().getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)Graphics::getInstance().getScreenWidth(), (int)Graphics::getInstance().getScreenHeight());
    currentScreenTexture = SDL_CreateTexture(Graphics::getInstance().getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)Graphics::getInstance().getScreenWidth(), (int)Graphics::getInstance().getScreenHeight());
    this->width = width;
    this->height = height;
    savedTiles.clear();
    map = std::make_shared<std::shared_ptr<Tile[]>[]>(width);
    for (int i = 0; i < width; i++) {
        map[i] = std::make_shared<Tile[]>(height);
    }
}

GameMap::GameMap(int width, int height, const std::vector<Tile>& savedTiles) {
    previousScreenTexture = SDL_CreateTexture(Graphics::getInstance().getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)Graphics::getInstance().getScreenWidth(), (int)Graphics::getInstance().getScreenHeight());
    currentScreenTexture = SDL_CreateTexture(Graphics::getInstance().getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)Graphics::getInstance().getScreenWidth(), (int)Graphics::getInstance().getScreenHeight());
    this->width = width;
    this->height = height;
    this->savedTiles.clear();
    map = std::make_shared<std::shared_ptr<Tile[]>[]>(width);
    for (int i = 0; i < width; i++) {
        map[i] = std::make_shared<Tile[]>(height);
    }
    this->savedTiles = savedTiles;
    loadFromVector();
}

GameMap::~GameMap() {
    SDL_DestroyTexture(previousScreenTexture);
    previousScreenTexture = nullptr;

    SDL_DestroyTexture(currentScreenTexture);
    currentScreenTexture = nullptr;

    for (auto i = textures.begin(); i != textures.end(); i++) {
        SDL_DestroyTexture(i->second);
        i->second = nullptr;
    }
}
