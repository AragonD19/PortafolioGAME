// include/scenes/MenuScene.h
#pragma once
#include "../Scene.h"

class MenuScene : public Scene {
public:
    MenuScene();
    void setup() override;
    void update(float dt) override;
    void render() override;
    void clean() override;

    bool startGame = false;  // Flag para switch a Breakout
};