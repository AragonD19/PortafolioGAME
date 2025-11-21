// include/SceneManager.h
#pragma once
#include "Scene.h"
#include <memory>
#include <string>

class SceneManager {
public:
    void switchScene(const std::string& sceneName);  // En futuro: templates o factory
    void update(float dt);
    void render();

    Scene* getCurrentScene() { return currentScene.get(); }  // Para editor

private:
    std::unique_ptr<Scene> currentScene;
    std::string currentSceneName;
};