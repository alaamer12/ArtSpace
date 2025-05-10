#include "navigator.h"
#include "utility.h"



Navigator::Navigator()
    : currentScreen(Screen::StartScreen), onScreenChangeCallback(nullptr) {
}

Navigator::~Navigator() {
    // Clean up screen managers
    for (auto& pair : screens) {
        delete pair.second;
    }
    screens.clear();
}

void Navigator::navigateTo(Screen screen) {
    // Store the previous screen for the callback
    Screen previousScreen = currentScreen;
    
    // First hide the current screen
    ScreenManager* currentManager = getScreenManager(currentScreen);
    if (currentManager) {
        currentManager->hideScreen();
    }
    
    // Switch to the new screen
    currentScreen = screen;
    
    // Show the new screen
    ScreenManager* newManager = getScreenManager(currentScreen);
    if (newManager) {
        newManager->showScreen();
    }
    
    // Call the screen change callback if it exists
    if (onScreenChangeCallback) {
        onScreenChangeCallback(previousScreen, currentScreen);
    }
}

Screen Navigator::getCurrentScreen() const {
    return currentScreen;
}

void Navigator::registerScreen(ScreenManager* screenManager) {
    if (screenManager) {
        Screen type = screenManager->getType();
        
        // Delete any existing screen manager for this type
        auto it = screens.find(type);
        if (it != screens.end()) {
            delete it->second;
        }
        
        screens[type] = screenManager;
    }
}

void Navigator::unregisterScreen(Screen screenType) {
    auto it = screens.find(screenType);
    if (it != screens.end()) {
        delete it->second;
        screens.erase(it);
    }
}

ScreenManager* Navigator::getScreenManager(Screen screenType) {
    auto it = screens.find(screenType);
    if (it != screens.end()) {
        return it->second;
    }
    return nullptr;
}

void Navigator::renderCurrentScreen() {
    ScreenManager* manager = getScreenManager(currentScreen);
    if (manager) {
        manager->renderScreen();
    }
}

void Navigator::updateCurrentScreen(float deltaTime) {
    ScreenManager* manager = getScreenManager(currentScreen);
    if (manager) {
        manager->updateScreen(deltaTime);
    }
}

void Navigator::resizeAllScreens(int width, int height) {
    for (auto& pair : screens) {
        pair.second->resizeScreen(width, height);
    }
}

bool Navigator::handleMouseEvent(int button, int state, int x, int y) {
    ScreenManager* manager = getScreenManager(currentScreen);
    if (manager) {
        return manager->handleMouseEvent(button, state, x, y);
    }
    return false;
}

bool Navigator::handleKeyEvent(unsigned char key) {
    ScreenManager* manager = getScreenManager(currentScreen);
    if (manager) {
        return manager->handleKeyEvent(key);
    }
    return false;
}

bool Navigator::handleMouseMove(int x, int y) {
    ScreenManager* manager = getScreenManager(currentScreen);
    if (manager) {
        return manager->handleMouseMove(x, y);
    }
    return false;
}

void Navigator::addComponent(UIComponent* component) {
    ScreenManager* manager = getScreenManager(currentScreen);
    if (manager && component) {
        manager->addComponent(component);
    }
}

void Navigator::setOnScreenChangeCallback(ScreenChangeCallback callback) {
    onScreenChangeCallback = callback;
} 