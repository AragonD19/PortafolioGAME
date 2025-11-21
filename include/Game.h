// include/Game.h
#pragma once
#include <raylib.h>
#include <memory>
#include "Scene.h"
#include "scenes/BreakoutScene.h"
#include "scenes/MenuScene.h"
#include "editor/Editor.h"
#include "scenes/AdventureScene.h"

class Game {
public:
    Game(const char* title, int width, int height);
    ~Game();

    void setup();
    void frame_start();
    void handle_events();
    void update(float dt);
    void render();
    void frame_end();
    void clean();
    bool running();

private:
    int screen_width;
    int screen_height;
    bool isRunning = true;
    bool paused = false;
    bool cleaned = false;

    std::unique_ptr<Scene> currentScene;  // Nuevo: Current scene
    std::string currentSceneName;  // Para switching

    void switchScene(const std::string& sceneName);  // Nuevo

    Editor editor;  // Modificado: Pasará currentScene->getECS()
};