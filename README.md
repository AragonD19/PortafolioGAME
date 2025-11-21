GAME Engine - Un Motor de Videojuegos

Descripción
GAME Engine es un motor de videojuegos ligero y extensible construido desde cero en C++23, diseñado específicamente para entornos Linux Mint. Utiliza una arquitectura ECS (Entity-Component-System) pura para maximizar el rendimiento y la flexibilidad, permitiendo la creación de juegos con mundos infinitos generados proceduralmente, IA modular, un editor en vivo integrado con ImGui, y soporte para múltiples escenas (desde un menú simple hasta un Arkanoid clásico o un roguelike de aventura).
Este engine no es un framework como Unity o Godot; es un núcleo bajo nivel que prioriza el control total del desarrollador. Ideal para programadores que quieren entender cada píxel y cada frame, sin dependencias pesadas más allá de raylib para gráficos y rlImGui para el editor.
¿Por qué este engine?

Rendimiento nativo: ECS data-oriented con zero overhead, optimizado para cachés L1/L2.
Procedural infinito: Mundos que crecen en tiempo real sin loadings, usando Perlin Noise y chunks dinámicos.
Editor en vivo: Modifica entidades, IA y el mundo mientras juegas – iteración 100x más rápida.
Reutilizable: El mismo núcleo corre Breakout, un menú y un roguelike infinito sin cambiar código central.
Linux-first: Desarrollado y testeado en Linux Mint, con CMake para builds limpios.

Este repositorio es el resultado de un proyecto académico avanzado.
Capturas de Pantalla y Demos
Mundo Procedural 
Expansión dinámica en 4 direcciones con autotiling.
Editor en Vivo Integrado
Editor Inspector
Modificando patrones de IA en runtime – cambia de "Patrol" a "Tracking" y ve el enemigo perseguirte al instante.

1. Arquitectura ECS Pura

Componentes como Position, Velocity, MovementPattern – todo data-driven.
Sistemas independientes: systemMovement(), systemAI(), systemRenderSprites().
Soporte para hot-add/remove de componentes en runtime.

2. Generación Procedural Infinita

Perlin Noise header-only para ruido 2D.
Chunks de 20x20 tiles generados en threads secundarios.
Expansión en 4 direcciones con coordinate shifting para mantener el origen estable.
Autotiling automático con reglas de bitmask para transiciones suaves.

3. Editor Integrado con rlImGui

Lista de entidades y inspector en vivo.
Edita posiciones, velocidades, patrones de IA, salud, etc., sin pausar el juego.
Toggles para debug: IntGrid overlay, spawners visuales.
Atajo: F1 para abrir, P para pausar.

4. IA Modular y Spawners

MovementPattern: Tracking (persecución con distancia), Circular (órbitas), Patrol (waypoints con loop).
Spawners: LineHorizontal, Circular, RandomArea – activados por proximidad al jugador.
Comportamiento emergente: mezcla patrones para hordas complejas.

5. Scene Manager y Reutilización

Clase base Scene con setup(), update(), render(), clean().
Escenas incluidas: MenuScene, BreakoutScene (Arkanoid), AdventureScene (roguelike).
Switch seamless con limpieza automática de recursos.

6. Otros Features

Cámara smooth follow con lerp.
Colisiones AABB y tile-based.
Animaciones híbridas (spritesheet o frames separados).
Soporte para health/score UI fija.
Debug FPS y overlays integrados.

Requisitos

Sistema Operativo: Linux Mint (testeado en 21.3) o cualquier distro Ubuntu-based.
Compilador: g++ con soporte C++23 (GCC 13+ recomendado).
Dependencias:
raylib (instala con sudo apt install libraylib-dev).
CMake 3.10+ (sudo apt install cmake).
Git submodules para ImGui y rlImGui (incluidos en external/).



Clona el repositorio
git submodule update --init --recursive  # Para ImGui y rlImGui
Crea el directorio de build:textmkdir build
cd build
Configura y compila con CMake:textcmake ..
make -j$(nproc)  # Usa todos los cores para build rápido
Ejecuta el engine:text./GAME

o puedes ejecutar el run.sh

Modo Juego: Ejecuta ./GAME – empieza en el menú. Presiona ENTER para ir a Adventure o Breakout.
Modo Editor: Presiona F1 en cualquier escena para abrir el inspector. Selecciona entidades y edita en vivo.
Extensión: Añade una nueva escena heredando de Scene y regístrala en Game::switchScene().
Assets: Coloca texturas en assets/ – el engine las carga automáticamente (e.g., "assets/hero.png").

Ejemplo rápido para añadir un enemigo nuevo en AdventureScene::setup():
C++Entity nuevoEnemigo = ecs.createEntity();
ecs.addComponent(nuevoEnemigo, Position{{500.0f, 300.0f}});
ecs.addComponent(nuevoEnemigo, MovementPattern{MovementType::Tracking, .target = player, .speed = 150.0f});
Estructura del Código
textGAME/
├── assets/             # Texturas, spritesheets, etc.
├── build/              # Artefactos de build (gitignore)
├── external/           # ImGui y rlImGui (submodules)
├── include/            # Headers: ecs.h, components.h, systems.h, editor/, scenes/
├── src/                # Fuentes: main.cpp, Game.cpp, systems.cpp, editor/, scenes/
├── CMakeLists.txt      # Configuración de build
├── LICENSE             # MIT
└── README.md           # Este archivo


Autor: [Tu Nombre] – Programador experto en C++ y engines bajo nivel.
Email: tu-email@example.com
GitHub Issues: Abre uno para bugs o features.
Créditos: raylib por raysan5 (motor gráfico), ImGui por ocornut (editor UI).

¡Construye tu próximo gran juego con este engine! Si tienes dudas, como experto te digo: el código es autoexplicativo – sumérgete y hackea. 🚀
