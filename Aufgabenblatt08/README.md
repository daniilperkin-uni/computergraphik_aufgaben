# Aufgabenblatt 08 - Instanced Rendering & Imposters

This assignment introduces advanced rendering techniques to efficiently render a large scene with many objects, specifically a forest of trees.

## Core Concepts

*   **Instanced Rendering:** Instead of issuing a draw call for each tree, we use `glDrawElementsInstanced` to render multiple copies of the same geometry (tree or sprite) in a single draw call. Per-instance data (like model matrices) is stored in Uniform Buffers or Vertex Buffers.
*   **Imposters (Billboards):** To further optimize rendering, distant complex 3D objects (trees) are replaced with simple 2D textured quads (sprites) that always face the camera or are aligned in a specific way. This significantly reduces the polygon count.
*   **Direct State Access (DSA):** Modern OpenGL (4.5+) allows modifying OpenGL objects (buffers, textures, VAOs) without binding them to the context first. This assignment practices using DSA functions like `glNamedBufferData`, `glVertexArrayAttribBinding`, `glCreateTextures`, etc.

## Implemented Features

### 1. Scene Setup & Camera
*   **Camera Control:** Implemented View and Projection matrix calculation in `UpdateScene` using `glm::lookAt` and `glm::perspective`.
*   **Input Handling:** Support for keyboard and joystick input to move the camera through the scene.

### 2. Geometry & Buffers (DSA)
*   **Render Batch Creation:** Implemented `CreateRenderBatch` to set up Vertex Array Objects (VAOs), Vertex Buffers (VBOs), and Index Buffers (IBOs) using exclusively DSA functions (`glCreateBuffers`, `glNamedBufferData`, `glVertexArrayElementBuffer`, etc.).
*   **Tree Geometry:** Defined the vertices and indices for a simplified 3D tree model.
*   **Imposter Geometry:** Defined the geometry for a 2D quad (imposter) to represent the tree.

### 3. Textures & Shaders
*   **Texture Creation:** Implemented `CreateSpriteTexture` to load an image and create an OpenGL texture object using DSA (`glCreateTextures`, `glTextureStorage2D`, `glTextureSubImage2D`).
*   **Texture Parameters:** Configured texture wrapping (Mirrored Repeat) and filtering (Linear Mipmap Linear) parameters.
*   **Shaders:** Utilization of specific shaders for the ground, 3D trees, and tree imposters.

## Usage

1.  **Build:**
    ```bash
    mkdir build
    cd build
    cmake ..
    cmake --build .
    ```
2.  **Run:**
    Execute the generated executable (e.g., `Geometrie` or similar name depending on CMake configuration).
    ```bash
    ./Geometrie
    ```

3.  **Controls:**
    *   **W/A/S/D:** Move camera (Forward, Left, Backward, Right).
    *   **Shift/Space:** Move camera Up/Down.
    *   **Arrow Keys:** Rotate camera (Yaw/Pitch).
    *   **Joystick:** Supported for movement and rotation.
    *   **R:** Reload shaders.

## Key Files
*   `src/main.cpp`: Main application loop, scene setup, and rendering logic.
*   `src/core.cpp`: OpenGL initialization and helper functions.
*   `shaders/`: GLSL shader files for different rendering passes.
