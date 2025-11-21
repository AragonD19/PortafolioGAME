// src/scenes/BreakoutScene.cpp
#include "BreakoutScene.h"
#include <iostream>
#include "../print.h"

BreakoutScene::BreakoutScene(int width, int height) : screen_width(width), screen_height(height) {}

void BreakoutScene::setup() {
    // Mismo código de Game::setup anterior
    paddle = ecs.createEntity();
    ecs.addComponent(paddle, Position{screen_width / 2.f - 50.f, screen_height - 40.f});
    ecs.addComponent(paddle, Size{100.f, 20.f});
    ecs.addComponent(paddle, Velocity{0.f, 0.f});
    ecs.addComponent(paddle, PaddleControlled{});

    ball = ecs.createEntity();
    ecs.addComponent(ball, Position{screen_width / 2.f - 7.5f, screen_height / 2.f - 7.5f});
    ecs.addComponent(ball, Size{15.f, 15.f});
    ecs.addComponent(ball, Velocity{0.f, -150.f});
    ecs.addComponent(ball, Ball{});

    const int cols = 4;
    const int rows = 2;
    const float blockWidth = screen_width / (float)cols;
    const float blockHeight = 20.f;

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            Entity block = ecs.createEntity();
            ecs.addComponent(block, Position{x * blockWidth + 2, y * (blockHeight + 5) + 40});
            ecs.addComponent(block, Size{blockWidth - 4, blockHeight});
            ecs.addComponent(block, Block{});
        }
    }
}

void BreakoutScene::update(float dt) {
    systemPaddleControl(ecs, dt, screen_width);
    systemBallMovement(ecs, dt, screen_width, screen_height, isRunning);
    systemBallPaddleCollision(ecs);
    systemBallBlockCollision(ecs);

    if (ecs.getComponentMap<Block>().empty()) {
        std::cout << "You Win!" << std::endl;
        isRunning = false;  // Podrías signal al manager para switch scene
    }

#ifdef DEBUG
    auto* ballPos = ecs.getComponent<Position>(ball);
    if (ballPos) vprint(*ballPos);
#endif
}

void BreakoutScene::render() {
    systemRender(ecs);
}

void BreakoutScene::clean() {
    // Limpieza específica de escena (e.g., clear ECS)
    // Por ahora vacío; en futuro, unload assets
}