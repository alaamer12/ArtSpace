# <span style="color: #4a86e8;">ArtSpace Development Guide</span>

<style>
h2 {
  color: #4a86e8;
  border-bottom: 1px solid #e0e0e0;
  padding-bottom: 5px;
}
h3 {
  color: #6aa84f;
}
code {
  background-color: #f5f5f5;
  padding: 2px 4px;
  border-radius: 3px;
}
pre code {
  display: block;
  padding: 10px;
  overflow-x: auto;
  line-height: 1.4;
}
.feature-list li {
  margin-bottom: 8px;
}
</style>

## Table of Contents
- [Architecture Overview](#architecture-overview)
- [Core Components](#core-components)
- [Application Flow](#application-flow)
- [File Structure](#file-structure)
- [Component Reference](#component-reference)
  - [Configuration](#configuration)
  - [Input System](#input-system)
  - [Navigation](#navigation)
  - [Screen Management](#screen-management)
  - [UI Components](#ui-components)
  - [Camera](#camera)
- [Best Practices](#best-practices)
- [Implementation Examples](#implementation-examples)

## Architecture Overview

<details>
<summary><b>Click to expand architecture details</b></summary>

ArtSpace is structured as a modular OpenGL-based application with clear separation of concerns. The architecture follows these principles:

<ul class="feature-list">
  <li><strong>Model-View-Controller pattern</strong> - Separating data, presentation, and control logic</li>
  <li><strong>Singleton pattern</strong> for global managers (Config, InputSystem)</li>
  <li><strong>Component-based UI system</strong> with hierarchical elements</li>
  <li><strong>State-based screen navigation</strong> for different application screens</li>
</ul>
</details>

## Core Components

The application is built using these core components:

<ol class="feature-list">
  <li><strong>Game Manager</strong> - Central control point for the entire application</li>
  <li><strong>Config</strong> - Application settings and configuration management</li>
  <li><strong>Input System</strong> - Keyboard and mouse input handling</li>
  <li><strong>Navigator</strong> - Screen navigation and management</li>
  <li><strong>Screen Managers</strong> - Individual screen implementations</li>
  <li><strong>UI Components</strong> - Interactive interface elements</li>
  <li><strong>Camera</strong> - 3D view management</li>
</ol>

## Application Flow

<details>
<summary><b>Click to expand application flow details</b></summary>

The typical flow of the application follows these steps:

<div style="padding-left: 15px; border-left: 3px solid #4a86e8;">
  <h4>1. Initialization (main.cpp)</h4>
  <ul>
    <li>Initialize OpenGL/GLUT</li>
    <li>Create Config instance and load settings</li>
    <li>Create GameManager instance</li>
    <li>Register input callbacks</li>
  </ul>

  <h4>2. Game Loop (GLUT main loop)</h4>
  <ul>
    <li>Process input</li>
    <li>Update game state</li>
    <li>Render current screen</li>
  </ul>

  <h4>3. Screen Flow</h4>
  <ul>
    <li>Start on the main menu</li>
    <li>Navigate between screens based on user input</li>
    <li>Handle game state changes through Navigator</li>
  </ul>
</div>
</details>

## File Structure

<details>
<summary><b>Click to expand file structure details</b></summary>

```
ArtSpace/
├── main.cpp                # Application entry point
├── game_manager.h/.cpp     # Central game management
├── config.h/.cpp           # Configuration system
├── input.h/.cpp            # Input handling
├── navigator.h/.cpp        # Screen navigation
├── screen_manager.h/.cpp   # Base screen management
├── screens/                # Screen implementations
│   ├── screens.h           # Screen enum definitions
│   ├── main_screen.h       # Main menu screen
│   ├── gameplay_screen.h   # Main gameplay screen
│   └── ...                 # Other screen implementations
├── camera.h/.cpp           # Camera management
└── utility.h/.cpp          # UI components and utilities
```
</details>

## Component Reference

### Configuration

<details>
<summary><b>Configuration system details</b></summary>

The `Config` class is a singleton that manages application settings.

**Key Features:**
<ul class="feature-list">
  <li>Load and save configuration to/from files</li>
  <li>Access to display settings (resolution, fullscreen)</li>
  <li>Camera settings (sensitivity, movement speed)</li>
  <li>Game-specific settings (rotation step, asset paths)</li>
</ul>

**Usage Example:**
```cpp
// Get the singleton instance
Config& config = Config::getInstance();

// Access settings
int width = config.getScreenWidth();
float sensitivity = config.getMouseSensitivity();

// Modify settings
config.setFullscreen(true);
config.setMoveSpeed(5.0f);

// Save/load configuration
config.saveToFile("config.ini");
config.loadFromFile("config.ini");
```
</details>

### Input System

<details>
<summary><b>Input system details</b></summary>

The `InputSystem` singleton handles all user input through GLUT callbacks.

**Key Features:**
<ul class="feature-list">
  <li>Keyboard input handling</li>
  <li>Mouse position and button tracking</li>
  <li>Mouse movement delta calculation</li>
  <li>Integration with GLUT callback system</li>
</ul>

**Usage Example:**
```cpp
// Get the singleton instance
InputSystem* input = InputSystem::getInstance();

// Register with GLUT
input->registerCallbacks();

// Check input states
if (input->isKeyPressed('w')) {
    // Move forward
}

float deltaX, deltaY;
input->getMouseDelta(deltaX, deltaY);
```
</details>

### Navigation

<details>
<summary><b>Navigation system details</b></summary>

The `Navigator` class manages transitions between different screens.

**Key Features:**
<ul class="feature-list">
  <li>Register and manage screen managers</li>
  <li>Navigate between screens</li>
  <li>Route input events to the current screen</li>
  <li>Handle screen lifecycle (show/hide)</li>
</ul>

**Usage Example:**
```cpp
// Create and set up navigator
Navigator navigator;

// Register screens
StartScreenManager* startScreen = new StartScreenManager();
startScreen->setupScreen(&navigator);
navigator.registerScreen(startScreen);

// Navigate to a screen
navigator.navigateTo(Screen::StartScreen);

// Handle input events
navigator.handleKeyEvent(key);
```
</details>

### Screen Management

<details>
<summary><b>Screen management details</b></summary>

The `ScreenManager` system provides a base interface for creating screens.

**Key Features:**
<ul class="feature-list">
  <li>Base class for all screen implementations</li>
  <li>Component management (add, remove, update)</li>
  <li>Input event routing to components</li>
  <li>Screen lifecycle management (show, hide, update, render)</li>
</ul>

**Usage Example:**
```cpp
// Create a custom screen manager
class MainScreenManager : public ScreenManager {
public:
    MainScreenManager() : ScreenManager(Screen::StartScreen) {}
    
    void setupScreen(Navigator* nav) {
        // Create components
        Button* startButton = new Button("Start Game");
        startButton->setOnClick([nav]() {
            nav->navigateTo(Screen::Level1Screen);
        });
        addComponent(startButton);
    }
};
```
</details>

### UI Components

<details>
<summary><b>UI components details</b></summary>

The UI system uses a component-based architecture with various elements.

**Key Components:**
<ul class="feature-list">
  <li><code>UIComponent</code> - Base class for all UI elements</li>
  <li><code>Button</code> - Interactive button with click handling</li>
  <li><code>TextBox</code> - Text display and input field</li>
  <li><code>Image</code> - Image display with texture support</li>
  <li><code>BorderBox</code> - Container with border and background</li>
  <li><code>Timer</code> - Time display and management</li>
</ul>

**Usage Example:**
```cpp
// Create a button
Button* button = new Button("Click Me");
button->setPosition(100, 100);
button->setSize(150, 50);
button->setOnClick([]() {
    // Button click handler
    Logger::getInstance().logInfo("Button clicked");
});

// Add to screen manager
screenManager->addComponent(button);
```
</details>

### Camera

<details>
<summary><b>Camera system details</b></summary>

The camera system handles the 3D viewpoint and controls.

**Key Features:**
<ul class="feature-list">
  <li>First-person navigation</li>
  <li>Mouse look controls</li>
  <li>WASD movement</li>
  <li>Human camera with head bobbing and smooth movement</li>
</ul>

**Usage Example:**
```cpp
// Create a human camera
HumanCamera camera;

// Set properties
camera.setPosition(0.0f, 1.7f, 0.0f);
camera.setMouseSensitivity(0.5f);

// In the update loop
camera.update(deltaTime);
camera.applyTransformation();
```
</details>

## Best Practices

<details>
<summary><b>Game Structure Best Practices</b></summary>

### Game Structure
1. **Define configuration in main.cpp**
   ```cpp
   Config::getInstance().loadFromFile("config.ini");
   ```

2. **Create GameManager for core logic**
   ```cpp
   GameManager gameManager;
   gameManager.initialize();
   ```

3. **Keep main.cpp focused on OpenGL rendering**
   ```cpp
   void display() {
       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       gameManager.render();
       glutSwapBuffers();
   }
   ```

4. **Include all needed components in GameManager**
   ```cpp
   // game_manager.h
   #include "config.h"
   #include "input.h"
   #include "navigator.h"
   #include "camera.h"
   ```
</details>

<details>
<summary><b>Screen Management Best Practices</b></summary>

### Screen Management
1. **Define screens in screens.h using enum**
   ```cpp
   enum class Screen {
       StartScreen,
       GameScreen,
       SettingsScreen
   };
   ```

2. **Create individual screen files for complex screens**
   ```cpp
   // main_screen.h
   class MainScreenManager : public ScreenManager {
       // Implementation
   };
   ```

3. **Use ScreenManager as interface for all screens**
   ```cpp
   class CustomScreen : public ScreenManager {
       void setupScreen(Navigator* nav) override;
       void resizeScreen(int width, int height) override;
   };
   ```
</details>

<details>
<summary><b>Input Handling Best Practices</b></summary>

### Input Handling
1. **Always log input events using Logger**
   ```cpp
   void update(float deltaTime) {
       InputSystem* input = InputSystem::getInstance();
       Logger& logger = Logger::getInstance();
       
       if (input->isKeyPressed('p')) {
           logger.log(Logger::DEBUG, "Pause key pressed");
           // Pause game
       }
   }
   ```

2. **Log UI interactions with appropriate level**
   ```cpp
   button->setOnClick([this]() {
       Logger::getInstance().log(Logger::INFO, "Button clicked");
       // Handle button click
   });
   ```

3. **Use debug logging for input diagnostics**
   ```cpp
   void handleMouseMove(int x, int y) {
       Logger::getInstance().log(Logger::DEBUG, 
           "Mouse moved to (" + std::to_string(x) + "," + std::to_string(y) + ")");
       // Handle mouse movement
   }
   ```
</details>

## Implementation Examples

<details>
<summary><b>Creating a New Screen</b></summary>

```cpp
// Define the screen enum
enum class Screen {
    StartScreen,
    GameScreen,
    NewCustomScreen  // Add your new screen
};

// Create the screen manager
class CustomScreenManager : public ScreenManager {
public:
    CustomScreenManager() : ScreenManager(Screen::NewCustomScreen) {}
    
    void setupScreen(Navigator* nav) {
        // Create title
        TextBox* title = new TextBox("Custom Screen", "");
        title->setPosition(100, 50);
        title->setSize(200, 40);
        addComponent(title);
        
        // Create back button
        Button* backButton = new Button("Back");
        backButton->setPosition(100, 100);
        backButton->setSize(100, 40);
        backButton->setOnClick([nav]() {
            nav->navigateTo(Screen::StartScreen);
        });
        addComponent(backButton);
    }
};

// Register with navigator
navigator.registerScreen(new CustomScreenManager());
```
</details>

<details>
<summary><b>Game Manager Implementation</b></summary>

```cpp
// game_manager.h
#pragma once
#include "config.h"
#include "input.h"
#include "navigator.h"
#include "camera.h"
#include "screen_manager.h"
#include "screens.h"

class GameManager {
private:
    Navigator navigator;
    HumanCamera camera;
    bool isPaused;
    
public:
    GameManager();
    ~GameManager();
    
    void initialize();
    void update(float deltaTime);
    void render();
    
    void handleKeyEvent(unsigned char key);
    void handleMouseEvent(int button, int state, int x, int y);
    void handleMouseMove(int x, int y);
    void handleResize(int width, int height);
    
    bool isPaused() const { return isPaused; }
    void setPaused(bool paused) { isPaused = paused; }
}; 