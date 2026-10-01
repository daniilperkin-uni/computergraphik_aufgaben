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

ctest --output-on-failure    # renders and compares against ../reference_*.ppm
```

`TEST_RAY_GENERATION` / `TEST_SPHERE_INTERSECT` at the top of `main.cpp` select which
reference image is compared; the committed state checks the sphere-intersection result.
The program exits non-zero when more than 0.1% of the pixels differ, and ctest runs the
same comparison in CI.

## Notes

- Target: `Raytracer03`; no external dependencies.
- Start it from `code/build` - the `../reference_*.ppm` paths are relative to the working
  directory.
