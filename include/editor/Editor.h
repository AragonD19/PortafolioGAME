// include/editor/Editor.h
#pragma once
#include "../ecs.h"
#include "../components.h"
#include <imgui.h>

class Scene;  // Forward declare

class Editor {
public:
    Editor(bool& paused_ref);  // Removido ECS ref, lo tomará de scene
    void renderGUI(Scene* currentScene);  // Modificado: Toma scene
    bool debugIntGrid = false;  // Toggle para overlays

private:
    bool& paused;
    Entity selectedEntity = -1;

    void drawEntityList(ECS& ecs);
    void drawInspector(ECS& ecs);
    void drawControls();
};