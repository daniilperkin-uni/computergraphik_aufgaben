# Aufgabenblatt 06 - Real-time Rasterization and Interactive Graphics

This assignment focuses on real-time computer graphics using a rasterization pipeline, integrating various external libraries to create interactive scenes. Key aspects include:

*   **Rasterization Pipeline:** Implementation of concepts like vertex processing, primitive assembly, rasterization, and fragment processing.
*   **Scene Graph Management:** Creation and manipulation of hierarchical scene structures with objects, cameras, and lights.
*   **Camera Controls:** Implementing different camera types and interactive camera movement.
*   **Lighting Models:** Application of various lighting techniques (e.g., ambient, point lights) to shade objects.
*   **Geometric Primitives:** Rendering of fundamental shapes such as triangles, cubes, and spheres.
*   **External Library Integration:** Extensive use of libraries like GLFW (windowing, input), GLAD (OpenGL loading), GLM (mathematics), and Dear ImGui (GUI).
*   **Complex Scene Generation:** Procedural generation of complex scenes, exemplified by the "DNA Double Helix" scene.

## Build and Run

To build and run the solutions for "Aufgabenblatt 06", follow the general build instructions in the main `README.md` from the project root. The main code for this assignment is located in `Aufgabenblatt06/00_student_setup/code/`.

**Example Build (from project root):**
```bash
# From the project root directory
mkdir build_ab06
cd build_ab06
cmake ../Aufgabenblatt06/00_student_setup/code/
cmake --build .
```

**Running the Executable:**

The primary executable for this assignment is typically named `ImageViewer`.
```bash
# On Linux/macOS (from build_ab06 directory)
./ImageViewer

# On Windows (from build_ab06 directory)
.\ImageViewer.exe
```
This will launch an interactive viewer displaying the scenes defined in the assignment.
