#pragma once
#include <GL/glut.h>
#include <string>

// Base UI component class
class UIComponent {
protected:
    float position[2];  // x, y in screen coordinates
    bool isVisible;
    float alpha;

public:
    UIComponent();
    virtual ~UIComponent() = default;

    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    
    void setPosition(float x, float y);
    void setVisible(bool visible);
    void setAlpha(float alpha);
    
protected:
    // Utility functions for OpenGL rendering
    void begin2D();
    void end2D();
    void renderText(const std::string& text, float x, float y);
};

// Progress bar component
class ProgressBar : public UIComponent {
private:
    float progress;
    float width;
    float height;
    std::string message;

public:
    ProgressBar();
    void setProgress(float value, const std::string& message = "");
    void update(float deltaTime) override;
    void render() override;
};

// Loading screen component
class LoadingScreen : public UIComponent {
private:
    std::string loadingText;
    float rotation;
    float spinnerRadius;

public:
    LoadingScreen();
    void setMessage(const std::string& message);
    void update(float deltaTime) override;
    void render() override;
};

// Message popup component
class MessagePopup : public UIComponent {
private:
    std::string messageText;
    float duration;
    float currentTime;
    float width;
    float height;

public:
    MessagePopup(const std::string& message, float duration);
    bool isExpired() const;
    void update(float deltaTime) override;
    void render() override;
};

// Timer display component
class Timer : public UIComponent {
private:
    std::string timeText;
    float currentTime;
    bool isRunning;

public:
    Timer();
    void start();
    void stop();
    void reset();
    void update(float deltaTime) override;
    void render() override;
    float getTime() const;
};
