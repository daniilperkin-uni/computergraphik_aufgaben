# Aufgabenblatt 07 - Image Filtering

This assignment focuses on implementing various image filtering techniques on the CPU.

## Implemented Features

### 1. Filter Application (`filterImage`)
*   Implemented the `filterImage` function in `ImageFilter.h`.
*   Iterates over the image and the kernel.
*   Uses `offsetImageCoordinates` to handle boundary conditions.
*   Accumulates weighted pixel values to produce the filtered image.

### 2. Edge Detection
*   **Kernel Construction:** Implemented `buildEdgeDetectionKernel` in `ImageFilter.cpp` using a standard 3x3 Laplacian kernel (8-neighbor).
*   **Edge Detection Filter:** Implemented `edgeDetection2D` in `ImageFilter.h` which applies the kernel and computes the absolute value of the result.

### 3. Gaussian Blur
*   **Kernel Calculation:** Implemented `setGaussianValues` in `ImageFilter.cpp` to compute and normalize Gaussian kernel values based on sigma.
*   **2D Gaussian:** Implemented `gaussian2D` in `ImageFilter.h` using `build2DGaussianKernel`.
*   **Separable Gaussian:** Implemented `seperatedGaussian2D` in `ImageFilter.h` using 1D horizontal and vertical kernels for optimization.

### 4. Border Policies
*   Implemented `MIRROR` policy: Mirrors coordinates at the image boundaries.
*   Implemented `REPEAT` policy: Wraps coordinates around the image (toroidal topology).

## Usage

To run the `ImageViewer`:
1.  Open the project in CLion.
2.  Select the `ImageViewer` run configuration.
3.  Build and Run.
4.  In the GUI, load an image (e.g., from `input/`) and select different filters to see the effects.

## Troubleshooting

*   **CMake Version:** The project requires a modern CMake version. If you encounter errors with `glfw`, ensure the `glfw/CMakeLists.txt` has been updated to remove deprecated policies (already applied).
