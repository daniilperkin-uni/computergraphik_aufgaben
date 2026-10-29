# Aufgabenblatt 03 - Introduction to Ray Tracing

Ray generation and ray/object intersection. The program renders a fixed 600 x 600 scene
to `result.ppm` and compares it against the reference image that matches the active test
flag.

## Build, Run and Test

```bash
# from the repository root
cmake -S Aufgabenblatt03/code -B Aufgabenblatt03/code/build
cmake --build Aufgabenblatt03/code/build --parallel

cd Aufgabenblatt03/code/build
./Raytracer03                # writes result.ppm next to the binary

ctest -C Release --output-on-failure   # renders and compares against the reference image
```

`TEST_RAY_GENERATION` / `TEST_SPHERE_INTERSECT` at the top of `main.cpp` select which
reference image is compared; the committed state checks the sphere-intersection result.
The program exits non-zero when more than 0.1% of the pixels differ, and ctest runs the
same comparison in CI. Two further tests cover the PPM comparison diagnostic
(`ComparePPMTests`) and `Vec3::normalize()` (`Vec3Tests`).

## Notes

- Target: `Raytracer03`; no external dependencies.
- The reference images are found through the `REFERENCE_DIR` compile definition (the
  absolute source directory), so the comparison no longer depends on the working
  directory.
