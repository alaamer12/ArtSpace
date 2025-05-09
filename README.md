# ArtSpace

An interactive 3D art gallery with puzzle elements. Users can navigate a virtual exhibition space, manipulate artworks, and solve puzzles by properly aligning art frames to their correct orientation.

![Art Gallery Screenshot](https://raw.githubusercontent.com/alaamer12/ArtSpace/main/assets/pictures/screenshots/screen1.PNG)

## Project Description

ArtSpace is a C++ application that creates a 3D navigable art gallery with proper lighting and interactive elements. Users move around in a first-person perspective, interact with artwork on the walls, and solve puzzles by rotating the frames to their correct orientations. The game features realistic artwork display with customizable frames, rotation mechanics, and proximity-based interaction.

Each artwork is positioned strategically throughout the gallery, and the user must correctly align all artworks to a vertical orientation (0 degrees) to win the game. This combines exploration with puzzle-solving in an immersive 3D environment.

![Artwork Interaction](https://raw.githubusercontent.com/alaamer12/ArtSpace/main/assets/pictures/screenshots/screen2.PNG)

## Features

- First-person navigation with WASD movement and mouse look camera
- 3D gallery room with textured walls, floor, and ceiling
- Custom artwork loading system with separate image and frame textures
- Interactive artworks with rotation capabilities
- Proximity detection system that identifies the closest artwork to the player
- Artwork manipulation: rotation, frame stretching, and image stretching
- Custom artwork configurations with randomized initial rotations
- Win condition system that detects when all artworks are properly aligned
- Collision detection to prevent walking through walls
- Detailed console feedback and in-game status information
- Customizable artwork appearance with independent frame and image stretching

## Technology Stack

- C++ for core game logic and mechanics
- OpenGL for 3D rendering and texture mapping
- FreeGLUT for window management and user input
- Custom-built artwork rendering system
- Singleton pattern for key systems (input, game management, etc.)
- Vector mathematics for camera movement and rotation

## Project Components

For a detailed breakdown of the core modules and architecture, see the [COMPONENTS](https://raw.githubusercontent.com/alaamer12/ArtSpace/main/COMPONENTS.md) file.

## Developer Guide

For detailed information about the codebase architecture, component usage, and implementation best practices, see the [GUIDE.md](https://raw.githubusercontent.com/alaamer12/ArtSpace/main/GUIDE.md) file. The guide includes:

- Architecture overview and application flow
- Detailed component documentation
- Best practices for extending the application
- Implementation examples for common tasks
- File structure and organization

## Prerequisites

- Visual Studio 2022 or later
- Windows 10 or later
- OpenGL and FreeGLUT libraries (included in the dependencies folder)

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

**Note:** To ensure SFML sound works correctly, make sure to select the **Release** configuration (not Debug) from the Solution Configuration dropdown at the top of Visual Studio before building and running. SFML audio may not function properly in Debug mode due to library/DLL differences.

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
- **F**: Toggle wireframe mode
- **+/-**: Adjust mouse sensitivity
- **ESC**: Exit application

### Artwork Manipulation
- **k**: Rotate closest artwork counterclockwise by 15 degrees
- **l**: Rotate closest artwork clockwise by 15 degrees
- **x/X**: Increase/decrease image width of closest artwork
- **y/Y**: Increase/decrease image height of closest artwork
- **f/F**: Increase/decrease frame width of closest artwork
- **g/G**: Increase/decrease frame height of closest artwork
- **r/R**: Reset image/frame stretching of closest artwork
- **p**: Toggle detailed proximity debugging information

## Gameplay

The objective of ArtSpace is to navigate the gallery and rotate all artworks to their proper vertical orientation (0 degrees). When you approach an artwork, the console will display which artwork is closest to you. You must be within 25 units of an artwork to interact with it.

Use the 'k' and 'l' keys to rotate the closest artwork counterclockwise or clockwise in 15-degree increments. When all artworks are properly aligned at 0 degrees, you win the game!

Each artwork starts with a random rotation (in 15-degree increments), so you'll need to carefully navigate the gallery and align each piece. The game provides feedback on your current distance from artworks and their current rotation angles.

## Development Status

ArtSpace has successfully implemented all core features:

- ✅ 3D Navigation & Camera System
- ✅ Gallery Environment with Textured Walls
- ✅ Artwork Loading and Display
- ✅ Proximity Detection for Artworks
- ✅ Artwork Rotation Mechanics
- ✅ Win Condition Logic
- ✅ Customizable Artwork Appearance
- ✅ Collision Detection

## Future Enhancements

- Additional gallery rooms and layouts
- More diverse artwork collections
- Advanced lighting effects
- Puzzle complexity with specific rotation patterns
- Audio feedback and ambient music
- Interactive tutorial system
- Saving/loading game state

## Acknowledgments

- [FreeGLUT](https://www.transmissionzero.co.uk/software/freeglut-devel/) for window management and OpenGL context creation
- [OpenGL](https://www.opengl.org/) for rendering 2D and 3D graphics
- [SFML](https://www.sfml-dev.org/) for sound and multimedia handling
- All art assets used with appropriate permissions
