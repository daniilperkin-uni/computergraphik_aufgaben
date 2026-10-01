# Aufgabenblatt 09 — Scene shading

Loads a glTF scene, builds an index-based transform hierarchy, uploads non-interleaved
vertex data and shades the result with a Phong lighting model plus normal mapping.

## Build and run

```bash
# from the repository root
cmake -S Aufgabenblatt09/code -B Aufgabenblatt09/code/build
cmake --build Aufgabenblatt09/code/build --parallel

cd Aufgabenblatt09/code/build
./SceneShading /path/to/scene.glb
```

`scene.glb` is **not** part of this repository. Either pass it as the first command line
argument or place it where the default path expects it:

```
Aufgabenblatt09/
├── code/
└── scene_file/
    └── resources/
        └── scene.glb
```

The program prints the path it tried and aborts when the file is missing. Start it from
`code/build`: the shaders are loaded from `../shaders/`. GLFW, GLM and tinygltf are
fetched by CMake on the first configure, so that step needs network access.

## Notes

- Target: `SceneShading`.
- The implementation is described in German: `solutions.md` covers the transform
  hierarchy and the vertex-data upload, `solutions-shading.md` walks through the shading
  tasks 2.1–2.5 (normal matrix, view direction, light loop, TBN, normal mapping).
