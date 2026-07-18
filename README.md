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

Each `AufgabenblattXX` folder contains a dedicated `README.md` with a more detailed description of the specific tasks and context for that assignment.

## Building and Running Assignments

To build and run the assignments, you will need a C++ compiler (like g++ or MSVC) and CMake installed.

**General Build Steps:**

1.  **Open a Terminal/Command Prompt:** Navigate to the root directory of this repository.
2.  **Create a Build Directory:** It's good practice to create a separate directory for build artifacts.
    ```bash
    mkdir build
    cd build
    ```
3.  **Configure CMake:**
    To configure the entire project at once (this will generate build files for all sub-projects):
    ```bash
    cmake ..
    ```
    Alternatively, to configure a specific assignment (e.g., `Aufgabenblatt06`):
    ```bash
    # Navigate into the assignment's build directory (or create it)
    mkdir ../Aufgabenblatt06/00_student_setup/code/build
    cd ../Aufgabenblatt06/00_student_setup/code/build
    # Then run cmake, pointing to the source directory of that assignment
    cmake ../../..
    ```
    *(Adjust the `cmake` command's path (`..` or `../../..`) based on your current directory relative to the `CMakeLists.txt` you wish to build.)*

4.  **Build the Project:** Compile the source code.
    ```bash
    cmake --build .
    ```
    This command compiles all targets defined in the configured `CMakeLists.txt` files.

**Running Executables:**

After a successful build, the executable files will be located in the build directory, often within a `Debug` or `Release` subfolder, depending on your build configuration.

Example of running an executable (e.g., the `ImageViewer` from `Aufgabenblatt06`):
```bash
# On Linux/macOS
./Aufgabenblatt06/00_student_setup/code/build/ImageViewer

# On Windows
.\Aufgabenblatt06\00_student_setup\code\build\ImageViewer.exe
```
*(The exact path and executable name will vary depending on the specific assignment and your operating system.)*

## Development Conventions

*   **Language Standard:** Modern C++ features are used, adhering to C++14 and C++17 standards.
*   **Memory Management:** `std::shared_ptr` is commonly employed for robust object management within scene graphs and object hierarchies, promoting safer memory handling.
*   **Code Structure:** Code is logically organized into modules and uses namespaces (e.g., `cg`) to maintain clarity and prevent naming conflicts.
*   **Documentation:** Code includes comments (some in German) explaining complex logic and `TODO` markers indicating areas for student implementation.

---
*This `README.md` was generated by a Gemini-powered AI agent to provide a comprehensive overview of the project.*