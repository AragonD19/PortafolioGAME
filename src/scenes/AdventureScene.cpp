// src/scenes/AdventureScene.cpp

#include "scenes/AdventureScene.h"
#include "print.h"
#include <raylib.h>
#include <iostream>
#include "../perlin.h"
#include <cstring>  // Para memmove
#include <thread>   // Para std::thread
#include <mutex>    // Para std::mutex
#include <ostream>

AdventureScene::AdventureScene(int width, int height) : screen_width(width), screen_height(height) {}

Entity tilemapEnt;
Entity cameraEnt;

void AdventureScene::setup() {
    SetRandomSeed(time(NULL));  // Para random reproducible/variado cada run (e.g., en spawning)

    Texture2D playerIdle1 = LoadTexture("assets/MagoIdel1.png");
    Texture2D playerIdle2 = LoadTexture("assets/MagoIdel2.png");
    Texture2D playerIdle3 = LoadTexture("assets/MagoIdel3.png");
    Texture2D playerIdle4 = LoadTexture("assets/MagoIdel4.png");
    Texture2D playerLeft1 = LoadTexture("assets/MagoWalkL1.png");
    Texture2D playerLeft2 = LoadTexture("assets/MagoWalkL2.png");
    Texture2D playerRight1 = LoadTexture("assets/MagoWalkR1.png");
    Texture2D playerRight2 = LoadTexture("assets/MagoWalkR2.png");
    player = ecs.createEntity();
    ecs.addComponent(player, Position{{100.0f, 100.0f}});
    ecs.addComponent(player, Velocity{{0, 0}});
    Sprite playerSprite{playerIdle1};
    playerSprite.isSheet = false;  // Separate mode
    ecs.addComponent(player, playerSprite);
    Animation playerAnim;
    playerAnim.mode = AnimationMode::Separate;
    playerAnim.frameTime = 0.15f;
    playerAnim.texStates["idle"] = {playerIdle1,playerIdle2,playerIdle3,playerIdle4};
    playerAnim.texStates["walk_left"] = {playerLeft1, playerLeft2};
    playerAnim.texStates["walk_right"] = {playerRight1, playerRight2};
    ecs.addComponent(player, playerAnim);
    ecs.addComponent(player, InputControlled{});

    ecs.getComponent<Sprite>(player)->scale = {4.0f, 4.0f};  // 16x16 -> 64x64, visible en 800x600
    
    // Después de player setup
    tilemapEnt = ecs.createEntity();
    TileMap tilemap;
    tilemap.tileset = LoadTexture("assets/tileset.png");  // Asume existe
    if (tilemap.tileset.id == 0) {
        std::cerr << "Error: Tileset load failed! Check path 'assets/tileset.png'" << std::endl;
    } else {
        std::cout << "Tileset loaded: width=" << tilemap.tileset.width << ", height=" << tilemap.tileset.height << std::endl;
    }

    tilemap.wallTex = LoadTexture("assets/wall.png");  // Ajusta path—tu sprite para NON_WALKABLE specials
    tilemap.hazardTex = LoadTexture("assets/hazard.png");  // Para HAZARD
    tilemap.pickupTex = LoadTexture("assets/pickup.png");   // Para PICKUP

    if (tilemap.wallTex.id == 0) std::cerr << "Wall tex load failed!" << std::endl;
    if (tilemap.hazardTex.id == 0) std::cerr << "Hazard tex load failed!" << std::endl;
    if (tilemap.pickupTex.id == 0) std::cerr << "Pickup tex load failed!" << std::endl;

    // Procedural gen inicial: Un chunk central
    // unsigned int seed = 12345;
    unsigned int seed = time(NULL);  // Fijo para test (cambia a time(NULL) para random)
    PerlinNoise perlin(seed);

    // Inicial gen: Un chunk central
    tilemap.tiles.reserve(tilemap.maxWidth * tilemap.maxHeight);  // Pre-allocate max para evitar reallocs
    tilemap.width = tilemap.chunkSize * 2;  // Start 40x40
    tilemap.height = tilemap.chunkSize * 2;
    tilemap.tiles.resize(tilemap.width * tilemap.height);

    // Gen con offset world inicial (0,0 para center)
    int worldOffsetX = 0;
    int worldOffsetY = 0;
    generateChunk(tilemap, worldOffsetX, worldOffsetY, perlin);

    ecs.addComponent(tilemapEnt, tilemap);

    // Autotiling después de gen
    systemAutoTiling(ecs);

    // Añadir Health y Score a player
    ecs.addComponent(player, Health{});
    ecs.addComponent(player, Score{});


    cameraEnt = ecs.createEntity();
    CameraComp camComp;
    camComp.cam.offset = { (float)screen_width / 2.0f, (float)screen_height / 2.0f };  // Center screen
    camComp.cam.target = {0, 0};  // Inicial
    camComp.cam.rotation = 0.0f;
    camComp.cam.zoom = 1.0f;  // 1x, ajusta para zoom out
    camComp.target = player;  // Follow player
    ecs.addComponent(cameraEnt, camComp);


    // Enemigo 1: Tracking (persigue player si cerca)
    Entity enemyTracking = ecs.createEntity();
    ecs.addComponent(enemyTracking, Position{{600.0f, 300.0f}});
    ecs.addComponent(enemyTracking, Velocity{{0, 0}});
    Sprite enemySprite = {playerIdle1};  // Reuse, pero tint RED para diferenciar
    enemySprite.tint = RED;
    enemySprite.isSheet = false;
    enemySprite.scale = {4.0f, 4.0f};
    ecs.addComponent(enemyTracking, enemySprite);
    Animation enemyAnim = playerAnim;  // Reuse
    ecs.addComponent(enemyTracking, enemyAnim);
    MovementPattern trackPat;
    trackPat.type = MovementType::Tracking;
    trackPat.target = player;
    trackPat.speed = 150.0f;
    trackPat.pursuitDistance = 400.0f;
    trackPat.lerpFactor = 0.05f;  // Suave
    ecs.addComponent(enemyTracking, trackPat);

    // Enemigo 2: Circular (orbita punto fijo)
    Entity enemyCircular = ecs.createEntity();
    ecs.addComponent(enemyCircular, Position{{400.0f, 300.0f}});
    ecs.addComponent(enemyCircular, Velocity{{0, 0}});
    enemySprite.tint = GREEN;
    ecs.addComponent(enemyCircular, enemySprite);
    ecs.addComponent(enemyCircular, enemyAnim);
    MovementPattern circPat;
    circPat.type = MovementType::Circular;
    circPat.center = {400.0f, 300.0f};  // Fijo
    circPat.radius = 150.0f;
    circPat.angularSpeed = 1.5f;
    circPat.currentAngle = 0.0f;
    circPat.speed = 0;  // No usado, pero set por consistencia
    ecs.addComponent(enemyCircular, circPat);

    // Enemigo 3: Patrol (triángulo loop)
    Entity enemyPatrol = ecs.createEntity();
    ecs.addComponent(enemyPatrol, Position{{100.0f, 500.0f}});
    ecs.addComponent(enemyPatrol, Velocity{{0, 0}});
    enemySprite.tint = BLUE;
    ecs.addComponent(enemyPatrol, enemySprite);
    ecs.addComponent(enemyPatrol, enemyAnim);
    MovementPattern patPat;
    patPat.type = MovementType::Patrol;
    patPat.waypoints = {{100.0f, 500.0f}, {300.0f, 600.0f}, {200.0f, 400.0f}};
    patPat.speed = 120.0f;
    patPat.loop = true;
    patPat.arrivalThreshold = 10.0f;
    ecs.addComponent(enemyPatrol, patPat);



    // Spawner 1: LineHorizontal (e.g., fila de enemigos como alarma horizontal)
    Entity spawnerLineH = ecs.createEntity();
    EnemySpawner spawnLH;
    spawnLH.type = SpawnType::LineHorizontal;
    spawnLH.center = {200.0f, 200.0f};
    spawnLH.frequency = 8.0f;
    spawnLH.minEnemies = 3;
    spawnLH.maxEnemies = 4;
    spawnLH.spacing = 60.0f;
    spawnLH.activeDistance = 500.0f;
    ecs.addComponent(spawnerLineH, spawnLH);

    // Spawner 2: Circular (criaturas emergiendo de portal circular)
    Entity spawnerCirc = ecs.createEntity();
    EnemySpawner spawnC;
    spawnC.type = SpawnType::Circular;
    spawnC.center = {800.0f, 400.0f};
    spawnC.frequency = 12.0f;
    spawnC.minEnemies = 4;
    spawnC.maxEnemies = 6;
    spawnC.radius = 120.0f;
    spawnC.activeDistance = 600.0f;
    ecs.addComponent(spawnerCirc, spawnC);

    // Spawner 3: RandomArea (trampas aleatorias en zona)
    Entity spawnerRand = ecs.createEntity();
    EnemySpawner spawnR;
    spawnR.type = SpawnType::RandomArea;
    spawnR.center = {500.0f, 700.0f};
    spawnR.frequency = 10.0f;
    spawnR.minEnemies = 2;
    spawnR.maxEnemies = 5;
    spawnR.areaSize = {300.0f, 200.0f};
    spawnR.activeDistance = 400.0f;
    ecs.addComponent(spawnerRand, spawnR);


}



void AdventureScene::generateChunk(TileMap& tilemap, int offsetX, int offsetY, PerlinNoise& perlin) {
    const int chunkSize = tilemap.chunkSize;  // Precompute const
    for (int y = 0; y < chunkSize; ++y) {
        for (int x = 0; x < chunkSize; ++x) {
            int globalX = x + offsetX * chunkSize;
            int globalY = y + offsetY * chunkSize;
            int localY = y + (offsetY >= 0 ? offsetY * chunkSize : 0);  // Ajuste para negative offsets
            int localX = x + (offsetX >= 0 ? offsetX * chunkSize : 0);
            int index = localY * tilemap.width + localX;

            double noiseVal = perlin.noise(globalX * frequency, globalY * frequency);

            if (noiseVal > thresholdWall) {
                tilemap.tiles[index].value = IntGridValue::NON_WALKABLE;
            } else if (noiseVal < thresholdHazard) {
                tilemap.tiles[index].value = IntGridValue::HAZARD;
            } else {
                tilemap.tiles[index].value = IntGridValue::WALKABLE;
            }

            // Random pickups en walkable (raro, ~2%)
            if (tilemap.tiles[index].value == IntGridValue::WALKABLE && GetRandomValue(0, 100) < 2) {
                tilemap.tiles[index].value = IntGridValue::PICKUP;
            }
        }
    }
}



void AdventureScene::update(float dt) {
    systemInput(ecs);
    systemAI(ecs, dt);
    systemTileInteractions(ecs, dt);
    systemMovement(ecs, dt);
    systemAnimationUpdate(ecs, dt);
    systemEnemySpawn(ecs, dt);
    // Agrega lógica de juego, e.g., colisiones si expandes
    systemCameraUpdate(ecs, dt);

    // Opt: Cooldown para check expansión (e.g., cada 0.2s para evitar multiples triggers)
    static float expansionCooldown = 0.0f;
    expansionCooldown -= dt;
    if (expansionCooldown > 0.0f) return;
    expansionCooldown = 0.2f;  // Reset, aumentado para stability

    // Después de systemMovement
    auto* pos = ecs.getComponent<Position>(player);
    if (pos) {
        auto* tilemap = ecs.getComponent<TileMap>(tilemapEnt);
        if (tilemap) {
            const float edgeThreshold = tilemap->chunkSize * tilemap->tileSize * tilemap->scale / 2.0f;  // Precompute
            bool dirty = false;  // Flag para autotile solo si expanded

            // Right edge (append)
            if (pos->pos.x > (tilemap->width * tilemap->tileSize * tilemap->scale) - edgeThreshold && tilemap->width < tilemap->maxWidth) {
                int oldWidth = tilemap->width;
                tilemap->width += tilemap->chunkSize;
                tilemap->tiles.resize(tilemap->width * tilemap->height);
                std::thread genThread([this, tilemap, oldWidth] {
                    tileMutex.lock();
                    generateChunk(*tilemap, oldWidth / tilemap->chunkSize, 0, perlin);
                    tileMutex.unlock();
                });
                genThread.join();
                systemAutoTilingChunk(ecs, oldWidth - 1, 0, tilemap->width - 1, tilemap->height - 1);
                dirty = true;
            }

            // Bottom (append)
            if (pos->pos.y > (tilemap->height * tilemap->tileSize * tilemap->scale) - edgeThreshold && tilemap->height < tilemap->maxHeight) {
                int oldHeight = tilemap->height;
                tilemap->height += tilemap->chunkSize;
                tilemap->tiles.resize(tilemap->width * tilemap->height);
                std::thread genThread([this, tilemap, oldHeight] {
                    tileMutex.lock();
                    generateChunk(*tilemap, 0, oldHeight / tilemap->chunkSize, perlin);
                    tileMutex.unlock();
                });
                genThread.join();
                systemAutoTilingChunk(ecs, 0, oldHeight - 1, tilemap->width - 1, tilemap->height - 1);
                dirty = true;
            }

            // Left edge (insert at front, shift right)
            if (pos->pos.x < edgeThreshold && tilemap->width < tilemap->maxWidth) {
                int oldWidth = tilemap->width;
                tilemap->width += tilemap->chunkSize;
                tilemap->tiles.resize(tilemap->width * tilemap->height);

                // Shift existing tiles right by chunkSize
                for (int y = 0; y < tilemap->height; ++y) {
                    std::memmove(&tilemap->tiles[y * tilemap->width + tilemap->chunkSize], &tilemap->tiles[y * oldWidth], sizeof(Tile) * oldWidth);
                }

                std::thread genThread([this, tilemap] {
                    tileMutex.lock();
                    generateChunk(*tilemap, -1, 0, perlin);
                    tileMutex.unlock();
                });
                genThread.join();
                systemAutoTilingChunk(ecs, 0, 0, tilemap->chunkSize, tilemap->height - 1);
                dirty = true;

                // Adjust ALL positions and patterns (después de gen y autotile)
                float offsetX = static_cast<float>(tilemap->chunkSize) * tilemap->tileSize * tilemap->scale;
                for (auto& [ent, comp] : ecs.getComponentMap<Position>()) {
                    comp.pos.x += offsetX;
                }
                // Adjust pattern params fijos
                for (auto& [ent, pat] : ecs.getComponentMap<MovementPattern>()) {
                    if (pat.type == MovementType::Patrol) {
                        for (auto& wp : pat.waypoints) {
                            wp.x += offsetX;
                        }
                    } else if (pat.type == MovementType::Circular && !pat.aroundTarget) {
                        pat.center.x += offsetX;
                    }
                }
                // Adjust spawners centers
                for (auto& [ent, spawn] : ecs.getComponentMap<EnemySpawner>()) {
                    spawn.center.x += offsetX;
                }
                print("Shift left: offsetX=", offsetX, " Adjusted entities and spawners");
            }

            // Top edge (insert at front, shift down)
            if (pos->pos.y < edgeThreshold && tilemap->height < tilemap->maxHeight) {
                int oldHeight = tilemap->height;
                tilemap->height += tilemap->chunkSize;
                tilemap->tiles.resize(tilemap->width * tilemap->height);

                // Shift existing tiles down by chunkSize rows
                for (int y = oldHeight - 1; y >= 0; --y) {
                    std::memmove(&tilemap->tiles[(y + tilemap->chunkSize) * tilemap->width], &tilemap->tiles[y * tilemap->width], sizeof(Tile) * tilemap->width);
                }

                std::thread genThread([this, tilemap] {
                    tileMutex.lock();
                    generateChunk(*tilemap, 0, -1, perlin);
                    tileMutex.unlock();
                });
                genThread.join();
                systemAutoTilingChunk(ecs, 0, 0, tilemap->width - 1, tilemap->chunkSize);
                dirty = true;

                // Adjust ALL positions and patterns (después de gen y autotile)
                float offsetY = static_cast<float>(tilemap->chunkSize) * tilemap->tileSize * tilemap->scale;
                for (auto& [ent, comp] : ecs.getComponentMap<Position>()) {
                    comp.pos.y += offsetY;
                }
                // Adjust pattern params fijos
                for (auto& [ent, pat] : ecs.getComponentMap<MovementPattern>()) {
                    if (pat.type == MovementType::Patrol) {
                        for (auto& wp : pat.waypoints) {
                            wp.y += offsetY;
                        }
                    } else if (pat.type == MovementType::Circular && !pat.aroundTarget) {
                        pat.center.y += offsetY;
                    }
                }
                // Adjust spawners centers
                for (auto& [ent, spawn] : ecs.getComponentMap<EnemySpawner>()) {
                    spawn.center.y += offsetY;
                }
                print("Shift top: offsetY=", offsetY, " Adjusted entities and spawners");
            }

            if (dirty) {
                // No needed con chunk autotile, pero mantiene por si
            }
        }
    }
}



void AdventureScene::render() {
    // Get camera
    auto* camComp = ecs.getComponent<CameraComp>(cameraEnt);
    if (camComp) {
        BeginMode2D(camComp->cam);
    }

    // Render map y entities
    systemRenderTileMap(ecs);
    systemRenderSprites(ecs);

    // Debug spawners (solo si toggleado)
    if (debugSpawners) {
        systemDebugSpawners(ecs);
    }

    if (camComp) {
        EndMode2D();
    }

    // Health/Score (fixed UI, fuera de camera para no scroll)
    if (auto* health = ecs.getComponent<Health>(player)) {
        DrawText(TextFormat("Health: %.0f", health->value), 10, 30, 20, RED);
    }
    if (auto* score = ecs.getComponent<Score>(player)) {
        DrawText(TextFormat("Score: %d", score->value), 10, 50, 20, GREEN);
    }
}

void AdventureScene::clean() {
    
    // Similar para player (repíte código o haz función helper)
    if (auto* playerAnim = ecs.getComponent<Animation>(player)) {
        if (playerAnim->mode == AnimationMode::Sheet) {
            UnloadTexture(ecs.getComponent<Sprite>(player)->texture);
        } else {
            for (auto& [state, frames] : playerAnim->texStates) {
                for (auto& tex : frames) UnloadTexture(tex);
            }
        }
    }

    if (auto* tm = ecs.getComponent<TileMap>(tilemapEnt)) {
        UnloadTexture(tm->tileset);
    }


    if (auto* tm = ecs.getComponent<TileMap>(tilemapEnt)) {
        UnloadTexture(tm->tileset);
        UnloadTexture(tm->wallTex);
        UnloadTexture(tm->hazardTex);
        UnloadTexture(tm->pickupTex);
    }

}