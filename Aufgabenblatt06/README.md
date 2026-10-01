# Aufgabenblatt 06 - Real-time Rasterization and Interactive Graphics

A software rasterizer (points / wireframe / filled triangles with z-buffer, Bresenham
lines, barycentric interpolation and point plus ambient lighting) with an OpenGL image
viewer. The default scene is a procedurally generated DNA double helix.

## Build and Run

```bash
# from the repository root
cmake -S Aufgabenblatt06/00_student_setup/code -B Aufgabenblatt06/00_student_setup/code/build
cmake --build Aufgabenblatt06/00_student_setup/code/build --parallel

cd Aufgabenblatt06/00_student_setup/code/build
./ImageViewer
```

The sheet's CMakeLists defaults to a release build (rasterizing is slow in Debug, which
matters for the interactive auto-update). Start the viewer from `code/build` or `code/`:
it looks for its shaders under `../shader/`, `../../shader/` and `shader/` relative to
the working directory. GLFW, ImGui (1.86) and GLM are fetched by CMake on the first
configure, so that step needs network access.

## Notes

- Target: `ImageViewer`; needs an OpenGL 4.4-capable driver (see the root README).
- The written answers to task 1.5 (z-buffer and clipping) are in `aufgabe-1-5.md`.
- `scene/` holds the scene graph, camera and lights; `image/` the image class and the
  viewer; `Rasterizer.cpp` the drawing code.
