// src/editor/Editor.cpp
#include "editor/Editor.h"
#include "../Scene.h"
#include <imgui.h>
#include <string>
#include <AdventureScene.h>

Editor::Editor(bool& paused_ref) : paused(paused_ref) {}

void Editor::renderGUI(Scene* currentScene) {
    if (!currentScene) return;
    ECS& ecs = currentScene->getECS();

    drawControls();
    drawEntityList(ecs);
    if (selectedEntity != -1) {
        drawInspector(ecs);
    }

    if (auto* advScene = dynamic_cast<AdventureScene*>(currentScene)) {
        ImGui::Checkbox("Debug Spawners", &advScene->debugSpawners);
    }
}


void Editor::drawControls() {
    ImGui::Begin("Game Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    if (ImGui::Button(paused ? "Resume" : "Pause")) {
        paused = !paused;
    }
    ImGui::Checkbox("Debug IntGrid", &debugIntGrid);
    ImGui::End();
}


void Editor::drawEntityList(ECS& ecs) {
    ImGui::Begin("Entities");
    ImGui::Text("Active Entities: %zu", ecs.getAllEntities().size());  // Count dinámico

    // Categorías genéricas por component (expansible)
    if (ImGui::TreeNode("Players (InputControlled)")) {
        for (auto& [e, _] : ecs.getComponentMap<InputControlled>()) {
            std::string label = "Player Entity " + std::to_string(e);
            if (ImGui::Selectable(label.c_str(), selectedEntity == e)) {
                selectedEntity = e;
            }
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("AI Entities (AIPatrol)")) {
        for (auto& [e, _] : ecs.getComponentMap<AIPatrol>()) {
            std::string label = "AI Entity " + std::to_string(e);
            if (ImGui::Selectable(label.c_str(), selectedEntity == e)) {
                selectedEntity = e;
            }
        }
        ImGui::TreePop();
    }

    // Fallback: Lista all si no matches (para Breakout legacy)
    if (ImGui::TreeNode("Other Entities")) {
        auto allEntities = ecs.getAllEntities();
        for (Entity e : allEntities) {
            // Skip si ya en categorías arriba
            if (ecs.hasComponent<InputControlled>(e) || ecs.hasComponent<AIPatrol>(e)) continue;
            std::string label = "Entity " + std::to_string(e);
            if (ImGui::Selectable(label.c_str(), selectedEntity == e)) {
                selectedEntity = e;
            }
        }
        ImGui::TreePop();
    }

    ImGui::End();
}



void Editor::drawInspector(ECS& ecs) {
    ImGui::Begin("Inspector");
    ImGui::Text("Selected Entity: %zu", selectedEntity);
    if (auto* pos = ecs.getComponent<Position>(selectedEntity)) {
        ImGui::Text("Position:");
        ImGui::InputFloat("X", &pos->pos.x);  
        ImGui::InputFloat("Y", &pos->pos.y);  
    }
    if (auto* vel = ecs.getComponent<Velocity>(selectedEntity)) {
        ImGui::Text("Velocity:");
        ImGui::InputFloat("VX", &vel->vel.x);  
        ImGui::InputFloat("VY", &vel->vel.y);  
    }
    if (auto* size = ecs.getComponent<Size>(selectedEntity)) {
        ImGui::Text("Size:");
        ImGui::InputFloat("Width", &size->w);
        ImGui::InputFloat("Height", &size->h);
    }
    if (auto* anim = ecs.getComponent<Animation>(selectedEntity)) {
        ImGui::Text("Animation:");
        ImGui::InputFloat("Frame Time", &anim->frameTime);
    }
    if (auto* sprite = ecs.getComponent<Sprite>(selectedEntity)) {
        ImGui::Text("Sprite:");
        ImGui::Checkbox("Is Sheet", &sprite->isSheet);
        ImGui::InputFloat2("Scale", (float*)&sprite->scale);  
        ImGui::ColorEdit3("Tint", (float*)&sprite->tint.r);   
    }  
    

    ImGui::End();
}


