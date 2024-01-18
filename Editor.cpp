#include "Editor.hpp"

void runMapEditor(GameMap* gameMap) {

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("Map Editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, window_flags);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Event event;
    bool quit = false;

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
                processMouseAction(gameMap, event);
            }
        }

        // rendering map tiles
        SDL_SetRenderDrawColor(renderer, (Uint8)(backgroundColour.x * 255), (Uint8)(backgroundColour.y * 255), (Uint8)(backgroundColour.z * 255), (Uint8)(backgroundColour.w * 255));
        SDL_RenderClear(renderer);

        for (int i = 0; i < gameMap->getWidth(); i++) {
            for (int j = 0; j < gameMap->getHeight(); j++) {
                int numberOfWalls = 0;
                SDL_Rect fillRect;
                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
                fillRect = { i * GRID_TILE_SIZE, j * GRID_TILE_SIZE, TILE_SIZE, TILE_SIZE };
                SDL_RenderFillRect(renderer, &fillRect);
                SDL_SetRenderDrawColor(renderer, 0xAA, 0x66, 0x00, 0xFF);

                if (gameMap->getTile(i, j)->isWalled(Direction::N)) {
                    numberOfWalls++;
                    fillRect = { i * GRID_TILE_SIZE, j * GRID_TILE_SIZE, TILE_SIZE, WALL_THICKNESS };
                    SDL_RenderFillRect(renderer, &fillRect);
                }

                if (gameMap->getTile(i, j)->isWalled(Direction::E)) {
                    numberOfWalls++;
                    fillRect = { TILE_SIZE - WALL_THICKNESS + i * GRID_TILE_SIZE, j * GRID_TILE_SIZE, WALL_THICKNESS, TILE_SIZE };
                    SDL_RenderFillRect(renderer, &fillRect);
                }

                if (gameMap->getTile(i, j)->isWalled(Direction::S)) {
                    numberOfWalls++;
                    fillRect = { i * GRID_TILE_SIZE, TILE_SIZE-WALL_THICKNESS + j * GRID_TILE_SIZE, TILE_SIZE, WALL_THICKNESS };
                    SDL_RenderFillRect(renderer, &fillRect);
                }

                if (gameMap->getTile(i, j)->isWalled(Direction::W)) {
                    numberOfWalls++;
                    fillRect = { i * GRID_TILE_SIZE, j * GRID_TILE_SIZE, WALL_THICKNESS, TILE_SIZE };
                    SDL_RenderFillRect(renderer, &fillRect);
                }

                if (numberOfWalls == 4) {
                    fillRect = { i * GRID_TILE_SIZE, j * GRID_TILE_SIZE, TILE_SIZE, TILE_SIZE };
                    SDL_RenderFillRect(renderer, &fillRect);
                }

                SDL_SetRenderDrawColor(renderer, 0x00, 0x66, 0xAA, 0xFF);
                if (gameMap->getTile(i, j)->containsObject()) {
                    fillRect = { i * GRID_TILE_SIZE + TILE_SIZE / 4, j * GRID_TILE_SIZE + TILE_SIZE / 4, TILE_SIZE / 2, TILE_SIZE / 2 };
                    SDL_RenderFillRect(renderer, &fillRect);
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

            ImGui::ColorEdit3("Background colour", (float*)&backgroundColour);
            ImGui::Checkbox("Show Second Window", &show_demo_window);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // show demo window
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // Rendering
        ImGui::Render();
        SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
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
	IMG_Quit();
	SDL_Quit();
}

void processMouseAction(GameMap* gameMap, SDL_Event mouseEvent) {
    int mouseCursorX = 0, mouseCursorY = 0;
    int mouseCursorOnButtonDownX = 0, mouseCursorOnButtonDownY = 0;
    bool mouseReleased = false;
    SDL_Event currentEvent;

    currentEvent.button.button = mouseEvent.button.button;
    SDL_GetMouseState(&mouseCursorOnButtonDownX, &mouseCursorOnButtonDownY);

    while (!mouseReleased) {
        SDL_PollEvent(&mouseEvent);
        if (mouseEvent.button.button == currentEvent.button.button && mouseEvent.type == SDL_MOUSEBUTTONUP) {
            SDL_GetMouseState(&mouseCursorX, &mouseCursorY);
            mouseReleased = true;
        }
    }

    if (mouseCursorOnButtonDownX > mouseCursorX) {
        std::swap(mouseCursorOnButtonDownX, mouseCursorX);
    }

    if (mouseCursorOnButtonDownY > mouseCursorY) {
        std::swap(mouseCursorOnButtonDownY, mouseCursorY);
    }

    int firstX = mouseCursorOnButtonDownX / GRID_TILE_SIZE;
    int firstY = mouseCursorOnButtonDownY / GRID_TILE_SIZE;
    int lastX = mouseCursorX / GRID_TILE_SIZE;
    int lastY = mouseCursorY / GRID_TILE_SIZE;

    switch (currentEvent.button.button) {
    case SDL_BUTTON_LEFT:
        for (int i = firstX; i <= lastX; i++) {
            for (int j = firstY; j <= lastY; j++) {
                if ((i < gameMap->getWidth()) && (j < gameMap->getHeight())) {
                    gameMap->setTileWalls(i, j, "basicWall", "basicWall", "basicWall", "basicWall", "", "", NULL);
                    if (i - 1 >= 0 && i == firstX) {
                        gameMap->setTileWall(i - 1, j, Direction::E, "basicWall");
                    }

                    if (i + 1 < gameMap->getWidth() && i == lastX) {
                        gameMap->setTileWall(i + 1, j, Direction::W, "basicWall");
                    }

                    if (j - 1 >= 0 && j == firstY) {
                        gameMap->setTileWall(i, j - 1, Direction::S, "basicWall");
                    }

                    if (j + 1 < gameMap->getHeight() && j == lastY) {
                        gameMap->setTileWall(i, j + 1, Direction::N, "basicWall");
                    }
                }
            }
        }
        break;

    case SDL_BUTTON_RIGHT:
        for (int i = firstX; i <= lastX; i++) {
            for (int j = firstY; j <= lastY; j++) {
                if ((i < gameMap->getWidth()) && (j < gameMap->getHeight())) {
                    gameMap->setTileWalls(i, j, "", "", "", "", "basicFloor", "basicCeiling", NULL);
                    if (i - 1 >= 0) {
                        if (i == firstX && gameMap->getTile(i - 1, j)->isWalled(Direction::E)) {
                            gameMap->setTileWall(i, j, Direction::W, "basicWall");
                        }
                    }
                    else {
                        gameMap->setTileWall(i, j, Direction::W, "basicWall");
                    }

                    if (i + 1 < gameMap->getWidth()) {
                        if (i == lastX && gameMap->getTile(i + 1, j)->isWalled(Direction::W)) {
                            gameMap->setTileWall(i, j, Direction::E, "basicWall");
                        }
                    }
                    else {
                        gameMap->setTileWall(i, j, Direction::E, "basicWall");
                    }

                    if (j - 1 >= 0) {
                        if (j == firstY && gameMap->getTile(i, j - 1)->isWalled(Direction::S)) {
                            gameMap->setTileWall(i, j, Direction::N, "basicWall");
                        }
                    }
                    else {
                        gameMap->setTileWall(i, j, Direction::N, "basicWall");
                    }

                    if (j + 1 < gameMap->getHeight()) {
                        if (j == lastY && gameMap->getTile(i, j + 1)->isWalled(Direction::N)) {
                            gameMap->setTileWall(i, j, Direction::S, "basicWall");
                        }
                    }
                    else {
                        gameMap->setTileWall(i, j, Direction::S, "basicWall");
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
