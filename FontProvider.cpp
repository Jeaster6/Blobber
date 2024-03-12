#include "FontProvider.hpp"

FontProvider::FontProvider() {
    font = nullptr;
    text = nullptr;
}

FontProvider::FontProvider(SDL_Renderer* renderer) {
    text = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 256, 144);
    SDL_Surface* surface = IMG_Load((getUITexturesDirectory() + "Font.png").c_str());
    font = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = nullptr;
}

FontProvider::~FontProvider() {
    SDL_DestroyTexture(font);
    font = nullptr;
    SDL_DestroyTexture(text);
    text = nullptr;
}

void FontProvider::init(SDL_Renderer* renderer) {
    text = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 2560, 1440);
    SDL_Surface* surface = IMG_Load((getUITexturesDirectory() + "Font.png").c_str());
    font = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = nullptr;
}

SDL_Texture* FontProvider::generateTextTexture(SDL_Renderer* renderer, std::string& const message, int r, int g, int b) {
    SDL_SetTextureBlendMode(text, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, text);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_Rect sourceArea = { 0, 0, 50, 50 };
    SDL_Rect targetArea = { 50, 0, 50, 50 };
    SDL_RenderCopy(renderer, font, &sourceArea, &targetArea);
    sourceArea = { 50, 0, 50, 50 };
    targetArea = { 150, 0, 50, 50 };
    SDL_RenderCopy(renderer, font, &sourceArea, &targetArea);
    SDL_SetTextureColorMod(text, r, g, b);
    SDL_SetRenderTarget(renderer, nullptr);
    return text;
}
