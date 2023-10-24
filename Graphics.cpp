#include "Graphics.hpp"

Graphics& Graphics::getInstance() {
	return graphics;
}

Graphics::Graphics() {
	surface = nullptr;
	texture = nullptr;

	SDL_SetMainReady();
	SDL_Init(SDL_INIT_VIDEO);
	refreshSettings();
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

void Graphics::renderFullscreenTexture(const std::string& textureFileName) {
	surface = IMG_Load((getTexturesDirectory() + textureFileName).c_str());
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

void Graphics::renderTextureUsingVertices(SDL_Texture* targetTexture, const std::array<std::pair<float, float>, 4>& vertextCollection) {
	int LOD = 8;
	std::vector<SDL_Vertex> vertices;
	std::vector<int> indexList;

	/*float ratio_X = (vertextCollection[0].first - vertextCollection[1].first) / (vertextCollection[2].first - vertextCollection[3].first);
	float ratio_Y = (vertextCollection[0].second - vertextCollection[2].second) / (vertextCollection[1].second - vertextCollection[3].second);

	float total_X = 0;
	float total_Y = 0;
	for (int i = 0; i < LOD; i++) {
		total_X += pow(ratio_Y, i);
		total_Y += pow(ratio_X, i);
	}*/

	for (int i = 0; i <= LOD; i++) {
		for (int j = 0; j <= LOD; j++) {

			float delta_X1 = (vertextCollection[0].first - (j * (vertextCollection[0].first - vertextCollection[2].first) / (float)(LOD)));
			float delta_X2 = (vertextCollection[1].first - (j * (vertextCollection[1].first - vertextCollection[3].first) / (float)(LOD)));
			float finalVertex_X = (delta_X1 - (i * (delta_X1 - delta_X2) / LOD));

			float delta_Y1 = (vertextCollection[0].second - (j * (vertextCollection[0].second - vertextCollection[2].second) / (float)(LOD)));
			float delta_Y2 = (vertextCollection[1].second - (j * (vertextCollection[1].second - vertextCollection[3].second) / (float)(LOD)));
			float finalVertex_Y = (delta_Y1 - (i * (delta_Y1 - delta_Y2) / LOD));

			/*float k = 0;
			float l = 0;

			for (int iter = 0; iter < i; iter++) {
				k += pow(ratio_Y, iter);
			}

			for (int iter = 0; iter < j; iter++) {
				l += pow(ratio_X, iter);
			}*/

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

void Graphics::refreshSettings() {
	SDL_DestroyRenderer(gameRenderer);
	gameRenderer = nullptr;
	SDL_DestroyWindow(gameWindow);
	gameWindow = nullptr;

	screenWidth = Configuration::getInstance().getScreenWidth();
	screenHeight = Configuration::getInstance().getScreenHeight();
	fieldOfView = Configuration::getInstance().getFieldOfView();

	gameWindow = SDL_CreateWindow("Blobber", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, (int)screenWidth, (int)screenHeight, SDL_WINDOW_SHOWN);
	gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

SDL_Renderer* Graphics::getRenderer() {
	return gameRenderer;
}

float Graphics::getScreenWidth() {
	return screenWidth;
}

float Graphics::getScreenHeight() {
	return screenHeight;
}

float Graphics::getFOV() {
	return fieldOfView;
}

Graphics Graphics::graphics;
