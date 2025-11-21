// include/ecs.h

#pragma once
#include <unordered_map>
#include <typeindex>
#include <type_traits>
#include <vector>      
#include <unordered_set> 
#include "components.h"     

using Entity = size_t;

class ECS {
public:
    Entity createEntity() {
        return nextEntity++;
    }

    template<typename T>
    void addComponent(Entity e, T comp) {
        auto& map = getComponentMap<T>();
        map[e] = comp;
    }

    template<typename T>
    T* getComponent(Entity e) {
        auto& map = getComponentMap<T>();
        auto it = map.find(e);
        if (it != map.end()) return &it->second;
        return nullptr;
    }

    template<typename T>
    std::unordered_map<Entity, T>& getComponentMap() {
        static std::unordered_map<Entity, T> map;
        return map;
    }

    template<typename T>
    void removeComponent(Entity e) {
        auto& map = getComponentMap<T>();
        map.erase(e);
    }

    template<typename T>
    bool hasComponent(Entity e) {
        auto& map = getComponentMap<T>();
        return map.find(e) != map.end();
    }
    // Nuevo: Query all entities (genérico para editor)
    std::vector<Entity> getAllEntities() {
        std::unordered_set<Entity> uniqueEntities;
        // Merge de todos los component maps (evita dups)
        auto mergeMap = [&](auto& map) {
            for (auto& [e, _] : map) uniqueEntities.insert(e);
        };
        mergeMap(getComponentMap<Position>());
        mergeMap(getComponentMap<Sprite>());  // Prioriza visuals
        mergeMap(getComponentMap<InputControlled>());
        mergeMap(getComponentMap<AIPatrol>());
        // Agrega más si expandes (e.g., Block, Ball)
        return std::vector<Entity>(uniqueEntities.begin(), uniqueEntities.end());
    }

    // Nuevo: Remove entity completely (erase from all component maps)
    void removeEntity(Entity e) {
        // Call remove for each known component type (add new as project grows)
        removeComponent<Position>(e);
        removeComponent<Velocity>(e);
        removeComponent<Size>(e);
        removeComponent<PaddleControlled>(e);
        removeComponent<Ball>(e);
        removeComponent<Block>(e);
        removeComponent<Sprite>(e);
        removeComponent<Animation>(e);
        removeComponent<InputControlled>(e);
        removeComponent<Tile>(e);  // Si usas individual tiles
        removeComponent<TileMap>(e);
        removeComponent<Health>(e);
        removeComponent<Score>(e);
        removeComponent<CameraComp>(e);
        removeComponent<MovementPattern>(e);
        removeComponent<EnemySpawner>(e);
        // Si añades más components (e.g., future Door), agrega aquí
    }

    

private:
    Entity nextEntity = 0;
    
    
};