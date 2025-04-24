#pragma once
#include "utility.h"
#include <map>
#include "screen_manager.h"
#include "screens.h"


// Navigator class to manage screens and transitions
class Navigator {
private:
    Screen currentScreen;
    std::map<Screen, ScreenManager*> screens;

public:
    Navigator();
    ~Navigator();
    
    void navigateTo(Screen screen);
    Screen getCurrentScreen() const;
    
    // Register a screen manager for a specific screen type
    void registerScreen(ScreenManager* screenManager);
    
    // Unregister a screen manager
    void unregisterScreen(Screen screenType);
    
    // Get a specific screen manager
    ScreenManager* getScreenManager(Screen screenType);
    
    // Handle current screen rendering and updating
    void renderCurrentScreen();
    void updateCurrentScreen(float deltaTime);
    
    // Resize all screens
    void resizeAllScreens(int width, int height);
    
    // Event handling delegates to current screen
    bool handleMouseEvent(int button, int state, int x, int y);
    bool handleKeyEvent(unsigned char key);
    bool handleMouseMove(int x, int y);
    
    // Add component to current screen
    void addComponent(UIComponent* component);
}; 