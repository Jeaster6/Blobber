#include "Graphics.hpp"

Graphics& Graphics::getInstance() {
	return graphics;
}

Graphics::Graphics() {
	SDL_SetMainReady();
	surface = NULL;
	texture = NULL;
	SDL_Init(SDL_INIT_VIDEO);
	refreshSettings();
}

Graphics::~Graphics() {
	SDL_DestroyRenderer(gameRenderer);
	gameRenderer = NULL;
	SDL_DestroyWindow(gameWindow);
	gameWindow = NULL;

	IMG_Quit();
	SDL_Quit();
}

void Graphics::renderMainMenu() {
	renderMenuScreen("Menu.png");
}

void Graphics::renderSettings() {
	renderMenuScreen("Settings.png");
}

void Graphics::renderMenuScreen(const std::string& textureFileName) {
	surface = IMG_Load((getTexturesDirectory() + textureFileName).c_str());
	texture = SDL_CreateTextureFromSurface(gameRenderer, surface);

	SDL_RenderCopy(gameRenderer, texture, NULL, NULL);
	SDL_RenderPresent(gameRenderer);

	SDL_FreeSurface(surface);
	surface = NULL;
	SDL_DestroyTexture(texture);
	texture = NULL;
}

void Graphics::refreshSettings() {
	loadConfigFileData(screenWidth, screenHeight, fieldOfView);
	gameWindow = SDL_CreateWindow("Blobber", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
	gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

SDL_Renderer* Graphics::getRenderer() {
	return this->gameRenderer;
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