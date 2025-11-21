// include/components.h

#pragma once
#include <raylib.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "ecs.h"


using Entity = size_t;  // Forward alias—igual que en ecs.h, evita dependencia circular

enum class AnimationMode { Sheet, Separate };

struct Position { Vector2 pos; };  // Cambiado a Vector2
struct Velocity { Vector2 vel; };  // Cambiado a Vector2
struct Size { float w, h; };  // Mantiene, pero podría ser Vector2
struct PaddleControlled {};  // Viejo, mantiene
struct Ball {};
struct Block {};

// Modificado para híbrido
struct Sprite {
    Texture2D texture;  // Para sheet: el atlas; para separate: ignorado o base
    Rectangle frameRec = {0, 0, 16, 16};  // Usado solo en sheet
    Vector2 origin = {0, 0};
    Color tint = WHITE;
    bool isSheet = true;  // Nuevo: Flag para render (default sheet)
    Vector2 scale = {1.0f, 1.0f};  // Nuevo: Escala por sprite (default 1x)
};

// Modificado para híbrido: Un mapa por tipo de frame
struct Animation {
    std::string currentState = "idle";
    AnimationMode mode = AnimationMode::Sheet;  // Default sheet
    std::unordered_map<std::string, std::vector<Rectangle>> rectStates;  // Para sheet
    std::unordered_map<std::string, std::vector<Texture2D>> texStates;   // Para separate
    int currentFrame = 0;
    float frameTime = 0.1f;
    float timer = 0.0f;
};

// Nuevo: Para control por input
struct InputControlled {};

// Nuevo: Para AI simple (patrulla horizontal)
struct AIPatrol {
    float speed = 100.0f;
    float direction = 1.0f;  // 1 derecha, -1 izquierda
    float patrolDistance = 200.0f;
    float traveled = 0.0f;
};



enum class IntGridValue {
    WALKABLE = 0,      // Vacío, caminable
    NON_WALKABLE = 1,  // Muro, no caminable
    HAZARD = 2,        // Daño al player (e.g., espinas)
    PICKUP = 3         // Item recolectable (e.g., coin)
    // Extensible para más (e.g., DOOR=4, TELEPORT=5 en futuras fases)
};

struct Tile {
    IntGridValue value = IntGridValue::WALKABLE;
    Vector2 frame = {0.0f, 0.0f};  // ix, iy para sub-rect en tileset (precomputado por autotiling)
    bool needsAutoTiling = false;  // Flag para procesar en autotiling
    Texture2D specialTex = {0};    // Nuevo: Para textures fuera del tileset (e.g., hazard spike)
};

struct TileMap {
    int width = 20;  // Inicial, crece
    int height = 20;
    int tileSize = 16;
    float scale = 4.0f;
    Texture2D tileset;
    std::vector<Tile> tiles;  // Resize on gen
    int chunkSize = 20;       // Nuevo: Tamaño chunk
    int maxWidth = 50;       // Límite
    int maxHeight = 50;
    unsigned int seed = 12345;  // Para Perlin consistente

    Texture2D wallTex = {0};
    Texture2D hazardTex = {0};
    Texture2D pickupTex = {0};
};

// Nuevo: Para interacciones (añade a player si hazard/pickup)
struct Health { float value = 100.0f; };
struct Score { int value = 0; };


struct CameraComp {  // Renombrado para evitar conflicto con Raylib's Camera2D
    Camera2D cam = {0};  // Raylib struct: offset, target, rotation, zoom
    Entity target = -1;   // Entity a seguir (e.g., player)
    float smoothSpeed = 0.1f;  // Para lerp follow (opcional, 0=instant)
};

enum class MovementType { None, Tracking, Circular, Patrol };

struct MovementPattern {
    MovementType type = MovementType::None;
    float speed = 100.0f;  // Compartido: Velocidad base

    // Para Tracking
    Entity target = -1;    // e.g., player
    float pursuitDistance = 500.0f;  // Max dist para perseguir (0=siempre)
    float lerpFactor = 0.1f;  // Suavizado (0=instant, 1=ninguno)

    // Para Circular
    Vector2 center = {0.0f, 0.0f};  // Punto fijo
    float radius = 100.0f;
    float angularSpeed = 2.0f;  // rad/s (positivo=anti-clockwise)
    float currentAngle = 0.0f;  // Estado interno
    bool aroundTarget = false;  // Si true, center = target pos dinámica

    // Para Patrol
    std::vector<Vector2> waypoints;  // Puntos a patrullar
    size_t currentWaypoint = 0;  // Estado interno
    bool loop = true;  // Repetir ciclo
    float arrivalThreshold = 5.0f;  // Dist para cambiar waypoint
};


enum class SpawnType { LineHorizontal, LineVertical, Circular, RandomArea };

struct EnemySpawner {
    SpawnType type = SpawnType::LineHorizontal;
    Vector2 center = {0.0f, 0.0f};  // Posición base o centro del área
    float frequency = 10.0f;  // Segundos entre spawns
    int minEnemies = 3;       // Min num a spawnear (random entre min-max)
    int maxEnemies = 5;
    float spacing = 50.0f;    // Para Line: distancia entre enemies
    float radius = 100.0f;    // Para Circular
    Vector2 areaSize = {200.0f, 200.0f};  // Para Random: ancho/alto del rect
    float activeDistance = 300.0f;  // Spawn solo si player < esta dist (0=siempre)
    float timer = 0.0f;       // Estado interno
};
