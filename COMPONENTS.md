# ArtSpace Components

This document outlines the main components of the ArtSpace project, providing a structural overview of how the application should be organized as it evolves beyond a single-file implementation.

## Core Components

- **Camera** (`camera.h`, `camera.cpp`): Handles the first-person camera system.
- **Input System** (`input.h`, `input.cpp`): Manages keyboard and mouse input.
- **Artwork** (`artwork.h`, `artwork.cpp`): Defines artwork objects that can be rotated and interacted with.
- **Artwork Manager** (`artwork_manager.h`, `artwork_manager.cpp`): Manages the collection of artworks in the gallery.
- **Room** (`room.h`, `room.cpp`): Defines the 3D room that contains the artworks.
- **Texture Manager** (`texture_manager.h`, `texture_manager.cpp`): Handles loading and managing textures.
- **Collision System** (`collision.h`, `collision.cpp`): Handles collision detection between the player and environment.
- **Puzzle System** (`puzzle.h`, `puzzle.cpp`): Manages puzzle logic and win conditions.
- **Game State** (`game_state.h`, `game_state.cpp`): Manages the overall game state.
- **Main Application** (`main.cpp`): The main application class that ties everything together.
- **Configuration** (`config.h`, `config.cpp`): Centralized configuration settings.

### Centralized configuration settings.

```cpp
class Config {
private:
    static Config* instance;
    
    // Display settings
    int screenWidth;
    int screenHeight;
    bool fullscreen;
    
    // Camera settings
    float mouseSensitivity;
    float moveSpeed;
    
    // Gameplay settings
    float interactionDistance;
    float rotationStep;
    
    // Constructor is private (singleton)
    Config();
    
public:
    static Config* getInstance();
    
    // Load/save
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename);
    
    // Getters/Setters (for all properties)
    int getScreenWidth() const;
    void setScreenWidth(int width);
    // ... other getters/setters ...
};
```

### Implementation Strategy

1. **Phase 1**: Start by implementing the core classes (Camera, Room, Artwork)
2. **Phase 2**: Add texture loading and basic interaction
3. **Phase 3**: Implement collision detection and advanced interaction
4. **Phase 4**: Add puzzle mechanics and win conditions
5. **Phase 5**: Refine user interface and add polish