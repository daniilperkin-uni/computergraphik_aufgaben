# Aufgabenblatt 02 - Image Processing Fundamentals

The image class and color-space conversions: grayscale, black/white, an RGB -> HSV -> RGB
round trip, and a color-key effect - each selected from the console.

## Build and Run

```bash
# from the repository root
cmake -S Aufgabenblatt02/aufgaben_blatt_02_/code -B Aufgabenblatt02/aufgaben_blatt_02_/code/build
cmake --build Aufgabenblatt02/aufgaben_blatt_02_/code/build --parallel

./Aufgabenblatt02/aufgaben_blatt_02_/code/build/ColorSpaces <source> <target>
```

The program asks for the exercise number on stdin: 1 = grayscale, 2 = black/white,
3 = RGB -> HSV -> RGB, 4 = color-key effect in HSV.

`images/` holds the sample data: `ginkgo.ppm`, `lena.ppm` and `seattle.pgm` are inputs
(the image I/O reads PBM/PGM/PPM only, so the `.jpg` is just the original photo, see
`images/image_source.txt`), while `ginkgo_gray.pgm`, `ginkgo_black_and_white.pgm` and
`ginkgo_modified.ppm` are kept results of exercises 1, 2 and 4.

## Notes

- Target: `ColorSpaces`; no external dependencies.
