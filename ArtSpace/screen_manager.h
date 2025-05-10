#pragma once
#include <vector>
#include <algorithm>
#include "utility.h"
#include "screens.h"

// Screen class to manage a collection of components for a specific screen
class ScreenManager {
private:
    std::vector<UIComponent*> components;
    Screen screenType;
    
public:
    ScreenManager(Screen type) : screenType(type) {}
    virtual ~ScreenManager();
    
    Screen getType() const { return screenType; }
    
    void addComponent(UIComponent* component);
    void removeComponent(UIComponent* component);
    virtual void showScreen();
    virtual void hideScreen();
    void updateScreen(float deltaTime);
    void renderScreen();
    
    bool handleMouseEvent(int button, int state, int x, int y);
    bool handleKeyEvent(unsigned char key);
    bool handleMouseMove(int x, int y);
    
    virtual void resizeScreen(int width, int height) {
        // Derived classes should override this to reposition components
    }
    
    const std::vector<UIComponent*>& getComponents() const {
        return components;
    }
}; 