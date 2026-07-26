# COMPUTERGRAFIK - Assignments

This repository contains a collection of assignments for a "Computergraphik" (Computer Graphics) course. The assignments are structured to guide students through fundamental concepts and advanced techniques in computer graphics, utilizing C++ and CMake.

## Course Overview

The curriculum covers a range of topics, progressing from basic C++ programming to complex graphics implementations:
*   **Basic C++ Programming:** Foundational programming concepts are reinforced in early assignments.
*   **Image Processing:** Introduction to digital image representation, manipulation, and fundamental algorithms.
*   **Ray Tracing:** Implementation of ray tracing techniques for realistic image synthesis, including object intersection, lighting, and scene construction.
*   **Real-time Rasterization:** Development of a rasterization pipeline for interactive graphics, covering scene management, camera controls, various lighting models, and rendering of geometric primitives.

## Technologies Used

*   **Primary Language:** C++ (primarily C++14 and C++17 standards)
*   **Build System:** CMake
*   **Key Libraries (especially in later assignments):**
    *   **GLFW:** Used for creating windowed environments and managing OpenGL contexts.
    *   **GLAD:** An essential OpenGL loader to access OpenGL functions.
    *   **GLM (OpenGL Mathematics):** A header-only library that provides GLSL-like types and functions for C++, crucial for 3D mathematics.
    *   **Dear ImGui:** A minimalist, bloat-free graphical user interface library for in-application debugging and tools.
    *   **glowl:** (Likely a custom or course-specific library providing abstractions for OpenGL functionalities).

## Project Structure and Navigation

The assignments are organized into `AufgabenblattXX` directories, where `XX` represents the assignment number (e.g., `Aufgabenblatt01`, `Aufgabenblatt02`). Each directory typically contains its own `CMakeLists.txt` and source files relevant to that assignment.

-   **`Aufgabenblatt01/`**: Introduction to C++ fundamentals and basic programming tasks.
-   **`Aufgabenblatt02/`**: Focus on image processing and manipulation.
-   **`Aufgabenblatt03/`**: Introduction to ray generation and object intersection, forming the basis of ray tracing.
-   **`Aufgabenblatt04/`**: Advanced Ray Tracing features including point lights and shadows.
-   **`Aufgabenblatt05/`**: (Assignment details/code to be added).
-   **`Aufgabenblatt06/`**: Advanced topics covering rasterization, scene graphs, lighting, and interactive rendering using OpenGL-related libraries.
-   **`Aufgabenblatt07/`**: Image processing on CPU, featuring convolution filters (Gaussian, Laplacian) and boundary handling.
-   **`Aufgabenblatt08/`**: Efficient rendering of large scenes using Instanced Rendering and Billboards (Imposters), utilizing Direct State Access (DSA).
-   **`Aufgabenblatt09/`**: Scene Shading, covering scene graphs, vertex data management, Phong lighting, and Normal Mapping.

Each `AufgabenblattXX` folder contains a dedicated `README.md` with a more detailed description of the specific tasks and context for that assignment.

## Prerequisites

*   **C++ compiler** supporting **C++14** (e.g., g++ >= 5, clang++, or MSVC). A C++17-capable compiler is recommended for the later OpenGL assignments.
*   **CMake >= 3.5**.
*   **OpenGL 4.4+** (4.5+ recommended) plus **GLFW**, **GLAD**, **GLM**, and **Dear ImGui** for the real-time assignments (`Aufgabenblatt06`-`Aufgabenblatt09`). `Aufgabenblatt09` additionally uses **glowl** and **tinygltf**.
*   The early ray-tracing assignments (`Aufgabenblatt03`, `Aufgabenblatt04`) have **no external dependencies** beyond a C++14 compiler and CMake.

## Building and Running Assignments

There are two supported ways to build.

### Option A - Build everything from the repository root

A root `CMakeLists.txt` is provided that wires up every assignment sheet that ships its own `CMakeLists.txt` via `add_subdirectory()` (guarded by `EXISTS` checks, so missing sheets are skipped). From the repository root:

```bash
cmake -B build
cmake --build build
```

> Note: `Aufgabenblatt03` and `Aufgabenblatt04` build targets are named `Raytracer03` and `Raytracer04` (they originally both produced a `Raytracer` target). `Aufgabenblatt06` and `Aufgabenblatt07` both define an `ImageViewer` target, so only `Aufgabenblatt06` is included in the root build - build `Aufgabenblatt07` per-sheet.

### Option B - Build a single sheet (per-sheet)

Each assignment sheet has its own `CMakeLists.txt` and can be built independently. This is the recommended approach when you only work on one sheet or when a sheet needs OpenGL dependencies that are not available globally.

For the ray tracer sheets (no OpenGL deps), e.g. `Aufgabenblatt03`:

```bash
cd Aufgabenblatt03/code
cmake -B build
cmake --build build
```

For an OpenGL sheet, e.g. `Aufgabenblatt06`:

```bash
cd Aufgabenblatt06/00_student_setup/code
cmake -B build
cmake --build build
```

**Running Executables**

After a successful build, the executable files will be located in the build directory (often within a `Debug` or `Release` subfolder, depending on the generator).

Run the Blatt 03 ray tracer:

```bash
# Linux/macOS
./Aufgabenblatt03/code/build/Raytracer03

# Windows
.\Aufgabenblatt03\code\build\Raytracer03.exe
```

Run the `ImageViewer` from `Aufgabenblatt06`:

```bash
# Linux/macOS
./Aufgabenblatt06/00_student_setup/code/build/ImageViewer

# Windows
.\Aufgabenblatt06\00_student_setup\code\build\ImageViewer.exe
```
*(The exact path and executable name will vary depending on the specific assignment and your operating system.)*

## Development Conventions

*   **Language Standard:** Modern C++ features are used, adhering to C++14 and C++17 standards.
*   **Memory Management:** `std::shared_ptr` is commonly employed for robust object management within scene graphs and object hierarchies, promoting safer memory handling.
*   **Code Structure:** Code is logically organized into modules. Note that the codebase does **not** use a `namespace cg` (or any project namespace) despite earlier documentation claims - symbols live in the global namespace.
*   **Documentation:** Code includes comments (some in German) explaining complex logic and `TODO` markers indicating areas for student implementation.

---
*This `README.md` was generated by a Gemini-powered AI agent to provide a comprehensive overview of the project.*