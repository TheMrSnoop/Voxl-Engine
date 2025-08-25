# Engine Source Code

The following is the main source code for core features of the Engine, like Block Spawning, Lighting, Textures, and more.

# Short Summary of Every C++ file here.

- `Block.cpp` Contains the code for basic geometry rendering (just cubes), a database for user created blocks, and the logic for block spawning and such.
- `Camera.cpp` Contains the code for Engine Input/Camera Controls, and the actual Camera Transform, and Screen Rendering.
- `DeltaTime.cpp` *Depricated*
- `EBO.cpp` Edge Buffer Object
- `FBO.cpp` Frame Buffer Object
- `MenuBar.cpp` Renders the top MenuBar for the Engine Window
- `Texture.cpp` Defines how textures are handled with blocks, and overall inside the engine
- `Tree.cpp` Defines the logic for Tree Spawning, and lists the relative cordinates for every tree inside a project
- `VAO.cpp` Vertex Array Object
- `VBO.cpp` Vertext Buffer Object
- `VoxlEngine.cpp` A developer friendly class containing engine related functions, like returning engine FPS and DeltaTime
- `shaderClass.cpp` Defines the logic on how shaders are rendered
- `stb.cpp` *Depricated*
- `UserInterface.cpp` Defines the logic for using ImGUI for spawning Text Blocks, with specific properties, and layering them to specific Canvas objects. 
