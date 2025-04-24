/**
 * @file input.h
 * @brief Input handling system using the singleton pattern
 * 
 * This module provides a centralized system for handling user input in an OpenGL/GLUT application.
 * The InputSystem class manages keyboard input (both standard and special keys) and mouse
 * interactions (clicks, movement, dragging, and rotation).
 * 
 * Main class:
 * - InputSystem: Singleton class that processes and stores input state
 * 
 * Usage:
 * 1. Get the singleton instance:
 *    InputSystem* input = InputSystem::getInstance();
 * 
 * 2. Register callbacks with GLUT:
 *    input->registerCallbacks();
 * 
 * 3. In your main loop, update the input system:
 *    input->update();
 * 
 * 4. Query input state:
 *    if (input->isKeyPressed('w')) { // handle 'w' key press  }
 *    if (input->isMouseButtonPressed(GLUT_LEFT_BUTTON)) { // handle left mouse button }
 *    
 *    float mouseX, mouseY;
 *    input->getMousePosition(mouseX, mouseY);
 * 
 */

#pragma once
#include <GL/glut.h>

class InputSystem {
private:
    static InputSystem* instance;
    bool mouseButtonStates[3];  // Left, Right, Middle
    bool keyStates[256];        // Regular keys
    bool specialKeyStates[256]; // Special keys (arrow keys, function keys)
    
    float lastMouseX, lastMouseY;
    float currentMouseX, currentMouseY;
    bool isDragging;
    bool isRotating;

    InputSystem();  // Private constructor for singleton

public:
    static InputSystem* getInstance();
    
    // GLUT callback registration
    void registerCallbacks();
    
    // Input handling methods (called by GLUT callbacks)
    void handleKeyPress(unsigned char key, int x, int y);
    void handleKeyRelease(unsigned char key, int x, int y);
    void handleSpecialKeyPress(int key, int x, int y);
    void handleSpecialKeyRelease(int key, int x, int y);
    void handleMouseButton(int button, int state, int x, int y);
    void handleMouseMotion(int x, int y);
    void handlePassiveMouseMotion(int x, int y);
    
    // Update method to be called each frame
    void update();
    
    // Query methods for input state
    bool isKeyPressed(unsigned char key);
    bool isSpecialKeyPressed(int key);
    bool isMovementKeyPressed();
    bool isMouseButtonPressed(int button);
    
    // Mouse position and movement
    void getMousePosition(float& x, float& y);
    void getMouseDelta(float& deltaX, float& deltaY);
    bool getIsDragging() const;
    bool getIsRotating() const;
};
