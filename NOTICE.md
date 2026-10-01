# Third-party code, sample assets and course material

Personal archive of exercise submissions for a university "Computergraphik" course. The
exercise sheets and handouts that ship as PDFs are course material and remain the
property of the course; they are kept here as context for the submitted work.

## Vendored source code

Checked into the repository. Paths are relative to the sheet's code directory
(`AufgabenblattXX/00_student_setup/code` for sheets 01-08, `Aufgabenblatt09/code`):

| Library | Path | License |
| --- | --- | --- |
| glad (OpenGL loader) | `06 libs/glad/`, `07 glad/`, `08 libs/glad/`, `09 libs/glad/` | MIT |
| glowl (OpenGL object wrappers) | `06 libs/glowl/`, `07 glowl/` | MIT |
| Dear ImGui 1.65 | `07 imgui/` | MIT |
| lodepng | `08 libs/lodepng/` | zlib |
| tinygltf (translation unit) | `09 libs/tinygltf/` | MIT |

The vendored copies do not include separate LICENSE files; the upstream notices are kept
where the projects embed them in their sources.

## Fetched at configure time

CMake `FetchContent` downloads these during configure (pinned by hash or commit, not
stored in the repository): GLFW 3.4 and GLM 1.0.1 (all OpenGL sheets), Dear ImGui 1.86
(Aufgabenblatt06) and the tinygltf sources (Aufgabenblatt09).

## Sample images

The images under `Aufgabenblatt02/aufgaben_blatt_02_/images/` and
`Aufgabenblatt07/00_student_setup/code/bilder/` ship with the course material (see
`images/image_source.txt` for the ginkgo photo's source) and are kept for reference.
