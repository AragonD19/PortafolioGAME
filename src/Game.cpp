// src/Game.cpp
#include "Game.h"
#include <iostream>
#include "print.h"
#include <rlImGui.h>

Game::Game(const char* title, int width, int height) 
    : screen_width(width), screen_height(height), editor(paused) { 
    InitWindow(screen_width, screen_height, title);
    SetTargetFPS(60);
    rlImGuiSetup(true);

    
    switchScene("Menu");
}

Game::~Game() {}

void Game::setup() {
    
    if (currentScene) currentScene->setup();
}

void Game::frame_start() {}

void Game::handle_events() {}

void Game::update(float dt) {
    if (paused) return;

    if (currentScene) {
        currentScene->update(dt);


        
        if (auto* menu = dynamic_cast<MenuScene*>(currentScene.get())) {
            if (menu->startGame) {
                switchScene("Adventure");
            }
        }
        
    }
}

void Game::render() {
    BeginDrawing();
    ClearBackground(BLACK);

    if (currentScene) currentScene->render();

    // Nuevo: Debug IntGrid (wrap en camera si existe)
    if (editor.debugIntGrid) {
        auto& ecs = currentScene->getECS();
        // Encuentra la camera entity (asumiendo solo una—puedes iterar si múltiples)
        CameraComp* camComp = nullptr;
        for (auto& [ent, comp] : ecs.getComponentMap<CameraComp>()) {
            camComp = &comp;
            break;  // Toma la primera
        }
        if (camComp) {
            BeginMode2D(camComp->cam);
        }
        systemDebugIntGrid(ecs);
        if (camComp) {
            EndMode2D();
        }
    }

    DrawFPS(10, 10);

    rlImGuiBegin();
    editor.renderGUI(currentScene.get());
    rlImGuiEnd();

    EndDrawing();
}

void Game::frame_end() {}

void Game::clean() {
    if (cleaned) return;
    cleaned = true;

    if (currentScene) currentScene->clean();

    rlImGuiShutdown();
    CloseWindow();
}

bool Game::running() {
    return isRunning && !WindowShouldClose();
}

void Game::switchScene(const std::string& sceneName) {
    if (currentScene) currentScene->clean();

    if (sceneName == "Menu") {
        currentScene = std::make_unique<MenuScene>();
    } else if (sceneName == "Adventure") {
        currentScene = std::make_unique<AdventureScene>(screen_width, screen_height);
    } else {
        std::cerr << "Unknown scene: " << sceneName << std::endl;
        return;
    }

    currentSceneName = sceneName;
    currentScene->setup();
}