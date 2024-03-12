#include "FontProvider.hpp"

FontProvider::FontProvider() {
    fontTexture = nullptr;
    textTexture = nullptr;
}

FontProvider::~FontProvider() {
    SDL_DestroyTexture(fontTexture);
    fontTexture = nullptr;
    SDL_DestroyTexture(textTexture);
    textTexture = nullptr;
}

void FontProvider::init(SDL_Renderer* renderer) {
    textTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 2560, 1440);
    SDL_Surface* surface = IMG_Load((getUITexturesDirectory() + "Font.png").c_str());
    fontTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = nullptr;
}

SDL_Texture* FontProvider::generateTextTexture(SDL_Renderer* renderer, const std::string& text, int fontSize, int r, int g, int b) {
    SDL_Rect sourceArea = { 0, 0, 0, 0 };
    SDL_Rect targetArea = { 0, 0, 0, 0 };

    SDL_SetTextureBlendMode(textTexture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, textTexture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    for (int i = 0; i < text.size(); i++) {
        switch (text[i]) {
            case 'A':
                sourceArea = { 0, 0, 100, 100 };
                break;

            case 'B':
                sourceArea = { 100, 0, 100, 100 };
                break;

            case 'C':
                sourceArea = { 200, 0, 100, 100 };
                break;

            case 'D':
                sourceArea = { 300, 0, 100, 100 };
                break;

            case 'E':
                sourceArea = { 400, 0, 100, 100 };
                break;

            case 'F':
                sourceArea = { 500, 0, 100, 100 };
                break;

            case 'G':
                sourceArea = { 600, 0, 100, 100 };
                break;

            case 'H':
                sourceArea = { 700, 0, 100, 100 };
                break;

            case 'I':
                sourceArea = { 800, 0, 100, 100 };
                break;

            case 'J':
                sourceArea = { 900, 0, 100, 100 };
                break;

            case 'K':
                sourceArea = { 1000, 0, 100, 100 };
                break;

            case 'L':
                sourceArea = { 1100, 0, 100, 100 };
                break;

            case 'M':
                sourceArea = { 1200, 0, 100, 100 };
                break;

            case 'N':
                sourceArea = { 1300, 0, 100, 100 };
                break;

            case 'O':
                sourceArea = { 1400, 0, 100, 100 };
                break;

            case 'P':
                sourceArea = { 1500, 0, 100, 100 };
                break;

            case 'Q':
                sourceArea = { 1600, 0, 100, 100 };
                break;

            case 'R':
                sourceArea = { 1700, 0, 100, 100 };
                break;

            case 'S':
                sourceArea = { 1800, 0, 100, 100 };
                break;

            case 'T':
                sourceArea = { 1900, 0, 100, 100 };
                break;

            case 'U':
                sourceArea = { 0, 100, 100, 100 };
                break;

            case 'V':
                sourceArea = { 100, 100, 100, 100 };
                break;

            case 'W':
                sourceArea = { 200, 100, 100, 100 };
                break;

            case 'X':
                sourceArea = { 300, 100, 100, 100 };
                break;

            case 'Y':
                sourceArea = { 400, 100, 100, 100 };
                break;

            case 'Z':
                sourceArea = { 500, 100, 100, 100 };
                break;

            case 'a':
                sourceArea = { 600, 100, 100, 100 };
                break;

            case 'b':
                sourceArea = { 700, 100, 100, 100 };
                break;

            case 'c':
                sourceArea = { 800, 100, 100, 100 };
                break;

            case 'd':
                sourceArea = { 900, 100, 100, 100 };
                break;

            case 'e':
                sourceArea = { 1000, 100, 100, 100 };
                break;

            case 'f':
                sourceArea = { 1100, 100, 100, 100 };
                break;

            case 'g':
                sourceArea = { 1200, 100, 100, 100 };
                break;

            case 'h':
                sourceArea = { 1300, 100, 100, 100 };
                break;

            case 'i':
                sourceArea = { 1400, 100, 100, 100 };
                break;

            case 'j':
                sourceArea = { 1500, 100, 100, 100 };
                break;

            case 'k':
                sourceArea = { 1600, 100, 100, 100 };
                break;

            case 'l':
                sourceArea = { 1700, 100, 100, 100 };
                break;

            case 'm':
                sourceArea = { 1800, 100, 100, 100 };
                break;

            case 'n':
                sourceArea = { 1900, 100, 100, 100 };
                break;

            case 'o':
                sourceArea = { 0, 200, 100, 100 };
                break;

            case 'p':
                sourceArea = { 100, 200, 100, 100 };
                break;

            case 'q':
                sourceArea = { 200, 200, 100, 100 };
                break;

            case 'r':
                sourceArea = { 300, 200, 100, 100 };
                break;

            case 's':
                sourceArea = { 400, 200, 100, 100 };
                break;

            case 't':
                sourceArea = { 500, 200, 100, 100 };
                break;

            case 'u':
                sourceArea = { 600, 200, 100, 100 };
                break;

            case 'v':
                sourceArea = { 700, 200, 100, 100 };
                break;

            case 'w':
                sourceArea = { 800, 200, 100, 100 };
                break;

            case 'x':
                sourceArea = { 900, 200, 100, 100 };
                break;

            case 'y':
                sourceArea = { 1000, 200, 100, 100 };
                break;

            case 'z':
                sourceArea = { 1100, 200, 100, 100 };
                break;

            case '1':
                sourceArea = { 1200, 200, 100, 100 };
                break;

            case '2':
                sourceArea = { 1300, 200, 100, 100 };
                break;

            case '3':
                sourceArea = { 1400, 200, 100, 100 };
                break;

            case '4':
                sourceArea = { 1500, 200, 100, 100 };
                break;

            case '5':
                sourceArea = { 1600, 200, 100, 100 };
                break;

            case '6':
                sourceArea = { 1700, 200, 100, 100 };
                break;

            case '7':
                sourceArea = { 1800, 200, 100, 100 };
                break;

            case '8':
                sourceArea = { 1900, 200, 100, 100 };
                break;

            case '9':
                sourceArea = { 0, 300, 100, 100 };
                break;

            case '0':
                sourceArea = { 100, 300, 100, 100 };
                break;

            case '!':
                sourceArea = { 200, 300, 100, 100 };
                break;

            case '?':
                sourceArea = { 300, 300, 100, 100 };
                break;

            case '.':
                sourceArea = { 400, 300, 100, 100 };
                break;

            case ':':
                sourceArea = { 500, 300, 100, 100 };
                break;

            case '-':
                sourceArea = { 600, 300, 100, 100 };
                break;

            case ',':
                sourceArea = { 700, 300, 100, 100 };
                break;

            case '\'':
                sourceArea = { 800, 300, 100, 100 };
                break;

            case '\"':
                sourceArea = { 900, 300, 100, 100 };
                break;

            case '+':
                sourceArea = { 1000, 300, 100, 100 };
                break;

            case '_':
                sourceArea = { 1100, 300, 100, 100 };
                break;

            case '/':
                sourceArea = { 1200, 300, 100, 100 };
                break;

            case '(':
                sourceArea = { 1300, 300, 100, 100 };
                break;

            case ')':
                sourceArea = { 1400, 300, 100, 100 };
                break;

            case '#':
                sourceArea = { 1500, 300, 100, 100 };
                break;

            case '%':
                sourceArea = { 1600, 300, 100, 100 };
                break;

            case '&':
                sourceArea = { 1700, 300, 100, 100 };
                break;

            case '=':
                sourceArea = { 1800, 300, 100, 100 };
                break;

            case '|':
                sourceArea = { 1900, 300, 100, 100 };
                break;

            case '<':
                sourceArea = { 0, 400, 100, 100 };
                break;

            case '>':
                sourceArea = { 100, 400, 100, 100 };
                break;

            case '\\':
                sourceArea = { 200, 400, 100, 100 };
                break;

            case ';':
                sourceArea = { 300, 400, 100, 100 };
                break;

            case '[':
                sourceArea = { 400, 400, 100, 100 };
                break;

            case ']':
                sourceArea = { 500, 400, 100, 100 };
                break;

            case '{':
                sourceArea = { 600, 400, 100, 100 };
                break;

            case '}':
                sourceArea = { 700, 400, 100, 100 };
                break;

            case '~':
                sourceArea = { 800, 400, 100, 100 };
                break;

            case '@':
                sourceArea = { 900, 400, 100, 100 };
                break;

            case '$':
                sourceArea = { 1000, 400, 100, 100 };
                break;

            case '^':
                sourceArea = { 1100, 400, 100, 100 };
                break;

            case '*':
                sourceArea = { 1200, 400, 100, 100 };
                break;

            default:
                sourceArea = { 1900, 500, 100, 100 };
                break;
        }

        targetArea = { fontSize * i, 0, fontSize, fontSize };

        SDL_RenderCopy(renderer, fontTexture, &sourceArea, &targetArea);
    }

    SDL_SetTextureColorMod(textTexture, r, g, b);
    SDL_SetRenderTarget(renderer, nullptr);
    return textTexture;
}
