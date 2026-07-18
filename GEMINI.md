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
    *   **glowl:** (likely a custom or course-specific library for OpenGL abstractions).

## Building and Running the Project

The project uses CMake for its build system. Each `Aufgabenblatt` (assignment sheet) often has its own `CMakeLists.txt` and can be built independently or as part of the larger project structure.

**General Build Steps:**

1.  **Create a build directory:** It is recommended to create a `build` directory at the root of the project or within specific assignment folders.
    ```bash
    mkdir build
    cd build
    ```
2.  **Configure CMake:** Run CMake to generate the build system files (e.g., Makefiles or Visual Studio projects). If running from the project root `build` directory, you might configure for all assignments.
    ```bash
    cmake ..
    ```
    To configure a specific assignment (e.g., `Aufgabenblatt06/00_student_setup/code`):
    ```bash
    mkdir Aufgabenblatt06/00_student_setup/code/build
    cd Aufgabenblatt06/00_student_setup/code/build
    cmake ../../..
    ```
    *Note: The `cmake ..` or `cmake ../../..` path depends on where you create your build directory relative to the `CMakeLists.txt` you want to configure.*

3.  **Build the project:** Compile the source code using the generated build system.
    ```bash
    cmake --build .
    ```
    This command should be executed from the build directory created in the previous step.

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
*   **Code Structure:** Code is organized into logical directories (e.g., `scene`, `image`, `shader`) and uses namespaces (`cg`) to prevent naming collisions.
*   **Comments and Documentation:** Code includes comments, often in German, providing explanations and `TODO` markers for assignment tasks.
*   **Assignment-Driven Development:** The codebase is designed around individual assignments (`AufgabenblattXX`), where students are expected to implement specific functionalities within the provided framework.

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

