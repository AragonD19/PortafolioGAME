// include/scenes/BreakoutScene.h
#pragma once
#include "../Scene.h"
#include "../components.h"
#include "../systems.h"

class BreakoutScene : public Scene {
public:
    BreakoutScene(int width, int height);
    void setup() override;
    void update(float dt) override;
    void render() override;
    void clean() override;

private:
    int screen_width;
    int screen_height;
    bool isRunning = true;
    Entity paddle;
    Entity ball;
};