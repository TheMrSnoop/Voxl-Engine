# Overview
These are all Shaders written in C. They are used by OpenGL when rendering the FBO. They are seperated by `.frag` and `.vert`.

- Basic-Pixelation: Simply renders the world at a slower resolution, then upscales it with zero interpolation
- Default: The basics required for basic world rendering
- Outline: Adds a colored outline around every Voxel/Object in the world
- Pixelation: Renders the world at a lower resolution, then upscales it with zero interpolation AND adds a colored outline around every object
