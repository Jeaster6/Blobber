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
    generateScreenTexture(player, targetTexture, 0.0);
}

void GameMap::generateScreenTexture(Player player, SDL_Texture* targetTexture, float offset) { //offset determines the point of view (negative value means the player is moving left, positive right and 0.0 is centered)
    float screenWidth = Graphics::getInstance().getScreenWidth();
    float screenHeight = Graphics::getInstance().getScreenHeight();
    float fov = Graphics::getInstance().getFOV();
    float gameWidth = 3 * screenWidth / 4;
    float tileWidth = gameWidth;
    float tileHeight = 1.3f * screenHeight;

    SDL_Renderer* renderer = Graphics::getInstance().getRenderer();
    SDL_SetRenderTarget(renderer, targetTexture);
    SDL_Rect targetArea = { 0, 0, 0, 0 };
    SDL_RenderClear(renderer);

    Direction leftHandSide = player.getDirection();
    leftHandSide--;
    Direction rightHandSide = player.getDirection();
    rightHandSide++;
    int x = 0;
    int y = 0;

    for (int j = 10; j >= 0; j--) {

        for (int i = -j - 3; i <= 0; i++) {

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

            if (map[x][y].hasFloor()) {
                float x1 = (float)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fov, j + 1));
                float x2 = (float)(x1 + tileWidth * pow(fov, j + 1));
                float x3 = (float)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fov, j));
                float x4 = (float)(x3 + tileWidth * pow(fov, j));

                float y1 = (float)((screenHeight + tileHeight * pow(fov, j + 1)) * 0.5);
                float y2 = (float)((screenHeight + tileHeight * pow(fov, j + 1)) * 0.5);
                float y3 = (float)((screenHeight + tileHeight * pow(fov, j)) * 0.5);
                float y4 = (float)((screenHeight + tileHeight * pow(fov, j)) * 0.5);

                std::array<std::pair<float, float>, 4> vertextCollection = { { { x1, y1 }, { x2, y2 }, { x3, y3 }, { x4, y4 } } };

                Graphics::getInstance().renderTextureUsingVertices(textures.find(map[x][y].getFloorType())->second, vertextCollection);
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

                std::array<std::pair<float, float>, 4> vertextCollection = { { { x1, y1 }, { x2, y2 }, { x3, y3 }, { x4, y4 } } };

                Graphics::getInstance().renderTextureUsingVertices(textures.find(map[x][y].getCeilingType())->second, vertextCollection);
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

                std::array<std::pair<float, float>, 4> vertextCollection = { { { x1, y1 }, { x2, y2 }, { x3, y3 }, { x4, y4 } } };

                Graphics::getInstance().renderTextureUsingVertices(textures.find(map[x][y].getWallType(leftHandSide))->second, vertextCollection);
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

                std::array<std::pair<float, float>, 4> vertextCollection = { { { x1, y1 }, { x2, y2 }, { x3, y3 }, { x4, y4 } } };

                Graphics::getInstance().renderTextureUsingVertices(textures.find(map[x][y].getWallType(player.getDirection()))->second, vertextCollection);
            }

            if (map[x][y].containsObject()) {
                targetArea.x = (int)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fov, j + 1));
                targetArea.y = (int)((screenHeight - tileHeight * pow(fov, j + 1)) * 0.5);
                targetArea.w = (int)(tileWidth * pow(fov, j + 1));
                targetArea.h = (int)(tileHeight * pow(fov, j + 1));
                SDL_RenderCopyEx(renderer, textures.find(map[x][y].getObjectType())->second, nullptr, &targetArea, 0.0, nullptr, SDL_FLIP_NONE);
            }
        }

        for (int i = j + 3; i >= 0; i--) {

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
            
            if (map[x][y].hasFloor()) {
                float x1 = (float)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fov, j + 1));
                float x2 = (float)(x1 + tileWidth * pow(fov, j + 1));
                float x3 = (float)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fov, j));
                float x4 = (float)(x3 + tileWidth * pow(fov, j));

                float y1 = (float)((screenHeight + tileHeight * pow(fov, j + 1)) * 0.5);
                float y2 = (float)((screenHeight + tileHeight * pow(fov, j + 1)) * 0.5);
                float y3 = (float)((screenHeight + tileHeight * pow(fov, j)) * 0.5);
                float y4 = (float)((screenHeight + tileHeight * pow(fov, j)) * 0.5);

                std::array<std::pair<float, float>, 4> vertextCollection = { { { x1, y1 }, { x2, y2 }, { x3, y3 }, { x4, y4 } } };

                Graphics::getInstance().renderTextureUsingVertices(textures.find(map[x][y].getFloorType())->second, vertextCollection);
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

                std::array<std::pair<float, float>, 4> vertextCollection = { { { x1, y1 }, { x2, y2 }, { x3, y3 }, { x4, y4 } } };

                Graphics::getInstance().renderTextureUsingVertices(textures.find(map[x][y].getCeilingType())->second, vertextCollection);
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

                std::array<std::pair<float, float>, 4> vertextCollection = { { { x2, y2 }, { x1, y1 }, { x4, y4 }, { x3, y3 } } };

                Graphics::getInstance().renderTextureUsingVertices(textures.find(map[x][y].getWallType(rightHandSide))->second, vertextCollection);
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

                std::array<std::pair<float, float>, 4> vertextCollection = { { { x1, y1 }, { x2, y2 }, { x3, y3 }, { x4, y4 } } };

                Graphics::getInstance().renderTextureUsingVertices(textures.find(map[x][y].getWallType(player.getDirection()))->second, vertextCollection);
            }

            if (map[x][y].containsObject()) {
                targetArea.x = (int)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fov, j + 1));
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

void GameMap::animateRightRotation(Player player) {
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

void GameMap::animateForwardMovement(Player player) {
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

void GameMap::animateBackwardMovement(Player player) {
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

void GameMap::animateSidestepLeft(Player player) {
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

void GameMap::animateSidestepRight(Player player) {
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

void GameMap::renderVisibleArea(Player player) {
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

GameMap::GameMap(int width, int height) {
    previousScreenTexture = SDL_CreateTexture(Graphics::getInstance().getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)Graphics::getInstance().getScreenWidth(), (int)Graphics::getInstance().getScreenHeight());
    currentScreenTexture = SDL_CreateTexture(Graphics::getInstance().getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)Graphics::getInstance().getScreenWidth(), (int)Graphics::getInstance().getScreenHeight());
    this->width = width;
    this->height = height;
    savedTiles.clear();
    map = new Tile*[width];
    for (int i = 0; i < width; i++) {
        map[i] = new Tile[height];
    }
}

GameMap::GameMap(int width, int height, std::vector <Tile> savedTiles) {
    previousScreenTexture = SDL_CreateTexture(Graphics::getInstance().getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)Graphics::getInstance().getScreenWidth(), (int)Graphics::getInstance().getScreenHeight());
    currentScreenTexture = SDL_CreateTexture(Graphics::getInstance().getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)Graphics::getInstance().getScreenWidth(), (int)Graphics::getInstance().getScreenHeight());
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
