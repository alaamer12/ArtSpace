#include "input.h"

InputSystem* InputSystem::instance = nullptr;

InputSystem::InputSystem() {
    for (int i = 0; i < 3; i++) {
        mouseButtonStates[i] = false;
    }
    lastMouseX = lastMouseY = 0.0f;
    isDragging = false;
}

InputSystem* InputSystem::getInstance() {
    if (instance == nullptr) {
        instance = new InputSystem();
    }
    return instance;
}

void InputSystem::update() {
    // TODO: Update mouse position and button states
    // TODO: Update keyboard states
    // TODO: Process input events
}

bool InputSystem::isKeyPressed(sf::Keyboard::Key key) {
    return sf::Keyboard::isKeyPressed(key);
}

bool InputSystem::isMouseButtonPressed(sf::Mouse::Button button) {
    return sf::Mouse::isButtonPressed(button);
}

void InputSystem::getMousePosition(float& x, float& y) {
    sf::Vector2i pos = sf::Mouse::getPosition();
    x = static_cast<float>(pos.x);
    y = static_cast<float>(pos.y);
}

void InputSystem::getMouseDelta(float& deltaX, float& deltaY) {
    // TODO: Calculate mouse movement delta
}

bool InputSystem::getIsDragging() const {
    return isDragging;
}
