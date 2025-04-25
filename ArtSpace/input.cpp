#include "input.h"
#include <cstring>

// Initialize static member
InputSystem* InputSystem::instance = nullptr;

// Static callback functions for GLUT integration
void keyboardCallback(unsigned char key, int x, int y) {
    InputSystem::getInstance()->handleKeyPress(key, x, y);
}

void keyboardUpCallback(unsigned char key, int x, int y) {
    InputSystem::getInstance()->handleKeyRelease(key, x, y);
}

void specialCallback(int key, int x, int y) {
    InputSystem::getInstance()->handleSpecialKeyPress(key, x, y);
}

void specialUpCallback(int key, int x, int y) {
    InputSystem::getInstance()->handleSpecialKeyRelease(key, x, y);
}

void mouseCallback(int button, int state, int x, int y) {
    InputSystem::getInstance()->handleMouseButton(button, state, x, y);
}

void motionCallback(int x, int y) {
    InputSystem::getInstance()->handleMouseMotion(x, y);
}

void passiveMotionCallback(int x, int y) {
    InputSystem::getInstance()->handlePassiveMouseMotion(x, y);
}

// Constructor
InputSystem::InputSystem() {
    // Initialize mouse button states to false (not pressed)
    memset(mouseButtonStates, 0, sizeof(mouseButtonStates));
    memset(keyStates, 0, sizeof(keyStates));
    memset(specialKeyStates, 0, sizeof(specialKeyStates));
    
    lastMouseX = 0.0f;
    lastMouseY = 0.0f;
    currentMouseX = 0.0f;
    currentMouseY = 0.0f;
    
    isDragging = false;
    isRotating = false;
}

// Get singleton instance
InputSystem* InputSystem::getInstance() {
    if (instance == nullptr) {
        instance = new InputSystem();
    }
    return instance;
}

// Register callbacks with GLUT
void InputSystem::registerCallbacks() {
    glutKeyboardFunc(keyboardCallback);
    glutKeyboardUpFunc(keyboardUpCallback);
    glutSpecialFunc(specialCallback);
    glutSpecialUpFunc(specialUpCallback);
    glutMouseFunc(mouseCallback);
    glutMotionFunc(motionCallback);
    glutPassiveMotionFunc(passiveMotionCallback);
}

// Handle keyboard press events
void InputSystem::handleKeyPress(unsigned char key, int x, int y) {
    keyStates[key] = true;
}

// Handle keyboard release events
void InputSystem::handleKeyRelease(unsigned char key, int x, int y) {
    keyStates[key] = false;
}

// Handle special key press events
void InputSystem::handleSpecialKeyPress(int key, int x, int y) {
    if (key >= 0 && key < 256) {
        specialKeyStates[key] = true;
    }
}

// Handle special key release events
void InputSystem::handleSpecialKeyRelease(int key, int x, int y) {
    if (key >= 0 && key < 256) {
        specialKeyStates[key] = false;
    }
}

// Handle mouse button events
void InputSystem::handleMouseButton(int button, int state, int x, int y) {
    if (button >= 0 && button < 3) {
        // GLUT_DOWN = 0, GLUT_UP = 1, so we invert it for our boolean state
        mouseButtonStates[button] = (state == GLUT_DOWN);
        
        // Update mouse position
        currentMouseX = static_cast<float>(x);
        currentMouseY = static_cast<float>(y);
        
        // If left button is pressed, we're starting a drag operation
        if (button == GLUT_LEFT_BUTTON) {
            isDragging = mouseButtonStates[button];
        }
        
        // If right button is pressed, we're starting a rotation operation
        if (button == GLUT_RIGHT_BUTTON) {
            isRotating = mouseButtonStates[button];
        }
    }
}

// Handle mouse motion events (when button is pressed)
void InputSystem::handleMouseMotion(int x, int y) {
    // Save last position before updating
    lastMouseX = currentMouseX;
    lastMouseY = currentMouseY;
    
    // Update current position
    currentMouseX = static_cast<float>(x);
    currentMouseY = static_cast<float>(y);
}

// Handle passive mouse motion events (when no button is pressed)
void InputSystem::handlePassiveMouseMotion(int x, int y) {
    // Save last position before updating
    lastMouseX = currentMouseX;
    lastMouseY = currentMouseY;
    
    // Update current position
    currentMouseX = static_cast<float>(x);
    currentMouseY = static_cast<float>(y);
}

// Update method to be called each frame
void InputSystem::update() {
    // Any per-frame logic can go here
    
    // Mouse delta is now reset in getMouseDelta() method
}

// Check if a key is currently pressed
bool InputSystem::isKeyPressed(unsigned char key) {
    return keyStates[key];
}

// Check if a special key is currently pressed
bool InputSystem::isSpecialKeyPressed(int key) {
    return (key >= 0 && key < 256) ? specialKeyStates[key] : false;
}

// Check if any of the WASD keys are pressed
bool InputSystem::isMovementKeyPressed() {
    return keyStates['w'] || keyStates['a'] || keyStates['s'] || keyStates['d'] ||
           keyStates['W'] || keyStates['A'] || keyStates['S'] || keyStates['D'];
}

// Check if a mouse button is currently pressed
bool InputSystem::isMouseButtonPressed(int button) {
    return (button >= 0 && button < 3) ? mouseButtonStates[button] : false;
}

// Get the current mouse position
void InputSystem::getMousePosition(float& x, float& y) {
    x = currentMouseX;
    y = currentMouseY;
}

// Get the mouse movement delta since last update
void InputSystem::getMouseDelta(float& deltaX, float& deltaY) {
    // Calculate the delta
    deltaX = currentMouseX - lastMouseX;
    deltaY = currentMouseY - lastMouseY;
    
    // Reset immediately after reading to prevent continued camera movement
    lastMouseX = currentMouseX;
    lastMouseY = currentMouseY;
}

// Check if the mouse is being dragged
bool InputSystem::getIsDragging() const {
    return isDragging;
}

// Check if the mouse is being used for rotation
bool InputSystem::getIsRotating() const {
    return isRotating;
}
