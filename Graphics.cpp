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

void Graphics::refreshSettings() {
	SDL_DestroyRenderer(gameRenderer);
	gameRenderer = nullptr;
	SDL_DestroyWindow(gameWindow);
	gameWindow = nullptr;

	screenWidth = Configuration::getInstance().getScreenWidth();
	screenHeight = Configuration::getInstance().getScreenHeight();
	fieldOfView = Configuration::getInstance().getFieldOfView();

	gameWindow = SDL_CreateWindow("Blobber", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
	gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

SDL_Renderer* Graphics::getRenderer() {
	return gameRenderer;
}

int Graphics::getScreenWidth() {
	return screenWidth;
}

int Graphics::getScreenHeight() {
	return screenHeight;
}

double Graphics::getFOV() {
	return fieldOfView;
}

Graphics Graphics::graphics;