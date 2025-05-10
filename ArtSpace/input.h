#pragma once
#include <GL/glut.h>
#include <cstring>

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
    
    // Query methods for input state
    bool isKeyPressed(unsigned char key);
    bool isSpecialKeyPressed(int key);
    bool isMovementKeyPressed();
    bool isMouseButtonPressed(int button);
    
    void getMouseDelta(float& deltaX, float& deltaY);
};
