#include "utility.h"
#include "config.h"

// UIComponent base class implementation
UIComponent::UIComponent() 
    : position(0, 0)
    , isVisible(true)
    , alpha(1.0f) {
}

void UIComponent::setPosition(const sf::Vector2f& pos) {
    position = pos;
}

void UIComponent::setVisible(bool visible) {
    isVisible = visible;
}

void UIComponent::setAlpha(float a) {
    alpha = a;
}

// ProgressBar implementation
ProgressBar::ProgressBar(){}

void ProgressBar::setProgress(float value, const std::string& msg) {}

void ProgressBar::update(float deltaTime) {}

void ProgressBar::render(sf::RenderWindow& window) {}

// LoadingScreen implementation
LoadingScreen::LoadingScreen(){
    // TODO: Initialize font and text
}

void LoadingScreen::setMessage(const std::string& message) {
    loadingText.setString(message);
    // TODO: Center text
}

void LoadingScreen::render(sf::RenderWindow& window) {}

// MessagePopup implementation
MessagePopup::MessagePopup(const std::string& message, float dur)
    : duration(dur)
    , currentTime(0.0f) {
    background.setSize(sf::Vector2f(400, 100));
    background.setFillColor(sf::Color(0, 0, 0, 200));
    
    // TODO: Initialize font and text
    messageText.setString(message);
}

bool MessagePopup::isExpired() const {
    return currentTime >= duration;
}

void MessagePopup::update(float deltaTime) {
    currentTime += deltaTime;
    
    // Fade out near end of duration
    if (currentTime > duration - 1.0f) {
        float fadeAlpha = 1.0f - (currentTime - (duration - 1.0f));
        setAlpha(std::max(0.0f, fadeAlpha));
    }
}

void MessagePopup::render(sf::RenderWindow& window) {
    if (!isVisible) return;
    window.draw(background);
    window.draw(messageText);
}

// Timer implementation
Timer::Timer()
    : currentTime(0.0f)
    , isRunning(false) {
    // TODO: Initialize font and text
}

void Timer::start() {
    isRunning = true;
}

void Timer::stop() {
    isRunning = false;
}

void Timer::reset() {
    currentTime = 0.0f;
}

void Timer::update(float deltaTime) {
    if (isRunning) {
        currentTime += deltaTime;
    }
    
    int minutes = static_cast<int>(currentTime) / 60;
    int seconds = static_cast<int>(currentTime) % 60;
    
    char timeString[9];
    sprintf_s(timeString, "%02d:%02d", minutes, seconds);
    timeText.setString(timeString);
}

void Timer::render(sf::RenderWindow& window) {
    if (!isVisible) return;
    window.draw(timeText);
}

float Timer::getTime() const {
    return currentTime;
}
