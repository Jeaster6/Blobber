#include "Editor.hpp"

void runMapEditor(GameMap* gameMap) {

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("Map Editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, window_flags);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Event event;
    bool quit = false;

    SDL_Init(SDL_INIT_VIDEO);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                quit = true;
            }
            else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window)) {
                quit = true;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                processMouseAction(gameMap, event);
            }
        }

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

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

        SDL_RenderPresent(renderer);
    }

    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

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
