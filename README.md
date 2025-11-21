# GAME Engine – Un Motor de Videojuegos en C++23

## 📘 Descripción

**GAME Engine** es un motor de videojuegos ligero y extensible construido desde cero en **C++23**, diseñado específicamente para entornos **Linux Mint**.  
Está basado en una arquitectura **ECS (Entity-Component-System) pura**, enfocada en maximizar rendimiento y flexibilidad.

Permite crear:

- Mundos **infinitos** generados proceduralmente  
- **IA modular**  
- Un **editor en vivo** integrado con ImGui  
- Soporte para **múltiples escenas** (menú, Arkanoid/Breakout, roguelike, etc.)

Este engine **no es un framework** como Unity o Godot.  
Es un núcleo de bajo nivel para desarrolladores que quieren **control total**, sin dependencias pesadas (solo *raylib* + *rlImGui*).

---

## 🚀 ¿Por qué este engine?

- ⚡ **Rendimiento nativo:** ECS data-oriented con zero overhead y optimización para cachés L1/L2.  
- 🌍 **Procedural infinito:** Mundos que crecen en tiempo real sin pantallas de carga.  
- 🛠️ **Editor en vivo:** Modifica entidades mientras juegas.  
- ♻️ **Reutilizable:** El mismo núcleo ejecuta múltiples juegos/escenas sin cambiar el core.  
- 🐧 **Linux-first:** Probado y desarrollado en Linux Mint con CMake.

Este repositorio es parte de un proyecto académico avanzado.

---

## 🖼️ Capturas de Pantalla y Demos

### 🌍 Mundo Procedural
- Expansión dinámica en 4 direcciones  
- Autotiling automático

### 🛠️ Editor en Vivo Integrado
- Lista de entidades e inspector  
- Modificación de IA en runtime ("Patrol", "Tracking", etc.)

---

# 🧩 1. Arquitectura ECS Pura

- Componentes como `Position`, `Velocity`, `MovementPattern`  
- Sistemas independientes:  
  - `systemMovement()`  
  - `systemAI()`  
  - `systemRenderSprites()`  
- Soporte para **hot add/remove** de componentes en tiempo real  

---

# 🌄 2. Generación Procedural Infinita

- Perlin Noise (header-only)  
- Chunks de **20×20 tiles** generados en threads secundarios  
- Expansión en 4 direcciones con *coordinate shifting*  
- Autotiling con bitmasking  

---

# 🖥️ 3. Editor Integrado con rlImGui

- Lista de entidades + inspector
- Edita posiciones, velocidad, salud, IA, etc. **sin pausar el juego**
- Toggles de debug:  
  - Grid  
  - Spawners  
  - Colisiones  
- Atajos:  
  - **F1** abre/cierra editor  
  - **P** pausa el juego  

---

# 🤖 4. IA Modular y Spawners

### Patrones de Movimiento
- **Tracking** (persecución)
- **Circular** (órbitas)
- **Patrol** (waypoints)

### Spawners
- LineHorizontal  
- Circular  
- RandomArea  

Comportamientos emergentes mediante la combinación de patrones.

---

# 🎬 5. Scene Manager y Reutilización

- Clase base: `Scene` con `setup()`, `update()`, `render()`, `clean()`  
- Escenas incluidas:
  - `MenuScene`
  - `BreakoutScene`
  - `AdventureScene`
- Cambios de escena suaves con limpieza automática de recursos

---

# 🧱 6. Otros Features

- Cámara smooth follow con lerp  
- Colisiones AABB y tile-based  
- Animaciones (spritesheet o frames separados)  
- UI integrada (health, score)  
- Debug FPS y overlays  

---

# 📦 Requisitos

- **SO:** Linux Mint 21.3+ o Ubuntu-based  
- **Compilador:** GCC 13+ (C++23)  
- **Dependencias:**
  - `raylib` → `sudo apt install libraylib-dev`
  - `CMake` → `sudo apt install cmake`
  - Submodules (ImGui + rlImGui) incluidos en `external/`

---

# 🔧 Instalación

```bash
git clone <repo>
cd GAME
git submodule update --init --recursive

```
mkdir build
cd build
cmake ..
make -j$(nproc)
cd GAME
git submodule update --init --recursive

```
Ejecuta:
```

```
./GAME
```
O usa:
```
./run.sh
```


📁 Estructura del Código

```

GAME/
├── assets/             # Texturas, spritesheets, etc.
├── build/              # Artefactos de build (gitignore)
├── external/           # ImGui y rlImGui (submodules)
├── include/            # ecs.h, components.h, systems.h, editor/, scenes/
├── src/                # main.cpp, Game.cpp, systems.cpp, editor/, scenes/
├── CMakeLists.txt      # Configuración de build
├── LICENSE             # MIT
└── README.md           # Este archivo
