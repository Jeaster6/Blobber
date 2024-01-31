#include "Editor.hpp"

void runMapEditor(GameMap* gameMap) {

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("Map Editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, window_flags);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Surface* surface = nullptr;
    SDL_Texture* targetTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_Rect targetArea = { 0, 0, 0, 0 };
    SDL_Event event;

    bool quit = false;
    int currentMode = 0;
    std::vector<std::pair<std::string, SDL_Texture*>> textures = loadTexturesFromDirectory(renderer);
    std::pair<std::string, SDL_Texture*> selectedTile;

    if (!textures.empty()) {
        selectedTile = textures[0];
    }

    SDL_Init(SDL_INIT_VIDEO);

    // ImGui initialization
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);
    ImVec4 backgroundColour = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    bool show_demo_window = true;

    while (!quit) {

        // read user input
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                quit = true;
            }

            else if (ImGui::GetIO().WantCaptureMouse) {
                ImGui_ImplSDL2_ProcessEvent(&event);
            }

            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                processMouseAction(gameMap, event, currentMode, renderer, targetTexture, selectedTile.first);
            }
        }

        // rendering map tiles
        SDL_SetRenderDrawColor(renderer, (Uint8)(backgroundColour.x * 255), (Uint8)(backgroundColour.y * 255), (Uint8)(backgroundColour.z * 255), (Uint8)(backgroundColour.w * 255));
        targetArea = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_SetRenderTarget(renderer, targetTexture);
        SDL_RenderFillRect(renderer, &targetArea);

        for (int i = 0; i < gameMap->getWidth(); i++) {
            for (int j = 0; j < gameMap->getHeight(); j++) {
                int numberOfWalls = 0;
                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
                targetArea = { (i + 1) * GRID_TILE_SIZE, (j + 1) * GRID_TILE_SIZE, TILE_SIZE, TILE_SIZE };
                SDL_RenderFillRect(renderer, &targetArea);

                if (currentMode == 2 && gameMap->getTile(i, j)->hasFloor()) {
                    auto index = std::distance(textures.begin(), std::find_if(textures.begin(), textures.end(), [&](const auto& pair) { return pair.first == gameMap->getTile(i, j)->getFloorType(); }));
                    SDL_RenderCopy(renderer, textures[index].second, nullptr, &targetArea);
                }

                if (currentMode == 3 && gameMap->getTile(i, j)->hasCeiling()) {
                    auto index = std::distance(textures.begin(), std::find_if(textures.begin(), textures.end(), [&](const auto& pair) { return pair.first == gameMap->getTile(i, j)->getCeilingType(); }));
                    SDL_RenderCopy(renderer, textures[index].second, nullptr, &targetArea);
                }

                if (gameMap->getTile(i, j)->isWalled(Direction::N)) {
                    numberOfWalls++;
                    targetArea = { (i + 1) * GRID_TILE_SIZE, (j + 1) * GRID_TILE_SIZE, TILE_SIZE, WALL_THICKNESS };
                    auto index = std::distance(textures.begin(), std::find_if(textures.begin(), textures.end(), [&](const auto& pair) { return pair.first == gameMap->getTile(i, j)->getWallType(Direction::N); }));
                    SDL_RenderCopy(renderer, textures[index].second, nullptr, &targetArea);
                }

                if (gameMap->getTile(i, j)->isWalled(Direction::E)) {
                    numberOfWalls++;
                    targetArea = { TILE_SIZE - WALL_THICKNESS + (i + 1) * GRID_TILE_SIZE, (j + 1) * GRID_TILE_SIZE, WALL_THICKNESS, TILE_SIZE };
                    auto index = std::distance(textures.begin(), std::find_if(textures.begin(), textures.end(), [&](const auto& pair) { return pair.first == gameMap->getTile(i, j)->getWallType(Direction::E); }));
                    SDL_RenderCopy(renderer, textures[index].second, nullptr, &targetArea);
                }

                if (gameMap->getTile(i, j)->isWalled(Direction::S)) {
                    numberOfWalls++;
                    targetArea = { (i + 1) * GRID_TILE_SIZE, TILE_SIZE-WALL_THICKNESS + (j + 1) * GRID_TILE_SIZE, TILE_SIZE, WALL_THICKNESS };
                    auto index = std::distance(textures.begin(), std::find_if(textures.begin(), textures.end(), [&](const auto& pair) { return pair.first == gameMap->getTile(i, j)->getWallType(Direction::S); }));
                    SDL_RenderCopy(renderer, textures[index].second, nullptr, &targetArea);
                }

                if (gameMap->getTile(i, j)->isWalled(Direction::W)) {
                    numberOfWalls++;
                    targetArea = { (i + 1) * GRID_TILE_SIZE, (j + 1) * GRID_TILE_SIZE, WALL_THICKNESS, TILE_SIZE };
                    auto index = std::distance(textures.begin(), std::find_if(textures.begin(), textures.end(), [&](const auto& pair) { return pair.first == gameMap->getTile(i, j)->getWallType(Direction::W); }));
                    SDL_RenderCopy(renderer, textures[index].second, nullptr, &targetArea);
                }

                if (numberOfWalls == 4) {
                    targetArea = { (i + 1) * GRID_TILE_SIZE, (j + 1) * GRID_TILE_SIZE, TILE_SIZE, TILE_SIZE };
                    SDL_SetRenderDrawColor(renderer, 0x66, 0x66, 0x66, 0xFF);
                    SDL_RenderFillRect(renderer, &targetArea);
                }

                SDL_SetRenderDrawColor(renderer, 0x00, 0x66, 0xAA, 0xFF);
                if (gameMap->getTile(i, j)->containsObject()) {
                    targetArea = { (i + 1) * GRID_TILE_SIZE + TILE_SIZE / 4, (j + 1) * GRID_TILE_SIZE + TILE_SIZE / 4, TILE_SIZE / 2, TILE_SIZE / 2 };
                    SDL_RenderFillRect(renderer, &targetArea);
                }
            }
        }

        // rendering ImGui windows
        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // show main window
        {
            ImGui::Begin("Main menu");

            ImGui::RadioButton("Basic Mode", &currentMode, 0); ImGui::SameLine();
            ImGui::RadioButton("Wall Mode", &currentMode, 1); ImGui::SameLine();
            ImGui::RadioButton("Floor Mode", &currentMode, 2); ImGui::SameLine();
            ImGui::RadioButton("Ceiling Mode", &currentMode, 3);

            ImGui::ColorEdit3("Background colour", (float*)&backgroundColour);
            ImGui::Checkbox("Show Second Window", &show_demo_window);

            ImGui::TextWrapped("Selected Texture");
            ImVec2 size = ImVec2(32.0f, 32.0f);                         // Size of the image we want to make visible
            ImVec2 uv0 = ImVec2(0.0f, 0.0f);                            // UV coordinates for lower-left
            ImVec2 uv1 = ImVec2(1.0f, 1.0f);                            // UV coordinates for (32,32) in our texture
            ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);             // Black background
            ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);           // No tint

            int ID = 0;
            for (std::pair<std::string, SDL_Texture*> texture : textures) {
                ImGui::PushID(ID);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                if (selectedTile.first == texture.first) {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
                }

                else ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
                if (ImGui::ImageButton("", ((ImTextureID)(intptr_t)texture.second), size, uv0, uv1, bg_col, tint_col)) {
                    selectedTile = texture;
                }

                if (ID % 10 == 9) {
                    ImGui::NewLine();
                }
                else ImGui::SameLine();

                ImGui::PopStyleColor(2);
                ImGui::PopID();
                ID++;
            }

            ImGui::End();
        }

        // show demo window
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // Rendering
        ImGui::Render();
        SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
        SDL_SetRenderTarget(renderer, nullptr);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, targetTexture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    // ImGui cleanup
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    // Window and renderer cleanup
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
	SDL_DestroyWindow(window);
    window = nullptr;

    for (std::pair<std::string, SDL_Texture*> texture : textures) {
        SDL_DestroyTexture(texture.second);
        texture.second = nullptr;
    }

    SDL_DestroyTexture(targetTexture);
    targetTexture = nullptr;
    SDL_DestroyTexture(selectedTile.second);
    selectedTile.second = nullptr;
    surface = nullptr;
    SDL_FreeSurface(surface);
	IMG_Quit();
	SDL_Quit();
}

void processMouseAction(GameMap* gameMap, SDL_Event mouseEvent, int currentMode, SDL_Renderer* renderer, SDL_Texture* targetTexture, const std::string& selectedTile) {

    int mouseCursorX = 0, mouseCursorY = 0;
    int mouseCursorOnButtonDownX = 0, mouseCursorOnButtonDownY = 0;
    bool mouseReleased = false;
    SDL_Event currentEvent;

    currentEvent.button.button = mouseEvent.button.button;
    SDL_GetMouseState(&mouseCursorOnButtonDownX, &mouseCursorOnButtonDownY);

    while (!mouseReleased) {
        SDL_PollEvent(&mouseEvent);
        if (mouseEvent.button.button == currentEvent.button.button && mouseEvent.type == SDL_MOUSEBUTTONUP) {
            mouseReleased = true;
        }
        SDL_GetMouseState(&mouseCursorX, &mouseCursorY);

        // render selector box
        SDL_RenderCopy(renderer, targetTexture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);
        SDL_Rect targetArea = { mouseCursorOnButtonDownX, mouseCursorOnButtonDownY, mouseCursorX - mouseCursorOnButtonDownX, mouseCursorY - mouseCursorOnButtonDownY };
        SDL_RenderFillRect(renderer, &targetArea);
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);
    }

    if (mouseCursorOnButtonDownX > mouseCursorX) {
        std::swap(mouseCursorOnButtonDownX, mouseCursorX);
    }

    if (mouseCursorOnButtonDownY > mouseCursorY) {
        std::swap(mouseCursorOnButtonDownY, mouseCursorY);
    }

    int firstX = (mouseCursorOnButtonDownX / GRID_TILE_SIZE) - 1;
    int firstY = (mouseCursorOnButtonDownY / GRID_TILE_SIZE) - 1;
    int lastX = (mouseCursorX / GRID_TILE_SIZE) - 1;
    int lastY = (mouseCursorY / GRID_TILE_SIZE) - 1;

    if (currentMode == 0) {
        switch (currentEvent.button.button) {
        case SDL_BUTTON_LEFT:
            for (int i = firstX; i <= lastX; i++) {
                for (int j = firstY; j <= lastY; j++) {
                    if ((i < gameMap->getWidth()) && (j < gameMap->getHeight()) && (i >=0 ) && (j >= 0)) {
                        gameMap->setTileWalls(i, j, selectedTile, selectedTile, selectedTile, selectedTile, "", "", NULL);
                        if (i - 1 >= 0 && i == firstX) {
                            gameMap->setTileWall(i - 1, j, Direction::E, selectedTile);
                        }

                        if (i + 1 < gameMap->getWidth() && i == lastX) {
                            gameMap->setTileWall(i + 1, j, Direction::W, selectedTile);
                        }

                        if (j - 1 >= 0 && j == firstY) {
                            gameMap->setTileWall(i, j - 1, Direction::S, selectedTile);
                        }

                        if (j + 1 < gameMap->getHeight() && j == lastY) {
                            gameMap->setTileWall(i, j + 1, Direction::N, selectedTile);
                        }
                    }
                }
            }
            break;

        case SDL_BUTTON_RIGHT:
            for (int i = firstX; i <= lastX; i++) {
                for (int j = firstY; j <= lastY; j++) {
                    if ((i < gameMap->getWidth()) && (j < gameMap->getHeight()) && (i >= 0) && (j >= 0)) {
                        gameMap->setTileWalls(i, j, "", "", "", "", selectedTile, selectedTile, NULL);
                        if (i - 1 >= 0) {
                            if (i == firstX && gameMap->getTile(i - 1, j)->isWalled(Direction::E)) {
                                gameMap->setTileWall(i, j, Direction::W, selectedTile);
                            }
                        }
                        else {
                            gameMap->setTileWall(i, j, Direction::W, selectedTile);
                        }

                        if (i + 1 < gameMap->getWidth()) {
                            if (i == lastX && gameMap->getTile(i + 1, j)->isWalled(Direction::W)) {
                                gameMap->setTileWall(i, j, Direction::E, selectedTile);
                            }
                        }
                        else {
                            gameMap->setTileWall(i, j, Direction::E, selectedTile);
                        }

                        if (j - 1 >= 0) {
                            if (j == firstY && gameMap->getTile(i, j - 1)->isWalled(Direction::S)) {
                                gameMap->setTileWall(i, j, Direction::N, selectedTile);
                            }
                        }
                        else {
                            gameMap->setTileWall(i, j, Direction::N, selectedTile);
                        }

                        if (j + 1 < gameMap->getHeight()) {
                            if (j == lastY && gameMap->getTile(i, j + 1)->isWalled(Direction::N)) {
                                gameMap->setTileWall(i, j, Direction::S, selectedTile);
                            }
                        }
                        else {
                            gameMap->setTileWall(i, j, Direction::S, selectedTile);
                        }
                    }
                }
            }
            break;

        case SDL_BUTTON_X2:
            gameMap->getTile(firstX, firstY)->spawnObject("Chest");
            break;

        case SDL_BUTTON_X1:
            gameMap->getTile(firstX, firstY)->deSpawnObject();
            break;
        }
    }

    if (currentMode == 1) {
        for (int i = firstX + 1; i <= lastX - 1; i++) {
            for (int j = firstY + 1; j <= lastY - 1; j++) {
                if ((i < gameMap->getWidth()) && (j < gameMap->getHeight()) && (i >= 0) && (j >= 0)) {
                    Direction direction = N;
                    for (int k = 0; k < 4; k++) {
                        if (gameMap->getTile(i, j)->isWalled(direction)) {
                            gameMap->setTileWall(i, j, direction, selectedTile);
                        }
                        direction++;
                    }
                }
            }
        }

        for (int i = firstX + 1; i <= lastX - 1; i++) {
            if ((i < gameMap->getWidth()) && (i >= 0) && (firstY >= 0)) {
                if (gameMap->getTile(i, firstY)->isWalled(Direction::S)) {
                    gameMap->setTileWall(i, firstY, Direction::S, selectedTile);
                }
            }
            if ((i < gameMap->getWidth()) && (lastY < gameMap->getHeight()) && (i >= 0)) {
                if (gameMap->getTile(i, lastY)->isWalled(Direction::N)) {
                    gameMap->setTileWall(i, lastY, Direction::N, selectedTile);
                }
            }
        }

        for (int i = firstY + 1; i <= lastY - 1; i++) {
            if ((i < gameMap->getHeight()) && (firstX >= 0) && (i >= 0)) {
                if (gameMap->getTile(firstX, i)->isWalled(Direction::E)) {
                    gameMap->setTileWall(firstX, i, Direction::E, selectedTile);
                }
            }
            if ((lastX < gameMap->getWidth()) && (i < gameMap->getHeight()) && (i >= 0)) {
                if (gameMap->getTile(lastX, i)->isWalled(Direction::W)) {
                    gameMap->setTileWall(lastX, i, Direction::W, selectedTile);
                }
            }
        }
    }

    if (currentMode == 2) {
        for (int i = firstX; i <= lastX; i++) {
            for (int j = firstY; j <= lastY; j++) {
                if ((i < gameMap->getWidth()) && (j < gameMap->getHeight()) && i >= 0 && j >= 0) {
                    gameMap->setFloorType(i, j, selectedTile);
                }
            }
        }
    }

    if (currentMode == 3) {
        for (int i = firstX; i <= lastX; i++) {
            for (int j = firstY; j <= lastY; j++) {
                if ((i < gameMap->getWidth()) && (j < gameMap->getHeight()) && i >= 0 && j >= 0) {
                    gameMap->setCeilingType(i, j, selectedTile);
                }
            }
        }
    }
}

std::vector<std::pair<std::string, SDL_Texture*>> loadTexturesFromDirectory(SDL_Renderer* renderer) {
    SDL_Surface* surface = nullptr;
    std::vector<std::string> allFiles;
    std::vector<std::pair<std::string, SDL_Texture*>> textureFiles;
    boost::filesystem::directory_iterator start(getEnvironmentTexturesDirectory());
    boost::filesystem::directory_iterator end;
    transform(start, end, back_inserter(allFiles), pathLeafString());

    textureFiles.clear();
    for (unsigned int i = 0; i < allFiles.size(); i++) {
        if (allFiles[i].substr(allFiles[i].length() - 4) == ".png") {
            surface = IMG_Load((getEnvironmentTexturesDirectory() + allFiles[i]).c_str());
            textureFiles.push_back(std::make_pair(allFiles[i].substr(0, allFiles[i].size()-4), SDL_CreateTextureFromSurface(renderer, surface)));
            SDL_FreeSurface(surface);
        }
    }

    return textureFiles;
}
