// include/scenes/AdventureScene.h
#pragma once
#include "../Scene.h"
#include "../components.h"
#include "../systems.h"
#include "../perlin.h"  // Nuevo: Para PerlinNoise
#include <mutex>  // Para std::mutex

class AdventureScene : public Scene {
public:
    AdventureScene(int width, int height);
    void setup() override;
    void update(float dt) override;
    void render() override;
    void clean() override;
    bool debugSpawners = true;
    

private:
    int screen_width, screen_height;
    Entity player, enemy;
    PerlinNoise perlin;

    // Nuevo: Parámetros para procedural gen (accesibles en métodos)
    float frequency = 0.03f;
    float thresholdWall = 0.65f;
    float thresholdHazard = 0.2f;
    

    // Nuevo: Mutex para safe thread access a tiles
    std::mutex tileMutex;

    // Nuevo: Método privado para gen de chunks
    void generateChunk(TileMap& tilemap, int offsetX, int offsetY, PerlinNoise& perlin);
};