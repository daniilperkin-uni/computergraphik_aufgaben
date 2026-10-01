# Aufgabenblatt 08 - Instanced Rendering and Imposters

Renders a forest of 192 trees with instanced draw calls, and the same trees as billboard
imposters, using Direct State Access (OpenGL 4.5) for all buffers, VAOs and textures.

## Build and Run

```bash
# from the repository root
cmake -S Aufgabenblatt08/00_student_setup/code -B Aufgabenblatt08/00_student_setup/code/build
cmake --build Aufgabenblatt08/00_student_setup/code/build --parallel

cd Aufgabenblatt08/00_student_setup/code/build
./Geometrie
```

Start it from `code/build`: the shaders (`../shaders/`) and the tree sprite
(`../resources/pine_tree_sprite.png`) are loaded relative to the working directory. GLFW
and GLM are fetched by CMake on the first configure, so that step needs network access.

## Controls

- `W` / `A` / `S` / `D`: move the camera; `Shift` / `Space`: up / down
- Arrow keys: rotate; gamepads are supported as well
- `R`: reload the shaders

## Notes

- Target: `Geometrie`; needs an OpenGL 4.5-capable driver (DSA entry points).
- `g_render_sprites` at the top of `src/main.cpp` switches between the 3D tree geometry
  and the imposter version.
