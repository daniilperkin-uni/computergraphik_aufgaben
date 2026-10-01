# Aufgabenblatt 07 - Image Filtering

Image filtering with convolution kernels: edge detection (Laplacian), 2D and separable
Gaussian blur, and the CLAMP_TO_EDGE / MIRROR / REPEAT border policies - on the CPU and,
for the separable Gaussian, as an OpenGL compute shader, driven by an ImGui GUI.

## Build and Run

```bash
# from the repository root
cmake -S Aufgabenblatt07/00_student_setup/code -B Aufgabenblatt07/00_student_setup/code/build
cmake --build Aufgabenblatt07/00_student_setup/code/build --parallel

cd Aufgabenblatt07/00_student_setup/code
./build/ImageViewer07                    # GUI, loads bilder/ginkgo.ppm on startup
./build/ImageViewer07 source.ppm out.ppm # console mode, asks for filter on stdin
```

Start it from `code/` or `code/build` - the viewer resolves `bilder/`, `shader/` and
`../shader/` relative to the working directory. GLFW is fetched by CMake on the first
configure; glad, glowl and ImGui 1.65 are vendored in the sheet, as the exercise's
program skeleton ships them.

Console mode filter selection: 1 = edge detection, 2 = 2D Gaussian, 3 = separable
Gaussian, followed by the border-policy, kernel-extent and sigma prompts.

## Notes

- Target: `ImageViewer07`.
- The GPU path is only initialized on an OpenGL 4.3+ context (compute shaders); the CPU
  filters work regardless.
