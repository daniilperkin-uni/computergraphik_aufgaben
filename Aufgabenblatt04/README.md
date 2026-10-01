# Aufgabenblatt 04 - Point Lights, Shadows and Reflections

Extends the Blatt 03 ray tracer with a Phong lighting model (the ambient term is added
once, diffuse and specular contributions per light), shadow rays, distance attenuation
over 16 point lights, recursive specular reflections (depth limit 5) and an
OpenMP-parallel render loop.

## Build and Run

```bash
# from the repository root
cmake -S Aufgabenblatt04/code -B Aufgabenblatt04/code/build
cmake --build Aufgabenblatt04/code/build --parallel

cd Aufgabenblatt04/code/build
./Raytracer04     # writes result.ppm into the working directory
```

`referenz.png` and `referenz-hd.png` are the exercise's reference images; compare the
rendered `result.ppm` against them by eye (this sheet has no automated check).

## Notes

- Target: `Raytracer04`; no external dependencies. OpenMP is used when the compiler
  provides it, so the render is multi-threaded in a local release build and in CI.
