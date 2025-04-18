# ArtSpace

An interactive 3D art gallery with puzzle elements. Users can navigate a virtual exhibition space, manipulate artworks, and solve puzzles by rotating frames in specific patterns.

## Project Description

ArtSpace is a C++ application that creates a 3D navigable art gallery with proper lighting and interactive elements. Users move around in a first-person perspective, interact with artwork on the walls, and solve puzzles by rotating the frames in specific sequences or orientations.

## Features

- First-person navigation (WASD + mouse look)
- 3D room with textured walls, floor, and ceiling
- Realistic lighting system with multiple light sources
- Interactive artwork frames that can be rotated
- Collision detection with walls and objects
- Puzzle-solving mechanics through artwork manipulation
- Win conditions when puzzles are solved correctly

## Prerequisites

- Visual Studio 2019 or later
- Windows 10 or later

## Installation

1. Clone the repository:
   ```
   git clone https://github.com/alaamer12/ArtSpace.git
   ```

2. Open the solution file `ArtSpace.sln` in Visual Studio.

3. Build the solution (press F7 or use Build > Build Solution).

4. Run the application (press F5 or use Debug > Start Debugging).

## Setup Notes for Collaborators

The project is configured to use the included FreeGLUT libraries in the `Dependencies` folder. When you clone the repository, Visual Studio should automatically recognize these paths as they are set up using relative paths in the project file.

If you encounter any issues with missing libraries:

1. Right-click on the ArtSpace project in Solution Explorer
2. Select "Properties"
3. Navigate to:
   - VC++ Directories → Include Directories
     - Should include `$(SolutionDir)Dependencies\freeglut\include`
   - VC++ Directories → Library Directories
     - Should include `$(SolutionDir)Dependencies\freeglut\lib\$(Platform)`
4. Under Linker → Input → Additional Dependencies
   - Should include `freeglut.lib`

The project also includes a post-build event that copies the necessary DLL file to the output directory:
```
copy "$(SolutionDir)Dependencies\freeglut\bin\$(Platform)\freeglut.dll" "$(OutDir)"
```

If this event fails, you may need to manually copy `freeglut.dll` from the appropriate folder in `Dependencies\freeglut\bin\[x86 or x64]` to your output directory.

## Controls

- **W/A/S/D**: Move forward/left/backward/right
- **Mouse**: Look around
- **E**: Interact with artwork (when close enough)
- **← →**: Rotate selected artwork
- **Enter**: Confirm artwork position
- **P**: Pause
- **ESC**: Exit application

## Development Roadmap

1. **Core Navigation & Room** - Complete ✓
2. **Artwork Implementation**
   - Create frames and artwork display system
   - Load textures for artwork
   - Position frames on walls
3. **Interaction System**
   - Ray casting for selection
   - Rotation mechanics for frames
   - Visual feedback for selected items
4. **Collision System**
   - Wall and object collision
   - Proper physics for movement
5. **Puzzle Mechanics**
   - Define correct orientations
   - Implement win conditions
   - Add feedback and rewards

## Acknowledgments

- FreeGLUT
- OpenGL 