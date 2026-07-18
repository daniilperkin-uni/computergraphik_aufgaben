# Aufgabenblatt 03 - Introduction to Ray Tracing

This assignment introduces the core principles of ray tracing, a powerful technique for rendering realistic images. Key topics include:

*   **Ray Generation:** How to generate rays from a camera through pixels into a 3D scene.
*   **Scene Object Intersection:** Implementing intersection tests between rays and various geometric primitives (e.g., spheres, planes).
*   **Basic Rendering Pipeline:** Tracing rays, determining hits, and calculating basic colors based on intersections.

## Build and Run

To build and run the solutions for "Aufgabenblatt 03", follow the general build instructions in the main `README.md` from the project root. You can navigate into the `Aufgabenblatt03/code/` directory to build it specifically.

**Example Build (from project root):**
```bash
# From the project root directory
mkdir build_ab03
cd build_ab03
cmake ../Aufgabenblatt03/code/
cmake --build .
```
The main executable name will be determined by the `CMakeLists.txt` in that directory (likely `main`).
