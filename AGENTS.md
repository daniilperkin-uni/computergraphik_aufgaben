# COMPUTERGRAFIK Project Overview

This repository contains a series of assignments for a "Computergraphik" (Computer Graphics) course. The assignments are structured to progressively introduce students to fundamental concepts and advanced techniques in computer graphics, using C++ and CMake.

The project evolves through various topics:
*   **Basic C++ Programming:** Early assignments focus on core C++ programming concepts.
*   **Image Processing:** Introduction to image representation and manipulation.
*   **Ray Tracing:** Implementation of basic ray tracing techniques for rendering scenes.
*   **Real-time Rasterization:** Development of a rasterization pipeline including scene management, camera controls, lighting models, and geometric primitives using OpenGL-related libraries.

**Key Technologies and Libraries:**
*   **Language:** C++ (primarily C++14 and C++17 standards)
*   **Build System:** CMake
*   **Graphics Libraries (later assignments):**
    *   **GLFW:** For creating windows and handling OpenGL contexts.
    *   **GLAD:** An OpenGL loader.
    *   **GLM:** OpenGL Mathematics (header-only library for C++ with GLSL-like types and functions).
    *   **Dear ImGui:** A bloat-free graphical user interface library for C++.
    *   **glowl:** open-source lightweight C++ wrapper library for OpenGL objects (shader programs, textures, buffers); vendored in sheets 06 and 07.

## Building and Running the Project

The project uses CMake for its build system. Each `Aufgabenblatt` (assignment sheet) often has its own `CMakeLists.txt` and can be built independently or as part of the larger project structure.

**General Build Steps:**

1.  **Configure** out of source, pointing `-S` at the sheet that contains the `CMakeLists.txt` and `-B` at its build directory:
    ```bash
    cmake -S Aufgabenblatt06/00_student_setup/code -B Aufgabenblatt06/00_student_setup/code/build
    ```
    Use `cmake -S . -B build` to configure all sheets from the repository root.

2.  **Build:**
    ```bash
    cmake --build Aufgabenblatt06/00_student_setup/code/build --parallel
    ```

**Running Executables:**

After a successful build, executables will typically be found in the respective build directories (e.g., `build/Debug` or `cmake-build-debug`). The exact path and executable name will vary per assignment.

For example, to run the `ImageViewer` from `Aufgabenblatt06`:
```bash
./Aufgabenblatt06/00_student_setup/code/build/ImageViewer # On Linux/macOS
.\Aufgabenblatt06\00_student_setup\code\build\ImageViewer.exe # On Windows
```

## Development Conventions

*   **Language Standard:** The project utilizes modern C++ features, primarily adhering to C++14 and C++17 standards.
*   **Memory Management:** Extensive use of `std::shared_ptr` is observed for robust object ownership and memory management, especially within scene graphs and object hierarchies.
*   **Code Structure:** Code is organized into logical directories (e.g., `scene`, `image`, `shader`). The codebase does **not** use a `namespace cg` (or any project namespace) - symbols live in the global namespace, despite earlier documentation claims to the contrary.
*   **Comments and Documentation:** Code includes comments, often in German, providing explanations and `TODO` markers for assignment tasks.
*   **Assignment-Driven Development:** The codebase is designed around individual assignments (`AufgabenblattXX`), where students are expected to implement specific functionalities within the provided framework.

## Build

There is a **root `CMakeLists.txt`** that wires up every sheet which ships its own `CMakeLists.txt` via `add_subdirectory()` (guarded by `EXISTS`). You can configure everything from the repo root, or build a single sheet.

**Root build (all sheets):**
```bash
cmake -B build
cmake --build build
```

**Per-sheet build (recommended for a single sheet, or when OpenGL deps are not globally available):**
```bash
# Ray tracer - Blatt 03 (no external deps)
cd Aufgabenblatt03/code
cmake -B build
cmake --build build

# Ray tracer - Blatt 04 (no external deps)
cd Aufgabenblatt04/code
cmake -B build
cmake --build build

# OpenGL sheets (need OpenGL 4.4+, GLFW, GLAD, GLM, ImGui)
cd Aufgabenblatt06/00_student_setup/code
cmake -B build
cmake --build build
```

Target names: `Raytracer03` (Blatt 03), `Raytracer04` (Blatt 04), `ColorSpaces` (Blatt 02), `ImageViewer` (Blatt 06 & 07 - build only one in a shared tree), `Geometrie` (Blatt 08), `SceneShading` (Blatt 09).

## Known Issues

The following issues were identified and fixed in this refactor of the ray-tracing assignments (`Aufgabenblatt03`, `Aufgabenblatt04`):

*   **Color-clamp bug (fixed):** `saveAsPPM()` (and `comparePPM()` in Blatt 03) clamped framebuffer values to `[0, 255]`, but the framebuffer stores colors in normalized `[0, 1]` range. Clamping to `[0, 255]` left values above `1.0` un-clipped and produced wrong colors on write. Changed to `Vec3d::clamp(0., 1., ...)`. (Blatt 04 was already correct.)
*   **ODR violation risk (fixed):** `create_scene()` / `create_scene_objects()` / `create_scene_lights()` were defined as non-`inline` free functions in headers (`scene.h`). Including the header into more than one translation unit would cause multiple-definition linker errors. All such functions are now `inline`.
*   **Dead code removed:** the unused `static int state = {42};` was removed from `Aufgabenblatt03/code/util.h` (it was never referenced there). Note: the identical declaration in `Aufgabenblatt04/code/util.h` is **kept** because it is referenced by `cpRand()`. The unused `const static int SEED = 42;` was removed from both `main.cpp` files, along with the `// HIER IST DER FIX:` debug comment in Blatt 03.
*   **Encapsulation restored (Blatt 04):** `Plane::_point`, `Plane::_normal`, `Sphere::_radius` and `Sphere::_center` were changed from `public` back to `protected`, matching Blatt 03.
*   **`castRay` refactor (Blatt 04):** the 73-line `castRay` was split into `inShadow()`, `computeDirectLighting()` and a thin `castRay()` orchestrator. The ambient term (`k_a`) is now added **once** outside the light loop instead of being re-added per light (which previously caused ambient double-counting and an over-bright image). This is an intentional behavior change beyond the clamp fix.
*   **Plane parallel test (Blatt 03):** `Vec3d::approxEq(denom, 0.)` was replaced with a domain threshold `std::abs(denom) > 1e-6` to avoid dividing by a near-zero denominator. Blatt 04 already used a threshold (`denom < -1.e-6`) and was left unchanged to preserve its one-sided intersection behavior.
*   **CMake modernization:** both `CMakeLists.txt` files now use explicit source listings (no `file(GLOB)`), `if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")` / `if(MSVC)` (no `${VAR}` dereference), and `CXX_STANDARD 14` consistently. Blatt 03/04 targets renamed to `Raytracer03`/`Raytracer04` so both can coexist in a root build.
*   **Docs:** the false `namespace cg` claim was removed from `README.md` and `AGENTS.md`. A root `CMakeLists.txt` was added so the "configure all from root" instructions actually work.

## Assignment Progress

### Aufgabenblatt 07: Image Filtering on CPU
*   **Core Logic:** Implemented `filterImage` for general convolution and `offsetImageCoordinates` for boundary handling.
*   **Filters:**
    *   **Edge Detection:** Laplacian 3x3 kernel implementation.
    *   **Gaussian Blur:** Standard 2D Gaussian and optimized Separable Gaussian implementation.
*   **Border Policies:** Implemented `MIRROR` and `REPEAT` policies for handling out-of-bounds coordinates.
*   **Fixes:** Resolved CMake compatibility issues with the embedded `glfw` library.

### Aufgabenblatt 08: Instanced Rendering & Imposters
*   **Techniques:** Implemented **Instanced Rendering** to efficiently render a forest of trees and **Imposters (Billboards)** for level-of-detail optimization.
*   **Modern OpenGL:** Utilized **Direct State Access (DSA)** (OpenGL 4.5+) for creating and managing Vertex Buffers, VAOs, and Textures (`glCreateBuffers`, `glNamedBufferData`, `glCreateTextures`, etc.).
*   **Camera:** Implemented camera control with View and Projection matrix calculations (`glm::lookAt`, `glm::perspective`) and input handling (Keyboard/Joystick).
*   **Shaders:** Integrated shaders for instanced geometry and texture mapping for sprites.

### Aufgabenblatt 09: Scene Shading
*   **Scene Graph:** Implemented transformation hierarchies to compute local and global world matrices for scene objects.
*   **Vertex Data:** Mapped non-interleaved geometry data (Positions, Normals, UVs, Tangents) to specific shader locations using Vertex Buffers.
*   **Phong Lighting:** Implemented a multi-light source Phong reflection model calculating diffuse, specular, and distance attenuation components.
*   **Normal Mapping:** Transformed normals using Tangent-Space (TBN) matrices to add fine geometric details from textures.

