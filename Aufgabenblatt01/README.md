# Aufgabenblatt 01 - C++ Fundamentals

Four standalone exercises: simple functions, STL containers and algorithms, custom classes
with RAII, and declarations vs. definitions across translation units (the ODR problem).

## Build and Run

```bash
# from the repository root
cmake -S Aufgabenblatt01/aufgaben_blatt_01 -B Aufgabenblatt01/aufgaben_blatt_01/build
cmake --build Aufgabenblatt01/aufgaben_blatt_01/build --parallel
```

Each exercise is its own executable below the build directory
(`aufgabe_1/aufgabe_1`, `aufgabe_2/aufgabe_2`, ...), for example:

```bash
./Aufgabenblatt01/aufgaben_blatt_01/build/aufgabe_1/aufgabe_1
```

- `aufgabe_1` asks for a circle radius on stdin.
- `aufgabe_3` asks for the paths of two matrix files (samples in `code/`) and writes
  `matrix3.txt` into the working directory.

## Notes

- Targets: `aufgabe_1` ... `aufgabe_4`; no external dependencies.
- The root build (`cmake -S . -B build`) compiles this sheet as well.
