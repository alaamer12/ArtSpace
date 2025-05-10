#pragma once
#include <GL/glut.h>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <functional>
#include <fstream>
#include <map>

 // Forward declarations - don't include the full headers
class Config;


class UIComponent;

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
    sf::Image sfImage;
    unsigned int textureId;
    bool preserveAspectRatio;
    float tint[4];  // RGBA tint color
    bool imageLoaded;
    float fallbackColor[4];  // RGBA fallback color
    bool useFallback;

public:
    Image(const std::string& imagePath, const std::string& fallbackColorHex = "#ffffff");
    ~Image() override;

    void render() override;

    bool loadImage(const std::string& imagePath);
    void setTint(float r, float g, float b, float a = 1.0f);
    void setPreserveAspectRatio(bool preserve);
    void setFallbackColor(float r, float g, float b, float a = 1.0f);
    void setFallbackColor(const std::string& hexColor);

    bool isImageLoaded() const { return imageLoaded; }
    int getWidth() const { return sfImage.getSize().x; }
    int getHeight() const { return sfImage.getSize().y; }

    // Utility functions
    static void hexToRGB(const std::string& hexColor, float& r, float& g, float& b);
};