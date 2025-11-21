// include/systems.h

#pragma once
#include "ecs.h"
#include "components.h"
#include <raylib.h>

bool checkCollision(const Position& aPos, const Size& aSize, const Position& bPos, const Size& bSize);

void systemPaddleControl(ECS& ecs, float dt, int screenWidth);
void systemBallMovement(ECS& ecs, float dt, int screenWidth, int screenHeight, bool& isRunning);
void systemBallPaddleCollision(ECS& ecs);
void systemBallBlockCollision(ECS& ecs);
void systemRender(ECS& ecs);


void systemInput(ECS& ecs);
void systemAI(ECS& ecs, float dt);  // Mantiene, pero ahora maneja MovementPattern
void systemMovement(ECS& ecs, float dt);
void systemAnimationUpdate(ECS& ecs, float dt);
void systemRenderSprites(ECS& ecs);

// Para Tilemaps e IntGrid
void systemAutoTiling(ECS& ecs);
void systemRenderTileMap(ECS& ecs);
void systemTileInteractions(ECS& ecs, float dt);  // Aplica effects (damage, pickup)
void systemDebugIntGrid(ECS& ecs);

void systemCameraUpdate(ECS& ecs, float dt);  // Actualiza cam target
void systemRenderWithCamera(ECS& ecs);  // No needed—wrap en scene render
void systemAutoTilingChunk(ECS& ecs, int startX, int startY, int endX, int endY);

void systemEnemySpawn(ECS& ecs, float dt);
void systemDebugSpawners(ECS& ecs);  // Para overlays de zonas