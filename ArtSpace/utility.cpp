#include "utility.h"
#include "config.h"
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

// Initialize static instance
Logger Logger::instance;

// Logger implementation
Logger::Logger()
    : consoleLogging(true)
    , fileLogging(false)
    , logFilePath("artspace.log")
    , minLevel(LogLevel::INFO) {
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

Logger& Logger::getInstance() {
    return instance;
}

void Logger::enableConsoleLogging(bool enable) {
    consoleLogging = enable;
}

void Logger::enableFileLogging(bool enable, const std::string& filePath) {
    fileLogging = enable;
    logFilePath = filePath;
    
    if (fileLogging) {
        if (logFile.is_open()) {
            logFile.close();
        }
        
        logFile.open(logFilePath, std::ios::app);
        if (!logFile.is_open()) {
            std::cerr << "ERROR: Could not open log file: " << logFilePath << std::endl;
            fileLogging = false;
        }
    }
}

void Logger::setMinimumLogLevel(LogLevel level) {
    minLevel = level;
}

void Logger::logInfo(const std::string& message) {
    log(LogLevel::INFO, message);
}

void Logger::logWarning(const std::string& message) {
    log(LogLevel::WARNING, message);
}

void Logger::logError(const std::string& message) {
    log(LogLevel::ERROR_, message);
}

void Logger::logDebug(const std::string& message) {
    log(LogLevel::DEBUG, message);
}

void Logger::log(LogLevel level, const std::string& message) {
    // Skip if below minimum level
    if (level < minLevel) return;
    
    // Get current time
    auto now = std::chrono::system_clock::now();
    auto nowTime = std::chrono::system_clock::to_time_t(now);
    
    // Format timestamp
    std::stringstream timestampStream;
    struct tm timeInfo;
#ifdef _WIN32
    localtime_s(&timeInfo, &nowTime);
#else
    localtime_r(&nowTime, &timeInfo);
#endif
    
    timestampStream << std::put_time(&timeInfo, "%Y-%m-%d %H:%M:%S");
    std::string timestamp = timestampStream.str();
    
    // Get level string
    std::string levelStr;
    switch (level) {
        case LogLevel::INFO:
            levelStr = "INFO";
            break;
        case LogLevel::WARNING:
            levelStr = "WARNING";
            break;
        case LogLevel::ERROR_:
            levelStr = "ERROR";
            break;
        case LogLevel::DEBUG:
            levelStr = "DEBUG";
            break;
        default:
            levelStr = "UNKNOWN";
            break;
    }
    
    // Format log entry
    std::string logEntry = "[" + timestamp + "] [" + levelStr + "] " + message;
    
    // Output to console if enabled
    if (consoleLogging) {
        std::cout << logEntry << std::endl;
    }
    
    // Output to file if enabled
    if (fileLogging && logFile.is_open()) {
        logFile << logEntry << std::endl;
        logFile.flush();
    }
}

// UIComponent base class implementation
UIComponent::UIComponent(): isVisible(true), alpha(1.0f) {
    position[0] = position[1] = 0.0f;
    size[0] = size[1] = 100.0f;
}

void UIComponent::setPosition(float x, float y) {
    position[0] = x;
    position[1] = y;
}

void UIComponent::setSize(float width, float height) {
    size[0] = width;
    size[1] = height;
}

void UIComponent::setVisible(bool visible) {
    isVisible = visible;
}

void UIComponent::setAlpha(float a) {
    alpha = std::max(0.0f, std::min(1.0f, a));
}

bool UIComponent::isPointInside(float x, float y) const {
    return (x >= position[0] && x <= position[0] + size[0] &&
            y >= position[1] && y <= position[1] + size[1]);
}

// Function to calculate text position based on alignment
void calculateTextPosition(float boxX, float boxY, float boxWidth, float boxHeight, 
                          const std::string& text, TextAlignment alignment,
                          float& outX, float& outY) {
    // Approximate text width (8 pixels per character for GLUT_BITMAP_HELVETICA_12)
    float textWidth = text.length() * 8.0f;
    
    // Calculate Y position (vertical mid-point is the same for all alignments)
    outY = boxY + (boxHeight + 4.0f) / 2.0f; // +4 for vertical centering adjustment
    
    // Calculate X position based on alignment
    switch (alignment) {
        case TextAlignment::MidLeft:
            outX = boxX + 5.0f; // 5 pixels padding from left edge
            break;
        case TextAlignment::MidMid:
            outX = boxX + (boxWidth - textWidth) / 2.0f; // Centered horizontally
            break;
        case TextAlignment::MidRight:
            outX = boxX + boxWidth - textWidth - 5.0f; // 5 pixels padding from right edge
            break;
    }
}

// Helper function to render text using GLUT
void renderText(float x, float y, const std::string& text, float* color) {
    // Set color if provided, default to white
    if (color) {
        glColor4f(color[0], color[1], color[2], color[3]);
    } else {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    }
    
    // Position the text
    glRasterPos2f(x, y);
    
    // Render each character
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
}

// Overloaded renderText that supports alignment options
void renderText(float boxX, float boxY, float boxWidth, float boxHeight, 
               const std::string& text, TextAlignment alignment, float* color) {
    float x, y;
    calculateTextPosition(boxX, boxY, boxWidth, boxHeight, text, alignment, x, y);
    renderText(x, y, text, color);
}

// Button implementation
Button::Button(const std::string& buttonText)
    : text(buttonText)
    , isHovered(false)
    , isPressed(false) {
    
    // Default colors
    textColor[0] = textColor[1] = textColor[2] = 1.0f;
    textColor[3] = 1.0f;
    
    backgroundColor[0] = backgroundColor[1] = backgroundColor[2] = 0.3f;
    backgroundColor[3] = 1.0f;
    
    hoverColor[0] = hoverColor[1] = hoverColor[2] = 0.5f;
    hoverColor[3] = 1.0f;
    
    // Default size based on text length
    size[0] = text.length() * 8.0f + 20.0f;
    size[1] = 30.0f;
}

void Button::render() {
    if (!isVisible) return;
    
    // Draw button background
    if (isHovered) {
        glColor4f(hoverColor[0], hoverColor[1], hoverColor[2], hoverColor[3] * alpha);
    } else {
        glColor4f(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3] * alpha);
    }
    
    glBegin(GL_QUADS);
    glVertex2f(position[0], position[1]);
    glVertex2f(position[0] + size[0], position[1]);
    glVertex2f(position[0] + size[0], position[1] + size[1]);
    glVertex2f(position[0], position[1] + size[1]);
    glEnd();
    
    // Use different alignment based on the button text
    TextAlignment alignment = TextAlignment::MidMid; // Default to center align
    
    // Special handling for demo buttons
    if (text == "Left Aligned Text") {
        alignment = TextAlignment::MidLeft;
    } else if (text == "Center Aligned Text") {
        alignment = TextAlignment::MidMid;
    } else if (text == "Right Aligned Text") {
        alignment = TextAlignment::MidRight;
    }
    
    // Render text with selected alignment
    renderText(position[0], position[1], size[0], size[1], text, alignment, textColor);
}

void Button::update(float deltaTime) {
    // No animation or update needed for basic button
}

void Button::setText(const std::string& buttonText) {
    text = buttonText;
    // Update size based on new text length
    size[0] = text.length() * 8.0f + 20.0f;
}

void Button::setOnClick(const std::function<void()>& callback) {
    onClick = callback;
}

void Button::setTextColor(float r, float g, float b, float a) {
    textColor[0] = r;
    textColor[1] = g;
    textColor[2] = b;
    textColor[3] = a;
}

void Button::setBackgroundColor(float r, float g, float b, float a) {
    backgroundColor[0] = r;
    backgroundColor[1] = g;
    backgroundColor[2] = b;
    backgroundColor[3] = a;
}

void Button::setHoverColor(float r, float g, float b, float a) {
    hoverColor[0] = r;
    hoverColor[1] = g;
    hoverColor[2] = b;
    hoverColor[3] = a;
}

bool Button::handleMouseEvent(int button, int state, int x, int y) {
    if (!isVisible) return false;
    
    // Convert window coordinates to OpenGL coordinates
    int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
    float glY = windowHeight - y; // Invert Y coordinate
    
    bool wasInside = isPointInside(x, glY);
    
    if (wasInside && button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isPressed = true;
            return true;
        } else if (state == GLUT_UP && isPressed) {
            isPressed = false;
            if (onClick) {
                onClick();
            }
            return true;
        }
    }
    
    if (state == GLUT_UP) {
        isPressed = false;
    }
    
    return wasInside;
}

bool Button::handleMouseMove(int x, int y) {
    if (!isVisible) return false;
    
    // Convert window coordinates to OpenGL coordinates
    int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
    float glY = windowHeight - y; // Invert Y coordinate
    
    isHovered = isPointInside(x, glY);
    return isHovered;
}

// TextBox implementation
TextBox::TextBox(const std::string& initialText, const std::string& placeholderText)
    : text(initialText)
    , placeholder(placeholderText)
    , isEditable(true)
    , hasFocus(false)
    , cursorPosition(initialText.length())
    , cursorBlinkTime(0.0f)
    , showCursor(false) {
    
    // Default colors
    textColor[0] = textColor[1] = textColor[2] = 1.0f;
    textColor[3] = 1.0f;
    
    backgroundColor[0] = backgroundColor[1] = backgroundColor[2] = 0.2f;
    backgroundColor[3] = 1.0f;
    
    borderColor[0] = borderColor[1] = borderColor[2] = 0.5f;
    borderColor[3] = 1.0f;
    
    // Default size
    size[0] = 200.0f;
    size[1] = 30.0f;
}

void TextBox::render() {
    if (!isVisible) return;
    
    // Draw textbox background
    glColor4f(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3] * alpha);
    glBegin(GL_QUADS);
    glVertex2f(position[0], position[1]);
    glVertex2f(position[0] + size[0], position[1]);
    glVertex2f(position[0] + size[0], position[1] + size[1]);
    glVertex2f(position[0], position[1] + size[1]);
    glEnd();
    
    // Draw border
    glColor4f(borderColor[0], borderColor[1], borderColor[2], borderColor[3] * alpha);
    glBegin(GL_LINE_LOOP);
    glVertex2f(position[0], position[1]);
    glVertex2f(position[0] + size[0], position[1]);
    glVertex2f(position[0] + size[0], position[1] + size[1]);
    glVertex2f(position[0], position[1] + size[1]);
    glEnd();
    
    // Draw text or placeholder using left alignment
    if (!text.empty()) {
        renderText(position[0], position[1], size[0], size[1], text, TextAlignment::MidLeft, textColor);
    } else if (!hasFocus && !placeholder.empty()) {
        // Draw placeholder with reduced alpha
        float placeholderColor[4] = {
            textColor[0], textColor[1], textColor[2], textColor[3] * 0.5f
        };
        renderText(position[0], position[1], size[0], size[1], placeholder, TextAlignment::MidLeft, placeholderColor);
    }
    
    // Draw cursor if focused and cursor should be shown
    if (hasFocus && showCursor && isEditable) {
        // Calculate cursor position
        float textX, textY;
        calculateTextPosition(position[0], position[1], size[0], size[1], 
                             text.substr(0, cursorPosition), TextAlignment::MidLeft, textX, textY);
        
        // Draw cursor at calculated position
        glColor4f(textColor[0], textColor[1], textColor[2], textColor[3]);
        glBegin(GL_LINES);
        glVertex2f(textX, position[1] + 5.0f);
        glVertex2f(textX, position[1] + size[1] - 5.0f);
        glEnd();
    }
}

void TextBox::update(float deltaTime) {
    // Blink cursor
    if (hasFocus) {
        cursorBlinkTime += deltaTime;
        if (cursorBlinkTime >= 0.5f) {
            cursorBlinkTime = 0.0f;
            showCursor = !showCursor;
        }
    }
}

void TextBox::setText(const std::string& newText) {
    text = newText;
    cursorPosition = text.length();
}

const std::string& TextBox::getText() const {
    return text;
}

void TextBox::setPlaceholder(const std::string& newPlaceholder) {
    placeholder = newPlaceholder;
}

void TextBox::setEditable(bool editable) {
    isEditable = editable;
}

void TextBox::setTextColor(float r, float g, float b, float a) {
    textColor[0] = r;
    textColor[1] = g;
    textColor[2] = b;
    textColor[3] = a;
}

void TextBox::setBackgroundColor(float r, float g, float b, float a) {
    backgroundColor[0] = r;
    backgroundColor[1] = g;
    backgroundColor[2] = b;
    backgroundColor[3] = a;
}

void TextBox::setBorderColor(float r, float g, float b, float a) {
    borderColor[0] = r;
    borderColor[1] = g;
    borderColor[2] = b;
    borderColor[3] = a;
}

bool TextBox::handleKeyEvent(unsigned char key) {
    if (!isVisible || !hasFocus || !isEditable) return false;
    
    // Handle backspace
    if (key == 8 && cursorPosition > 0) {
        text.erase(cursorPosition - 1, 1);
        cursorPosition--;
        return true;
    }
    
    // Handle delete
    if (key == 127 && cursorPosition < text.length()) {
        text.erase(cursorPosition, 1);
        return true;
    }
    
    // Handle left/right arrow keys (special keys should be handled elsewhere)
    
    // Handle printable characters
    if (key >= 32 && key <= 126) {
        text.insert(cursorPosition, 1, key);
        cursorPosition++;
        return true;
    }
    
    return false;
}

bool TextBox::handleMouseEvent(int button, int state, int x, int y) {
    if (!isVisible) return false;
    
    // Convert window coordinates to OpenGL coordinates
    int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
    float glY = windowHeight - y; // Invert Y coordinate
    
    bool wasInside = isPointInside(x, glY);
    
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        hasFocus = wasInside;
        showCursor = hasFocus;
        cursorBlinkTime = 0.0f;
        
        // Update cursor position based on click position
        if (wasInside && !text.empty()) {
            float textStart = position[0] + 5.0f;
            float charWidth = 8.0f; // Approximate width per character
            int clickedPos = static_cast<int>((x - textStart) / charWidth);
            cursorPosition = std::max(0, std::min(static_cast<int>(text.length()), clickedPos));
        }
    }
    
    return wasInside;
}

// Timer implementation
Timer::Timer()
    : currentTime(0.0f)
    , isRunning(false) {
    
    // Default text
    timeText = "00:00";
    
    // Default colors
    textColor[0] = textColor[1] = textColor[2] = 1.0f;
    textColor[3] = 1.0f;
    
    // Default size
    size[0] = 80.0f;
    size[1] = 30.0f;
}

void Timer::start() {
    isRunning = true;
}

void Timer::stop() {
    isRunning = false;
}

void Timer::reset() {
    currentTime = 0.0f;
    timeText = "00:00";
}

void Timer::update(float deltaTime) {
    if (!isRunning) return;
    
    currentTime += deltaTime;
    
    // Format time as MM:SS
    int minutes = static_cast<int>(currentTime) / 60;
    int seconds = static_cast<int>(currentTime) % 60;
    
    char buffer[10];
    sprintf_s(buffer, "%02d:%02d", minutes, seconds);
    timeText = buffer;
}

void Timer::render() {
    if (!isVisible) return;
    
    // Draw timer background (optional)
    // glColor4f(0.2f, 0.2f, 0.2f, 0.7f * alpha);
    // glBegin(GL_QUADS);
    // glVertex2f(position[0], position[1]);
    // glVertex2f(position[0] + size[0], position[1]);
    // glVertex2f(position[0] + size[0], position[1] + size[1]);
    // glVertex2f(position[0], position[1] + size[1]);
    // glEnd();
    
    // Draw timer text using right alignment
    renderText(position[0], position[1], size[0], size[1], timeText, TextAlignment::MidRight, textColor);
}

float Timer::getTime() const {
    return currentTime;
}

void Timer::setTextColor(float r, float g, float b, float a) {
    textColor[0] = r;
    textColor[1] = g;
    textColor[2] = b;
    textColor[3] = a;
}

// Navigator implementation
Navigator::Navigator()
    : currentScreen(Screen::StartScreen) {
}

Navigator::~Navigator() {
    // Note: Navigator doesn't own the components, it just references them
}

void Navigator::navigateTo(Screen screen) {
    currentScreen = screen;
}

Screen Navigator::getCurrentScreen() const {
    return currentScreen;
}

void Navigator::addComponent(UIComponent* component) {
    if (component) {
        screenComponents.push_back(component);
    }
}

void Navigator::removeComponent(UIComponent* component) {
    auto it = std::find(screenComponents.begin(), screenComponents.end(), component);
    if (it != screenComponents.end()) {
        screenComponents.erase(it);
    }
}

void Navigator::renderCurrentScreen() {
    for (auto* component : screenComponents) {
        if (component && component->getVisible()) {
            component->render();
        }
    }
}

void Navigator::updateCurrentScreen(float deltaTime) {
    for (auto* component : screenComponents) {
        if (component && component->getVisible()) {
            component->update(deltaTime);
        }
    }
}

bool Navigator::handleMouseEvent(int button, int state, int x, int y) {
    bool handled = false;
    
    // Process in reverse order to give top-most components priority
    for (auto it = screenComponents.rbegin(); it != screenComponents.rend(); ++it) {
        UIComponent* component = *it;
        
        // Check if component can handle mouse events
        Button* buttonComponent = dynamic_cast<Button*>(component);
        if (buttonComponent && buttonComponent->getVisible()) {
            if (buttonComponent->handleMouseEvent(button, state, x, y)) {
                handled = true;
                break; // Stop after first component handles it
            }
        }
        
        TextBox* textboxComponent = dynamic_cast<TextBox*>(component);
        if (textboxComponent && textboxComponent->getVisible()) {
            if (textboxComponent->handleMouseEvent(button, state, x, y)) {
                handled = true;
                break; // Stop after first component handles it
            }
        }
    }
    
    return handled;
}

bool Navigator::handleKeyEvent(unsigned char key) {
    bool handled = false;
    
    // Find a focused textbox to send key events to
    for (auto* component : screenComponents) {
        TextBox* textboxComponent = dynamic_cast<TextBox*>(component);
        if (textboxComponent && textboxComponent->getVisible()) {
            if (textboxComponent->handleKeyEvent(key)) {
                handled = true;
                break;
            }
        }
    }
    
    return handled;
}

bool Navigator::handleMouseMove(int x, int y) {
    bool handled = false;
    
    for (auto* component : screenComponents) {
        Button* buttonComponent = dynamic_cast<Button*>(component);
        if (buttonComponent && buttonComponent->getVisible()) {
            if (buttonComponent->handleMouseMove(x, y)) {
                handled = true;
                // Don't break here, as mouse move should update all components
            }
        }
    }
    
    return handled;
}
