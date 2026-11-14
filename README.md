# Computer Graphics with Blender Objects

A University of Nottingham Year 3 graphics project combining a Blender-exported OBJ scene with C++, OpenGL, and OpenCV texture loading.

![Project scene](https://github.com/jaesungkr/Computer-Graphics-with-blender-object/assets/82886701/b03b05d4-9d4c-4477-ab3f-0dfe5ae403ca)

## View the project

- [Project report](CG_Project/CG_2020_20121762_REPORT.pdf)
- [Recorded demonstration](CG_Project/CG_2020_20121762_VIDEO.mp4)
- [Rendering source](CG_Project/Assignment1_JaesungPark/Source.cpp)
- [OBJ model and materials](CG_Project/Assignment1_JaesungPark/obj_files/)
- [Textures](CG_Project/Assignment1_JaesungPark/texture/)

## What it demonstrates

- Loading OBJ vertices, texture coordinates, normals, and faces.
- Rendering a textured scene with lighting.
- Animating scene elements.
- Moving a camera through the scene with keyboard input.

## Controls

| Key | Action |
|---|---|
| Left / Right | Turn the camera |
| Up / Down | Move forward / backward |
| Page Up / Page Down | Move the camera vertically |
| Escape | Exit |

## Build context

Open [the Visual Studio solution](CG_Project/Assignment1_JaesungPark.sln) to inspect the original project. It targets Win32 with the Visual Studio `v120` toolset and references OpenCV 3.0 libraries, GLEW, and GLUT/freeglut.

Before rebuilding, update the OpenCV include/library paths in the project and the OBJ/texture paths in `Source.cpp` for your own checkout. The original paths are machine-specific. A modern, portable build configuration is not included.

## Scope

This repository preserves the original coursework source, assets, report, and recording. The report and video are the simplest way to review the result without recreating the original Windows build environment.
