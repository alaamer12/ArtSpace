#pragma once
#include <SFML/Window.hpp>

class InputSystem {
private:
    static InputSystem* instance;
    bool mouseButtonStates[3];  // Left, Right, Middle
    float lastMouseX, lastMouseY;
    bool isDragging;

    InputSystem();  // Private constructor for singleton

public:
    static InputSystem* getInstance();
    
    void update();
    bool isKeyPressed(sf::Keyboard::Key key);
    bool isMouseButtonPressed(sf::Mouse::Button button);
    
    // Mouse handling
    void getMousePosition(float& x, float& y);
    void getMouseDelta(float& deltaX, float& deltaY);
    bool getIsDragging() const;
    
    // TODO: Add gesture recognition
    // TODO: Add input mapping system
    // TODO: Add input event callbacks
};
