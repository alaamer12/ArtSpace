/**
 * @file utility.h
 * @brief Utility classes and UI components for ArtSpace application
 * 
 * This module provides various utility classes and UI components for creating interactive
 * graphical applications with OpenGL/GLUT. It includes a logging system, text rendering,
 * and a complete UI component hierarchy.
 * 
 * Main classes:
 * - Logger: Singleton class for centralized application logging
 * - UIComponent: Base class for all UI elements with common functionality
 * - Image: Displays images with optional tinting and aspect ratio preservation
 * - BorderBox: Container component that adds borders and background to other components
 * - Button: Interactive button with hover and click states
 * - TextBox: Text input component with editing capabilities
 * - Timer: Component for displaying and tracking elapsed time
 * 
 * Usage examples:
 * 
 * 1. Using the Logger:
 *    Logger& logger = Logger::getInstance();
 *    logger.enableConsoleLogging(true);
 *    logger.logInfo("Application started");
 * 
 * 2. Creating UI components:
 *    Button* startButton = new Button("Start");
 *    startButton->setPosition(100, 100);
 *    startButton->setSize(120, 40);
 *    startButton->setOnClick([]() { // handle click });
 * 
 * 3. Using a BorderBox as a container:
 *    TextBox* inputBox = new TextBox("Enter text");
 *    BorderBox* container = new BorderBox(inputBox);
 *    container->setPosition(200, 150);
 *    container->setBorderColor(0.2f, 0.5f, 0.8f);
 * 
 * 4. Creating a timer:
 *    Timer* gameTimer = new Timer();
 *    gameTimer->setPosition(10, 10);
 *    gameTimer->start();
 */

#pragma once
#include <GL/glut.h>
#include <string>
#include <vector>
#include <functional>
#include <fstream>
#include <map>

// Forward declarations - don't include the full headers
class Config;
class Button;
class TextBox;
class Timer;
class UIComponent;
class ScreenManager;
class Navigator;
class MainScreenManager;
class TimerScreenManager;
class InputScreenManager;

// TextAlignment enumeration for text rendering
enum class TextAlignment {
    MidLeft,  // Align text to the left while vertically centered
    MidMid,   // Center text both horizontally and vertically
    MidRight  // Align text to the right while vertically centered
};

// Logger class for centralized logging
class Logger {
public:
    enum LogLevel {
        INFO,
        WARNING,
        DEBUG,
        ERROR_
    };

private:
    static Logger instance;
    bool consoleLogging;
    bool fileLogging;
    std::string logFilePath;
    std::ofstream logFile;
    LogLevel minLevel;

    // Private constructor (singleton)
    Logger();

public:
    // Delete copy constructor and assignment operator
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    // Destructor
    ~Logger();

    // Get singleton instance
    static Logger& getInstance();
    
    // Configuration
    void enableConsoleLogging(bool enable);
    void enableFileLogging(bool enable, const std::string& filePath = "artspace.log");
    void setMinimumLogLevel(LogLevel level);
    
    // Logging methods
    void logInfo(const std::string& message);
    void logWarning(const std::string& message);
    void logError(const std::string& message);
    void logDebug(const std::string& message);
    
    // Log with custom level
    void log(LogLevel level, const std::string& message);
};

// Forward text-related function declarations
void calculateTextPosition(float boxX, float boxY, float boxWidth, float boxHeight, 
                         const std::string& text, TextAlignment alignment,
                         float& outX, float& outY);
void renderText(float x, float y, const std::string& text, float* color = nullptr);
void renderText(float boxX, float boxY, float boxWidth, float boxHeight, 
               const std::string& text, TextAlignment alignment, float* color = nullptr);

// Base UI component class
class UIComponent {
protected:
    float position[2];  // x, y in screen coordinates
    float size[2];      // width, height
    bool isVisible;
    float alpha;

public:
    UIComponent();
    virtual ~UIComponent() = default;
    
    virtual void render() = 0;
    virtual void update(float deltaTime) {}
    
    void setPosition(float x, float y);
    void setSize(float width, float height);
    void setVisible(bool visible);
    void setAlpha(float a);
    
    bool isPointInside(float x, float y) const;
    const float* getPosition() const { return position; }
    const float* getSize() const { return size; }
    bool getVisible() const { return isVisible; }
    
    // Virtual event handlers
    virtual bool handleKeyEvent(unsigned char key) { return false; }
    virtual bool handleMouseMove(int x, int y) { return false; }
};

// Image component
class Image : public UIComponent {
private:
    unsigned char* imageData;
    int width;
    int height;
    int channels;
    unsigned int textureId;
    bool preserveAspectRatio;
    float tint[4];  // RGBA tint color

public:
    Image(const std::string& imagePath);
    ~Image() override;
    
    void render() override;
    
    bool loadImage(const std::string& imagePath);
    void setTint(float r, float g, float b, float a = 1.0f);
    void setPreserveAspectRatio(bool preserve);
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
};

// BorderBox component
class BorderBox : public UIComponent {
private:
    UIComponent* content;
    float borderWidth;
    float borderColor[4];  // RGBA
    float backgroundColor[4]; // RGBA
    
    // Helper method to update content position
    void updateContentPosition();

public:
    BorderBox(UIComponent* component, float border = 2.0f, float pad = 5.0f);
    ~BorderBox() override;
    
    void render() override;
    void update(float deltaTime) override;
    
    void setContent(UIComponent* component);
    UIComponent* getContent() const { return content; }
    
    void setBorderWidth(float width);
    void setBorderColor(float r, float g, float b, float a = 1.0f);
    void setBackgroundColor(float r, float g, float b, float a = 1.0f);
    
    // Override position and size setters to update the content position
    void setPosition(float x, float y);
    void setSize(float width, float height);
    
    // Event forwarding methods
    bool handleMouseEvent(int button, int state, int x, int y);
    bool handleKeyEvent(unsigned char key);
    bool handleMouseMove(int x, int y);
};

// Button component
class Button : public UIComponent {
private:
    std::string text;
    float textColor[4];     // RGBA
    float backgroundColor[4]; // RGBA
    float hoverColor[4];    // RGBA
    bool isHovered;
    bool isPressed;
    std::function<void()> onClick;

public:
    Button(const std::string& buttonText);
    ~Button() override = default;
    
    void render() override;
    void update(float deltaTime) override;
    
    void setText(const std::string& buttonText);
    void setOnClick(const std::function<void()>& callback);
    void setTextColor(float r, float g, float b, float a = 1.0f);
    void setBackgroundColor(float r, float g, float b, float a = 1.0f);
    void setHoverColor(float r, float g, float b, float a = 1.0f);
    
    bool handleMouseEvent(int button, int state, int x, int y);
    bool handleMouseMove(int x, int y);
};

// TextBox component
class TextBox : public UIComponent {
private:
    std::string text;
    std::string placeholder;
    float textColor[4];         // RGBA
    float backgroundColor[4];   // RGBA
    float borderColor[4];       // RGBA
    bool isEditable;
    bool hasFocus;
    int cursorPosition;
    float cursorBlinkTime;
    bool showCursor;

public:
    TextBox(const std::string& initialText = "", const std::string& placeholderText = "");
    ~TextBox() override = default;
    
    void render() override;
    void update(float deltaTime) override;
    
    void setText(const std::string& newText);
    const std::string& getText() const;
    void setPlaceholder(const std::string& newPlaceholder);
    void setEditable(bool editable);
    void setTextColor(float r, float g, float b, float a = 1.0f);
    void setBackgroundColor(float r, float g, float b, float a = 1.0f);
    void setBorderColor(float r, float g, float b, float a = 1.0f);
    
    bool handleKeyEvent(unsigned char key);
    bool handleMouseEvent(int button, int state, int x, int y);
};

// Timer display component
class Timer : public UIComponent {
private:
    std::string timeText;
    float currentTime;
    bool isRunning;
    float textColor[4];  // RGBA
    
    // Add private helper method to update time text
    void updateTimeText();

public:
    Timer();
    ~Timer() override = default;
    
    void start();
    void stop();
    void reset();
    void update(float deltaTime) override;
    void render() override;
    float getTime() const;
    void setTextColor(float r, float g, float b, float a = 1.0f);
}; 