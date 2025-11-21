// src/scenes/MenuScene.cpp
#include "MenuScene.h"

MenuScene::MenuScene() {}

void MenuScene::setup() {
    // Setup menú: Por ahora, solo flags; en futuro, entidades UI
}

void MenuScene::update(float dt) {
    // Lógica de input: e.g., si presiona space, startGame = true
    if (IsKeyPressed(KEY_SPACE)) {
        startGame = true;
    }
}

void MenuScene::render() {
    DrawText("Presiona SPACE para empezar Breakout", 100, 200, 20, BLACK);
    // En futuro, render UI entities
}

void MenuScene::clean() {
    // Limpieza
}