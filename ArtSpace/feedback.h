#pragma once
#include <GL/glut.h>
#include <string>
#include <vector>

// Forward declarations
class ProgressBar;
class LoadingScreen;
class MessagePopup;

class FeedbackSystem {
private:
    static FeedbackSystem* instance;
    std::vector<MessagePopup*> activeMessages;
    ProgressBar* progressBar;
    LoadingScreen* loadingScreen;
    bool isLoading;
    float fadeAlpha;
    int windowWidth;
    int windowHeight;

    FeedbackSystem();  // Private constructor for singleton

public:
    static FeedbackSystem* getInstance();
    ~FeedbackSystem();

    // Visual feedback
    void showMessage(const std::string& message, float duration = 3.0f);
    void showProgress(float progress, const std::string& message = "");
    void showLoadingScreen(const std::string& message = "Loading...");
    void hideLoadingScreen();

    // State feedback
    void showWinState(const std::string& message = "Level Complete!");
    void showLoseState(const std::string& message = "Try Again!");
    
    // Update and render
    void update(float deltaTime);
    void render();  // Changed to not require SFML window
    
    // OpenGL specific
    void setWindowSize(int width, int height);
    void renderText(const std::string& text, float x, float y);
    void setup2DMode();
    void restore3DMode();
};
