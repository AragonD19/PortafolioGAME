#!/bin/bash

# Nombre del ejecutable
EXECUTABLE="GAME"

# Directorio donde se hará el build
BUILD_DIR="build"

# Flags opcionales
CLEAN=false
VERBOSE=false

# Parse args: -c para clean, -v para verbose
while [[ $# -gt 0 ]]; do
    case $1 in
        -c|--clean)
            CLEAN=true
            shift
            ;;
        -v|--verbose)
            VERBOSE=true
            shift
            ;;
        *)
            echo "Uso: $0 [-c] [-v]"
            exit 1
            ;;
    esac
done

# Crear carpeta de build si no existe
if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
fi

# Entrar a la carpeta de build
cd "$BUILD_DIR" || exit 1

# Clean si flag
if [ "$CLEAN" = true ]; then
    echo "==> Limpiando build anterior..."
    rm -rf *
fi

# CMake flags para verbose si pedido
CMAKE_CMD="cmake .."
if [ "$VERBOSE" = true ]; then
    CMAKE_CMD="$CMAKE_CMD -DCMAKE_VERBOSE_MAKEFILE=ON"
fi

echo "==> Generando proyecto con CMake..."
$CMAKE_CMD || { echo "CMake falló"; exit 1; }

# Make con paralelismo (usa todos los cores en Mint)
MAKE_CMD="make -j$(nproc)"
if [ "$VERBOSE" = true ]; then
    MAKE_CMD="$MAKE_CMD VERBOSE=1"
fi

echo "==> Compilando..."
$MAKE_CMD || { echo "Compilación falló"; exit 1; }

# Verifica copy de assets (opcional, para debug)
if [ "$VERBOSE" = true ] && [ -d "assets" ]; then
    echo "==> Assets copiados: $(ls assets/ | wc -l) archivos en build/assets/"
fi

# Ejecutar el programa (trap Ctrl+C para cd back)
echo "==> Ejecutando $EXECUTABLE... (Ctrl+C para salir)"
trap 'cd ..; echo "Salida limpia.";' INT
./$EXECUTABLE

# Cd back al root
cd ..
echo "==> Build completo. Listo para próxima iteración."