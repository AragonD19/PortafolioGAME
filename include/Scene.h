// include/Scene.h
#pragma once
#include "ecs.h"
#include <raylib.h>

class Scene {
public:
    virtual ~Scene() = default;
    virtual void setup() = 0;
    virtual void update(float dt) = 0;
    virtual void render() = 0;
    virtual void clean() = 0;

    ECS& getECS() { return ecs; }  // Acceso para editor/manager

protected:
    ECS ecs;
};