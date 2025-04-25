#include "screen_manager.h"

ScreenManager::~ScreenManager() {
    // Doesn't own components, just references them
}

void ScreenManager::addComponent(UIComponent* component) {
    if (component) {
        components.push_back(component);
    }
}

void ScreenManager::removeComponent(UIComponent* component) {
    auto it = std::find(components.begin(), components.end(), component);
    if (it != components.end()) {
        components.erase(it);
    }
}

void ScreenManager::showScreen() {
    for (auto* component : components) {
        if (component) {
            component->setVisible(true);
        }
    }
}

void ScreenManager::hideScreen() {
    for (auto* component : components) {
        if (component) {
            component->setVisible(false);
        }
    }
}

void ScreenManager::updateScreen(float deltaTime) {
    for (auto* component : components) {
        if (component && component->getVisible()) {
            component->update(deltaTime);
        }
    }
}

void ScreenManager::renderScreen() {
    for (auto* component : components) {
        if (component && component->getVisible()) {
            component->render();
        }
    }
}

bool ScreenManager::handleMouseEvent(int button, int state, int x, int y) {
    bool handled = false;
    
    // Process in reverse order to give top-most components priority
    for (auto it = components.rbegin(); it != components.rend(); ++it) {
        UIComponent* component = *it;
        
        if (!component->getVisible()) continue;
        
        // Check if component can handle mouse events
        Button* buttonComponent = dynamic_cast<Button*>(component);
        if (buttonComponent) {
            if (buttonComponent->handleMouseEvent(button, state, x, y)) {
                handled = true;
                break; // Stop after first component handles it
            }
        }
        
        TextBox* textboxComponent = dynamic_cast<TextBox*>(component);
        if (textboxComponent) {
            if (textboxComponent->handleMouseEvent(button, state, x, y)) {
                handled = true;
                break; // Stop after first component handles it
            }
        }
        
        // Check for BorderBox components
        BorderBox* borderboxComponent = dynamic_cast<BorderBox*>(component);
        if (borderboxComponent) {
            if (borderboxComponent->handleMouseEvent(button, state, x, y)) {
                handled = true;
                break; // Stop after first component handles it
            }
        }
    }
    
    return handled;
}

bool ScreenManager::handleKeyEvent(unsigned char key) {
    bool handled = false;
    
    for (auto* component : components) {
        if (!component->getVisible()) continue;
        
        if (component->handleKeyEvent(key)) {
            handled = true;
            break;
        }
    }
    
    return handled;
}

bool ScreenManager::handleMouseMove(int x, int y) {
    bool handled = false;
    
    for (auto* component : components) {
        if (!component->getVisible()) continue;
        
        Button* buttonComponent = dynamic_cast<Button*>(component);
        if (buttonComponent) {
            if (buttonComponent->handleMouseMove(x, y)) {
                handled = true;
                // Don't break here, as mouse move should update all components
            }
        }
        
        // Try BorderBox components
        BorderBox* borderboxComponent = dynamic_cast<BorderBox*>(component);
        if (borderboxComponent) {
            if (borderboxComponent->handleMouseMove(x, y)) {
                handled = true;
                // Don't break, should update all components
            }
        }
    }
    
    return handled;
} 