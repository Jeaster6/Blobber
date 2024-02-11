#include "Graphics.hpp"

Graphics& Graphics::getInstance() {
	return graphics;
}

Graphics::Graphics() {
	surface = nullptr;
	texture = nullptr;
    gameRenderer = nullptr;
    gameWindow = nullptr;
    screenWidth = 0.0f;
    screenHeight = 0.0f;
    fieldOfView = 0.0f;
    animationFrames = 0;
    animationDuration = 0;
    levelOfDetail = 0;

	SDL_SetMainReady();
	SDL_Init(SDL_INIT_VIDEO);
}

Graphics::~Graphics() {
	SDL_FreeSurface(surface);
	surface = nullptr;
	SDL_DestroyTexture(texture);
	texture = nullptr;
	SDL_DestroyRenderer(gameRenderer);
	gameRenderer = nullptr;
	SDL_DestroyWindow(gameWindow);
	gameWindow = nullptr;

	IMG_Quit();
	SDL_Quit();
}

void Graphics::renderMenuTexture(const std::string& textureFileName) {
	surface = IMG_Load((getMenuTexturesDirectory() + textureFileName).c_str());
	texture = SDL_CreateTextureFromSurface(gameRenderer, surface);

	SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, 0xFF);
	SDL_RenderClear(gameRenderer);
	SDL_RenderCopy(gameRenderer, texture, nullptr, nullptr); 
	SDL_RenderPresent(gameRenderer);

	SDL_FreeSurface(surface);
	surface = nullptr;
	SDL_DestroyTexture(texture);
	texture = nullptr;
}

void Graphics::renderTextureUsingVertices(SDL_Texture* targetTexture, const std::array<std::pair<float, float>, 4>& vertexCollection, int distanceFromPlayer) {

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

	SDL_RenderGeometry(gameRenderer, targetTexture, vertices.data(), (int)vertices.size(), indexList.data(), (int)indexList.size());
}

void Graphics::init() {
	SDL_DestroyRenderer(gameRenderer);
	gameRenderer = nullptr;
	SDL_DestroyWindow(gameWindow);
	gameWindow = nullptr;

	screenWidth = Configuration::getInstance().getScreenWidth();
	screenHeight = Configuration::getInstance().getScreenHeight();
	fieldOfView = Configuration::getInstance().getFieldOfView();
    animationFrames = Configuration::getInstance().getAnimationFrames();
    animationDuration = Configuration::getInstance().getAnimationDuration();
    levelOfDetail = Configuration::getInstance().getLevelOfDetail();

	gameWindow = SDL_CreateWindow("Blobber", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, (int)screenWidth, (int)screenHeight, SDL_WINDOW_SHOWN);
	gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

SDL_Renderer* Graphics::getRenderer() {
	return gameRenderer;
}

float Graphics::getScreenWidth() const {
	return screenWidth;
}

float Graphics::getScreenHeight() const {
	return screenHeight;
}

float Graphics::getFOV() const {
	return fieldOfView;
}

int Graphics::getAnimationFrames() const {
    return animationFrames;
}
   
int Graphics::getAnimationDuration() const {
    return animationDuration;
}

Graphics Graphics::graphics;
