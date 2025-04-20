#include "feedback.h"
#include "sound_manager.h"

FeedbackSystem* FeedbackSystem::instance = nullptr;

FeedbackSystem::FeedbackSystem()
    : progressBar(nullptr)
    , loadingScreen(nullptr)
    , isLoading(false)
    , fadeAlpha(0.0f) {
}

FeedbackSystem* FeedbackSystem::getInstance() {
    if (instance == nullptr) {
        instance = new FeedbackSystem();
    }
    return instance;
}

FeedbackSystem::~FeedbackSystem() {
    for (auto msg : activeMessages) {
        delete msg;
    }
    activeMessages.clear();
    
    delete progressBar;
    delete loadingScreen;
}

void FeedbackSystem::showMessage(const std::string& message, float duration) {
    // TODO: Create and add message popup to active messages
}

void FeedbackSystem::showProgress(float progress, const std::string& message) {
    if (!progressBar) {
        // TODO: Create progress bar
    }
    // TODO: Update progress bar
}

void FeedbackSystem::showLoadingScreen(const std::string& message) {
    if (!loadingScreen) {
        // TODO: Create loading screen
    }
    isLoading = true;
    // TODO: Update loading screen message
}

void FeedbackSystem::hideLoadingScreen() {
    isLoading = false;
}

void FeedbackSystem::showWinState(const std::string& message) {
    // TODO: Show win state animation and effects
    SoundManager::getInstance()->playSound("win");
    showMessage(message, 5.0f);
}

void FeedbackSystem::showLoseState(const std::string& message) {
    // TODO: Show lose state animation and effects
    SoundManager::getInstance()->playSound("lose");
    showMessage(message, 5.0f);
}

void FeedbackSystem::update(float deltaTime) {
    // Update active messages
    for (auto it = activeMessages.begin(); it != activeMessages.end();) {
        // TODO: Update message timers and handle fadeout
        ++it;
    }
    
    // Update loading screen if active
    if (isLoading && loadingScreen) {
        // TODO: Update loading screen animation
    }
}

void FeedbackSystem::render(sf::RenderWindow& window) {
    // Render progress bar if active
    if (progressBar) {
        // TODO: Render progress bar
    }
    
    // Render loading screen if active
    if (isLoading && loadingScreen) {
        // TODO: Render loading screen
    }
    
    // Render active messages
    for (const auto& msg : activeMessages) {
        // TODO: Render message popups
    }
}
