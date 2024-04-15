#include "Graphics.hpp"

Graphics& Graphics::getInstance() {
	return graphics;
}

Graphics::Graphics() {
    previousScreenTexture = nullptr;
    currentScreenTexture = nullptr;
    UIOverlayTexture = nullptr;
    renderer = nullptr;
    window = nullptr;
    screenWidth = 0;
    screenHeight = 0;
    gameWidth = 0;
    fieldOfView = 0.0f;
    animationFrames = 0;
    animationDuration = 0;
    levelOfDetail = 0;

	SDL_SetMainReady();
	SDL_Init(SDL_INIT_VIDEO);
}

Graphics::~Graphics() {
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;
	SDL_DestroyWindow(window);
	window = nullptr;
    SDL_DestroyTexture(previousScreenTexture);
    previousScreenTexture = nullptr;
    SDL_DestroyTexture(currentScreenTexture);
    currentScreenTexture = nullptr;
    SDL_DestroyTexture(UIOverlayTexture);
    UIOverlayTexture = nullptr;

    for (auto i = currentMapTextures.begin(); i != currentMapTextures.end(); i++) {
        SDL_DestroyTexture(i->second);
        i->second = nullptr;
    }

	IMG_Quit();
	SDL_Quit();
}

void Graphics::init() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(previousScreenTexture);
    SDL_DestroyTexture(currentScreenTexture);
    SDL_DestroyTexture(UIOverlayTexture);

    screenWidth = Configuration::getInstance().getScreenWidth();
    screenHeight = Configuration::getInstance().getScreenHeight();
    gameWidth = (int)(0.75f * screenWidth);
    fieldOfView = Configuration::getInstance().getFieldOfView();
    animationFrames = Configuration::getInstance().getAnimationFrames();
    animationDuration = Configuration::getInstance().getAnimationDuration();
    levelOfDetail = Configuration::getInstance().getLevelOfDetail();

    window = SDL_CreateWindow("Blobber", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
    previousScreenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, screenWidth, screenHeight);
    currentScreenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, screenWidth, screenHeight);
    UIOverlayTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, screenWidth, screenHeight);
    fontProvider.init(renderer);
}

void Graphics::loadMapTextures(const GameMap& map) {
    for (auto i = currentMapTextures.begin(); i != currentMapTextures.end(); i++) {
        SDL_DestroyTexture(i->second);
        i->second = nullptr;
    }
    currentMapTextures.clear();

    // get texture sets from all tiles and add them into the texture map if not already added
    for (int i = 0; i < map.getWidth(); i++) {
        for (int j = 0; j < map.getHeight(); j++) {
            std::unordered_set tileTextures = map.getTile(i, j).getTextures();
            for (std::string tileTexture : tileTextures) {
                auto it = currentMapTextures.find(tileTexture);
                if (it == currentMapTextures.end()) {
                    SDL_Surface* surface = IMG_Load((getEnvironmentTexturesDirectory() + tileTexture + ".png").c_str());
                    currentMapTextures.insert({ tileTexture, SDL_CreateTextureFromSurface(renderer, surface) });
                    SDL_FreeSurface(surface);
                    surface = nullptr;
                }
            }
        }
    }
}

void Graphics::renderMainMenu(const SDL_Rect* targetArea, const std::string& textureFileName) {
    SDL_RenderClear(renderer);
    clearUI();
    renderUIElement(targetArea, textureFileName);
    SDL_RenderCopy(renderer, UIOverlayTexture, nullptr, targetArea);
    SDL_RenderPresent(renderer);
}

void Graphics::renderUIElement(const SDL_Rect* targetArea, const std::string& textureFileName) {
    SDL_Surface* surface = IMG_Load((getUITexturesDirectory() + textureFileName).c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_SetRenderTarget(renderer, UIOverlayTexture);
    SDL_RenderCopy(renderer, texture, nullptr, targetArea);
    SDL_SetRenderTarget(renderer, nullptr);

    SDL_FreeSurface(surface);
    surface = nullptr;
    SDL_DestroyTexture(texture);
    texture = nullptr;
}

void Graphics::renderTextMessage(int x, int y, const std::string& text, int fontSize, int r, int g, int b) {
    SDL_Rect targetArea = { x, y, screenWidth, screenHeight };
    SDL_Texture* texture = fontProvider.generateTextTexture(renderer, text, fontSize, r, g, b);

    SDL_SetRenderTarget(renderer, UIOverlayTexture);
    SDL_RenderCopy(renderer, texture, nullptr, &targetArea);
    SDL_SetRenderTarget(renderer, nullptr);
}

void Graphics::displayMessage(const std::string& message) {
    SDL_SetRenderTarget(renderer, UIOverlayTexture);

    // display message frame
    SDL_Rect targetArea = { (gameWidth / 2) - 290, 990, 580, 220 };
    SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
    SDL_RenderFillRect(renderer, &targetArea);

    targetArea = { (gameWidth / 2) - 280, 1000, 560, 200 };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &targetArea);
    //display message
    renderTextMessage((gameWidth / 2) - 255, 1025, message, 30, 255, 255, 0);

    SDL_SetRenderTarget(renderer, nullptr);
}

void Graphics::renderTextureUsingVertices(SDL_Texture* sourceTexture, const std::array<std::pair<float, float>, 4>& vertexCollection, int distanceFromPlayer) {
    std::vector<SDL_Vertex> vertices;
    std::vector<int> indexList;

    // limit level of detail when texture is further away from the player
	int LOD = std::max(levelOfDetail - distanceFromPlayer, 1);

	for (int i = 0; i <= LOD; i++) {
		for (int j = 0; j <= LOD; j++) {

			float delta_X1 = (vertexCollection[0].first - (j * (vertexCollection[0].first - vertexCollection[2].first) / (float)(LOD)));
			float delta_X2 = (vertexCollection[1].first - (j * (vertexCollection[1].first - vertexCollection[3].first) / (float)(LOD)));
			float finalVertex_X = (delta_X1 - (i * (delta_X1 - delta_X2) / LOD));

			float delta_Y1 = (vertexCollection[0].second - (j * (vertexCollection[0].second - vertexCollection[2].second) / (float)(LOD)));
			float delta_Y2 = (vertexCollection[1].second - (j * (vertexCollection[1].second - vertexCollection[3].second) / (float)(LOD)));
			float finalVertex_Y = (delta_Y1 - (i * (delta_Y1 - delta_Y2) / LOD));

			vertices.push_back({ {finalVertex_X, finalVertex_Y}, { 0xff, 0xff, 0xff, 0xff }, { (float)(i / (float)(LOD)), (float)(j / (float)(LOD)) } });
		}
	}

	for (int i = 0; i < LOD; i++) {
		for (int j = 0; j < LOD; j++) {

			indexList.push_back(j * (LOD + 1) + i);
			indexList.push_back(j * (LOD + 1) + i + 1);
			indexList.push_back((j + 1) * (LOD + 1) + i);

			indexList.push_back(j * (LOD + 1) + i + 1);
			indexList.push_back((j + 1) * (LOD + 1) + i);
			indexList.push_back((j + 1) * (LOD + 1) + i + 1);
		}
	}

	SDL_RenderGeometry(renderer, sourceTexture, vertices.data(), (int)vertices.size(), indexList.data(), (int)indexList.size());
}

void Graphics::generateScreenTexture(const GameMap& map, const Player& player, SDL_Texture* targetTexture) {
    generateScreenTexture(map, player, targetTexture, 0.0f);
}

// offset determines the point of view (negative value means the player is moving left, positive right and 0.0 is centered)
void Graphics::generateScreenTexture(const GameMap& map, const Player& player, SDL_Texture* targetTexture, float offset) { 
    int tileWidth = gameWidth;
    int tileHeight = (int)1.5f * screenHeight;

    SDL_SetRenderTarget(renderer, targetTexture);
    SDL_Rect targetArea = { 0, 0, 0, 0 };
    SDL_RenderClear(renderer);

    Direction leftHandSide = player.getDirection();
    leftHandSide--;
    Direction rightHandSide = player.getDirection();
    rightHandSide++;
    int viewDistance = std::min(13, playerDistanceFromMapEdge(map, player, player.getDirection()));
    int distanceFromPlayer = 0;
    int x = 0;
    int y = 0;

    for (int j = viewDistance; j >= 0; j--) {

        // calculate maximum cone of view based on view distance

        for (int i = -playerDistanceFromMapEdge(map, player, leftHandSide); i <= (offset == 0.0f ? 0 : 1); i++) {

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

            if (x < 0 || y < 0 || x >= map.getWidth() || y >= map.getHeight()) {
                continue;
            }

            distanceFromPlayer = (int)sqrt(i * i + j * j);

            if (!map.getTile(x, y).isFullyWalled()) { // ignore tiles, which are part of the wall and therefore not accessible or observable

                if (map.getTile(x, y).hasFloor()) {
                    float x1 = (float)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fieldOfView, j + 1));
                    float x2 = (float)(x1 + tileWidth * pow(fieldOfView, j + 1));
                    float x3 = (float)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fieldOfView, j));
                    float x4 = (float)(x3 + tileWidth * pow(fieldOfView, j));

                    float y1 = (float)((screenHeight + tileHeight * pow(fieldOfView, j + 1)) * 0.5);
                    float y2 = (float)((screenHeight + tileHeight * pow(fieldOfView, j + 1)) * 0.5);
                    float y3 = (float)((screenHeight + tileHeight * pow(fieldOfView, j)) * 0.5);
                    float y4 = (float)((screenHeight + tileHeight * pow(fieldOfView, j)) * 0.5);

                    std::array<std::pair<float, float>, 4> vertexCollection = { { { x1, y1 }, { x2, y2 }, { x3, y3 }, { x4, y4 } } };

                    if (isTextureInView(vertexCollection)) {
                        renderTextureUsingVertices(currentMapTextures.find(map.getTile(x, y).getFloorType())->second, vertexCollection, distanceFromPlayer);
                    }
                }

                if (map.getTile(x, y).hasCeiling()) {
                    float x1 = (float)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fieldOfView, j));
                    float x2 = (float)(x1 + tileWidth * pow(fieldOfView, j));
                    float x3 = (float)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fieldOfView, j + 1));
                    float x4 = (float)(x3 + tileWidth * pow(fieldOfView, j + 1));

                    float y1 = (float)((screenHeight - tileHeight * pow(fieldOfView, j)) * 0.5);
                    float y2 = (float)((screenHeight - tileHeight * pow(fieldOfView, j)) * 0.5);
                    float y3 = (float)((screenHeight - tileHeight * pow(fieldOfView, j + 1)) * 0.5);
                    float y4 = (float)((screenHeight - tileHeight * pow(fieldOfView, j + 1)) * 0.5);

                    std::array<std::pair<float, float>, 4> vertexCollection = { { { x1, y1 }, { x2, y2 }, { x3, y3 }, { x4, y4 } } };

                    if (isTextureInView(vertexCollection)) {
                        renderTextureUsingVertices(currentMapTextures.find(map.getTile(x, y).getCeilingType())->second, vertexCollection, distanceFromPlayer);
                    }
                }

                if (map.getTile(x, y).isWalled(leftHandSide)) {
                    float x1 = (float)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fieldOfView, j));
                    float x2 = (float)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fieldOfView, j + 1));
                    float x3 = x1;
                    float x4 = x2;

                    float y1 = (float)((screenHeight - tileHeight * pow(fieldOfView, j)) * 0.5);
                    float y2 = (float)((screenHeight - tileHeight * pow(fieldOfView, j + 1)) * 0.5);
                    float y3 = (float)(y1 + tileHeight * pow(fieldOfView, j));
                    float y4 = (float)(y2 + tileHeight * pow(fieldOfView, j + 1));

                    std::array<std::pair<float, float>, 4> vertexCollection = { { { x1, y1 }, { x2, y2 }, { x3, y3 }, { x4, y4 } } };

                    if (isTextureInView(vertexCollection)) {
                        renderTextureUsingVertices(currentMapTextures.find(map.getTile(x, y).getWallType(leftHandSide))->second, vertexCollection, distanceFromPlayer);
                    }
                }

                if (map.getTile(x, y).isWalled(player.getDirection())) {
                    float x1 = (float)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fieldOfView, j + 1));
                    float x2 = (float)(x1 + tileWidth * pow(fieldOfView, j + 1));
                    float x3 = (float)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fieldOfView, j + 1));
                    float x4 = (float)(x3 + tileWidth * pow(fieldOfView, j + 1));

                    float y1 = (float)((screenHeight - tileHeight * pow(fieldOfView, j + 1)) * 0.5);
                    float y2 = (float)((screenHeight - tileHeight * pow(fieldOfView, j + 1)) * 0.5);
                    float y3 = (float)(y1 + tileHeight * pow(fieldOfView, j + 1));
                    float y4 = (float)(y2 + tileHeight * pow(fieldOfView, j + 1));

                    std::array<std::pair<float, float>, 4> vertexCollection = { { { x1, y1 }, { x2, y2 }, { x3, y3 }, { x4, y4 } } };

                    if (isTextureInView(vertexCollection)) {
                        renderTextureUsingVertices(currentMapTextures.find(map.getTile(x, y).getWallType(player.getDirection()))->second, vertexCollection, 100);
                    }
                }

                if (map.getTile(x, y).containsObject()) {
                    targetArea.x = (int)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fieldOfView, j + 1));
                    targetArea.y = (int)((screenHeight - tileHeight * pow(fieldOfView, j + 1)) * 0.5);
                    targetArea.w = (int)(tileWidth * pow(fieldOfView, j + 1));
                    targetArea.h = (int)(tileHeight * pow(fieldOfView, j + 1));
                    SDL_RenderCopyEx(renderer, currentMapTextures.find(map.getTile(x, y).getObject().getType())->second, nullptr, &targetArea, 0.0, nullptr, SDL_FLIP_NONE);
                }
            }
        }

        // calculate maximum cone of view based on view distance

        for (int i = playerDistanceFromMapEdge(map, player, rightHandSide); i >= (offset == 0.0f ? 0 : -1); i--) {

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

            if (x < 0 || y < 0 || x >= map.getWidth() || y >= map.getHeight()) {
                continue;
            }

            distanceFromPlayer = (int)sqrt(i * i + j * j);

            if (!map.getTile(x, y).isFullyWalled()) { // ignore tiles, which are part of the wall and therefore not accessible or observable

                if (map.getTile(x, y).hasFloor()) {
                    float x1 = (float)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fieldOfView, j + 1));
                    float x2 = (float)(x1 + tileWidth * pow(fieldOfView, j + 1));
                    float x3 = (float)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fieldOfView, j));
                    float x4 = (float)(x3 + tileWidth * pow(fieldOfView, j));

                    float y1 = (float)((screenHeight + tileHeight * pow(fieldOfView, j + 1)) * 0.5);
                    float y2 = (float)((screenHeight + tileHeight * pow(fieldOfView, j + 1)) * 0.5);
                    float y3 = (float)((screenHeight + tileHeight * pow(fieldOfView, j)) * 0.5);
                    float y4 = (float)((screenHeight + tileHeight * pow(fieldOfView, j)) * 0.5);

                    std::array<std::pair<float, float>, 4> vertexCollection = { { { x1, y1 }, { x2, y2 }, { x3, y3 }, { x4, y4 } } };

                    if (isTextureInView(vertexCollection)) {
                        renderTextureUsingVertices(currentMapTextures.find(map.getTile(x, y).getFloorType())->second, vertexCollection, distanceFromPlayer);
                    }
                }

                if (map.getTile(x, y).hasCeiling()) {
                    float x1 = (float)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fieldOfView, j));
                    float x2 = (float)(x1 + tileWidth * pow(fieldOfView, j));
                    float x3 = (float)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fieldOfView, j + 1));
                    float x4 = (float)(x3 + tileWidth * pow(fieldOfView, j + 1));

                    float y1 = (float)((screenHeight - tileHeight * pow(fieldOfView, j)) * 0.5);
                    float y2 = (float)((screenHeight - tileHeight * pow(fieldOfView, j)) * 0.5);
                    float y3 = (float)((screenHeight - tileHeight * pow(fieldOfView, j + 1)) * 0.5);
                    float y4 = (float)((screenHeight - tileHeight * pow(fieldOfView, j + 1)) * 0.5);

                    std::array<std::pair<float, float>, 4> vertexCollection = { { { x1, y1 }, { x2, y2 }, { x3, y3 }, { x4, y4 } } };

                    if (isTextureInView(vertexCollection)) {
                        renderTextureUsingVertices(currentMapTextures.find(map.getTile(x, y).getCeilingType())->second, vertexCollection, distanceFromPlayer);
                    }
                }

                if (map.getTile(x, y).isWalled(rightHandSide)) {
                    float x1 = (float)(gameWidth * 0.5 + (i + 0.5 + offset) * tileWidth * pow(fieldOfView, j + 1));
                    float x2 = (float)(gameWidth * 0.5 + (i + 0.5 + offset) * tileWidth * pow(fieldOfView, j));
                    float x3 = x1;
                    float x4 = x2;

                    float y1 = (float)((screenHeight - tileHeight * pow(fieldOfView, j + 1)) * 0.5);
                    float y2 = (float)((screenHeight - tileHeight * pow(fieldOfView, j)) * 0.5);
                    float y3 = (float)(y1 + tileHeight * pow(fieldOfView, j + 1));
                    float y4 = (float)(y2 + tileHeight * pow(fieldOfView, j));

                    std::array<std::pair<float, float>, 4> vertexCollection = { { { x2, y2 }, { x1, y1 }, { x4, y4 }, { x3, y3 } } };

                    if (isTextureInView(vertexCollection)) {
                        renderTextureUsingVertices(currentMapTextures.find(map.getTile(x, y).getWallType(rightHandSide))->second, vertexCollection, distanceFromPlayer);
                    }
                }

                if (map.getTile(x, y).isWalled(player.getDirection())) {
                    float x1 = (float)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fieldOfView, j + 1));
                    float x2 = (float)(x1 + tileWidth * pow(fieldOfView, j + 1));
                    float x3 = (float)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fieldOfView, j + 1));
                    float x4 = (float)(x3 + tileWidth * pow(fieldOfView, j + 1));

                    float y1 = (float)((screenHeight - tileHeight * pow(fieldOfView, j + 1)) * 0.5);
                    float y2 = (float)((screenHeight - tileHeight * pow(fieldOfView, j + 1)) * 0.5);
                    float y3 = (float)(y1 + tileHeight * pow(fieldOfView, j + 1));
                    float y4 = (float)(y2 + tileHeight * pow(fieldOfView, j + 1));

                    std::array<std::pair<float, float>, 4> vertexCollection = { { { x1, y1 }, { x2, y2 }, { x3, y3 }, { x4, y4 } } };

                    if (isTextureInView(vertexCollection)) {
                        renderTextureUsingVertices(currentMapTextures.find(map.getTile(x, y).getWallType(player.getDirection()))->second, vertexCollection, 100);
                    }
                }

                if (map.getTile(x, y).containsObject()) {
                    targetArea.x = (int)(gameWidth * 0.5 + (i - 0.5 + offset) * tileWidth * pow(fieldOfView, j + 1));
                    targetArea.y = (int)((screenHeight - tileHeight * pow(fieldOfView, j + 1)) * 0.5);
                    targetArea.w = (int)(tileWidth * pow(fieldOfView, j + 1));
                    targetArea.h = (int)(tileHeight * pow(fieldOfView, j + 1));
                    SDL_RenderCopyEx(renderer, currentMapTextures.find(map.getTile(x, y).getObject().getType())->second, nullptr, &targetArea, 0.0, nullptr, SDL_FLIP_NONE);
                }
            }
        }
    }

    SDL_SetRenderTarget(renderer, nullptr);
}

void Graphics::animateLeftRotation(const GameMap& map, const Player& player) {
    SDL_Rect targetArea = { 0, 0, 0, 0 };
    SDL_Rect sourceArea = { 0, 0, 0, 0 };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

    generateScreenTexture(map, player, currentScreenTexture);

    for (int i = 1; i < animationFrames; i++) {
        sourceArea = { 0, 0, (gameWidth * abs(i - animationFrames)) / animationFrames, screenHeight };
        targetArea = { (gameWidth * i) / animationFrames, 0, (gameWidth * abs(i - animationFrames)) / animationFrames, screenHeight };
        SDL_RenderCopyEx(renderer, previousScreenTexture, &sourceArea, &targetArea, 0.0, nullptr, SDL_FLIP_NONE);

        sourceArea = { (gameWidth * abs(i - animationFrames)) / animationFrames, 0, (gameWidth * i) / animationFrames, screenHeight };
        targetArea = { 0, 0, (gameWidth * i) / animationFrames, screenHeight };
        SDL_RenderCopyEx(renderer, currentScreenTexture, &sourceArea, &targetArea, 0.0, nullptr, SDL_FLIP_NONE);

        renderUIOverlay();
        
        SDL_RenderPresent(renderer);
        SDL_Delay(animationDuration / animationFrames);
    }
}

void Graphics::animateRightRotation(const GameMap& map, const Player& player) {
    SDL_Rect targetArea = { 0, 0, 0, 0 };
    SDL_Rect sourceArea = { 0, 0, 0, 0 };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

    generateScreenTexture(map, player, currentScreenTexture);

    for (int i = 1; i < animationFrames; i++) {
        sourceArea = { (gameWidth * i) / animationFrames, 0, (gameWidth * abs(i - animationFrames)) / animationFrames, screenHeight };
        targetArea = { 0, 0, (gameWidth * abs(i - animationFrames)) / animationFrames, screenHeight };
        SDL_RenderCopyEx(renderer, previousScreenTexture, &sourceArea, &targetArea, 0.0, nullptr, SDL_FLIP_NONE);

        sourceArea = { 0, 0, (gameWidth * i) / animationFrames, screenHeight };
        targetArea = { (gameWidth * abs(i - animationFrames)) / animationFrames, 0, (gameWidth * i) / animationFrames, screenHeight };
        SDL_RenderCopyEx(renderer, currentScreenTexture, &sourceArea, &targetArea, 0.0, nullptr, SDL_FLIP_NONE);

        renderUIOverlay();

        SDL_RenderPresent(renderer);
        SDL_Delay(animationDuration / animationFrames);
    }
}

void Graphics::animateForwardMovement(const GameMap& map, const Player& player) {
    SDL_Rect targetArea = { 0, 0, 0, 0 };
    SDL_Rect sourceArea = { 0, 0, 0, 0 };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    int velocity = 0;
    float distance = 0;

    for (int i = 1; i < animationFrames; i++) {
        if (i <= animationFrames / 2) {
            velocity = i;
        }
        else velocity = animationFrames - i;

        distance = distance + (float)velocity / ((float)animationFrames / 4);

        sourceArea = { (int)((gameWidth * (1 - fieldOfView) * distance) / (animationFrames * 2)), (int)((screenHeight * (1 - fieldOfView) * distance) / (animationFrames * 2)), (int)(gameWidth - ((gameWidth * distance * (1 - fieldOfView)) / animationFrames)), (int)(screenHeight - ((screenHeight * distance * (1 - fieldOfView)) / animationFrames)) };
        targetArea = { 0, 0, gameWidth, screenHeight };
        SDL_RenderCopyEx(renderer, previousScreenTexture, &sourceArea, &targetArea, 0.0, nullptr, SDL_FLIP_NONE);

        renderUIOverlay();

        SDL_RenderPresent(renderer);
        SDL_Delay(animationDuration / animationFrames);
    }
}

void Graphics::animateBackwardMovement(const GameMap& map, const Player& player) {
    SDL_Rect targetArea = { 0, 0, 0, 0 };
    SDL_Rect sourceArea = { 0, 0, 0, 0 };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    int velocity = 0;
    float distance = (float)animationFrames;

    generateScreenTexture(map, player, currentScreenTexture);

    for (int i = 1; i < animationFrames; i++) {
        if (i <= animationFrames / 2) {
            velocity = i;
        }
        else velocity = animationFrames - i;

        distance = distance - (float)velocity / ((float)animationFrames / 4);

        sourceArea = { (int)((gameWidth * (1 - fieldOfView) * distance) / (animationFrames * 2)), (int)((screenHeight * (1 - fieldOfView) * distance) / (animationFrames * 2)), (int)(gameWidth - ((gameWidth * distance * (1 - fieldOfView)) / animationFrames)), (int)(screenHeight - ((screenHeight * distance * (1 - fieldOfView)) / animationFrames)) };
        targetArea = { 0, 0, gameWidth, screenHeight };
        SDL_RenderCopyEx(renderer, currentScreenTexture, &sourceArea, &targetArea, 0.0, nullptr, SDL_FLIP_NONE);

        renderUIOverlay();

        SDL_RenderPresent(renderer);
        SDL_Delay(animationDuration / animationFrames);
    }
}

void Graphics::animateSidestepLeft(const GameMap& map, const Player& player) {
    SDL_Rect targetArea = { 0, 0, 0, 0 };
    SDL_Rect sourceArea = { 0, 0, 0, 0 };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    int velocity = 0;
    float distance = 0;

    for (int i = 1; i < animationFrames; i++) {
        if (i <= animationFrames / 2) {
            velocity = i;
        }
        else velocity = animationFrames - i;

        distance = distance + (float)velocity / ((float)animationFrames / 4);

        generateScreenTexture(map, player, currentScreenTexture, ((float) + distance / animationFrames) - 1);
        sourceArea = { 0, 0, gameWidth, screenHeight };
        targetArea = { 0, 0, gameWidth, screenHeight };
        SDL_RenderCopyEx(renderer, currentScreenTexture, &sourceArea, &targetArea, 0.0, nullptr, SDL_FLIP_NONE);

        renderUIOverlay();

        SDL_RenderPresent(renderer);
        SDL_Delay(animationDuration / animationFrames);
    }
}

void Graphics::animateSidestepRight(const GameMap& map, const Player& player) {
    SDL_Rect targetArea = { 0, 0, 0, 0 };
    SDL_Rect sourceArea = { 0, 0, 0, 0 };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    int velocity = 0;
    float distance = 0;

    for (int i = 1; i < animationFrames; i++) {
        if (i <= animationFrames / 2) {
            velocity = i;
        }
        else velocity = animationFrames - i;

        distance = distance + (float)velocity / ((float)animationFrames / 4);

        generateScreenTexture(map, player, currentScreenTexture, ((float) - distance / animationFrames) + 1);
        sourceArea = { 0, 0, gameWidth, screenHeight };
        targetArea = { 0, 0, gameWidth, screenHeight };
        SDL_RenderCopyEx(renderer, currentScreenTexture, &sourceArea, &targetArea, 0.0, nullptr, SDL_FLIP_NONE);

        renderUIOverlay();

        SDL_RenderPresent(renderer);
        SDL_Delay(animationDuration / animationFrames);
    }
}

// returns false, if none of the X vertex coordinates are located inside the game view
bool Graphics::isTextureInView(const std::array<std::pair<float, float>, 4>& vertexCollection) const {
    int verticesOnScreen = 0;
    for (int i = 0; i < 4; i++) {
        if (vertexCollection[i].first >= 0 && vertexCollection[i].first <= gameWidth) {
            verticesOnScreen++;
        }
    }
    return verticesOnScreen == 0 ? false : true;
}

void Graphics::renderPlayerView(const GameMap& map, const Player& player) {
    generateScreenTexture(map, player, previousScreenTexture);
    SDL_RenderCopy(renderer, previousScreenTexture, nullptr, nullptr);
    renderUIOverlay();
    SDL_RenderPresent(renderer); 
}

void Graphics::renderUIOverlay() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

    // render right sidebar with UI elements
    SDL_Rect targetArea = { gameWidth, 0, screenWidth - gameWidth, screenHeight };
    SDL_RenderFillRect(renderer, &targetArea);

    // render top bar with UI elements
    targetArea = { 0, 0, gameWidth, screenHeight / 7 };
    SDL_RenderFillRect(renderer, &targetArea);
    SDL_RenderCopy(renderer, UIOverlayTexture, nullptr, nullptr);
}

int Graphics::playerDistanceFromMapEdge(const GameMap& map, const Player& player, Direction direction) {
    switch (direction) {

        case Direction::N: 
            return player.getY();
        
        case Direction::E: 
            return map.getWidth() - player.getX();
        
        case Direction::S: 
            return map.getHeight() - player.getY();
        
        case Direction::W: 
            return player.getX();
        
    }
    return std::max(map.getWidth(), map.getHeight());
}

// resets the UIOverlayTexture
void Graphics::clearUI() {
    SDL_SetRenderTarget(renderer, UIOverlayTexture);
    SDL_SetTextureBlendMode(UIOverlayTexture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, nullptr);
}

Graphics Graphics::graphics;
