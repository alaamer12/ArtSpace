#include "utility.h"



Logger Logger::instance;

Logger::Logger()
    : consoleLogging(false)
    , fileLogging(false)
    , logFilePath("artspace.log")
    , minLevel(INFO) {
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

    if (fileLogging && !logFile.is_open()) {
        logFile.open(logFilePath, std::ios::out | std::ios::app);
        if (!logFile.is_open()) {
            // Failed to open log file
            consoleLogging = true; // Fallback to console logging
            logError("Failed to open log file: " + logFilePath);
        }
    }
    else if (!fileLogging && logFile.is_open()) {
        logFile.close();
    }
}

void Logger::setMinimumLogLevel(LogLevel level) {
    minLevel = level;
}

void Logger::logInfo(const std::string& message) {
    log(INFO, message);
}

void Logger::logWarning(const std::string& message) {
    log(WARNING, message);
}

void Logger::logError(const std::string& message) {
    log(ERROR_, message);
}

void Logger::logDebug(const std::string& message) {
    log(DEBUG, message);
}

void Logger::log(LogLevel level, const std::string& message) {
    if (level < minLevel) {
        return; // Skip messages below minimum level
    }

    // Get current time
    std::time_t now = std::time(nullptr);
    struct tm timeInfo;
    localtime_s(&timeInfo, &now);

    std::stringstream logStream;
    logStream << "["
        << std::setfill('0') << std::setw(2) << timeInfo.tm_hour << ":"
        << std::setfill('0') << std::setw(2) << timeInfo.tm_min << ":"
        << std::setfill('0') << std::setw(2) << timeInfo.tm_sec << "] ";

    // Add level prefix
    switch (level) {
    case INFO:    logStream << "[INFO] "; break;
    case WARNING: logStream << "[WARN] "; break;
    case DEBUG:   logStream << "[DEBUG] "; break;
    case ERROR_:  logStream << "[ERROR] "; break;
    }

    logStream << message;

    if (consoleLogging) {
        std::cout << logStream.str() << std::endl;
    }

    if (fileLogging && logFile.is_open()) {
        logFile << logStream.str() << std::endl;
        logFile.flush();
    }
}

// UIComponent implementation
UIComponent::UIComponent()
    : position{ 0, 0 }
    , size{ 100, 50 }
    , isVisible(true)
    , alpha(1.0f) {
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
    alpha = a;
    if (alpha < 0.0f) alpha = 0.0f;
    if (alpha > 1.0f) alpha = 1.0f;
}

bool UIComponent::isPointInside(float x, float y) const {
    return (x >= position[0] && x <= position[0] + size[0] &&
        y >= position[1] && y <= position[1] + size[0]);
}

// Image implementation
Image::Image(const std::string& imagePath, const std::string& fallbackColorHex)
    : textureId(0)
    , preserveAspectRatio(true)
    , imageLoaded(false)
    , useFallback(false) {
    // Set default tint (white, no tint)
    tint[0] = 1.0f;
    tint[1] = 1.0f;
    tint[2] = 1.0f;
    tint[3] = 1.0f;

    // Set default fallback color (white)
    setFallbackColor(fallbackColorHex);

    // Try to load the image
    if (!loadImage(imagePath)) {
        if (fallbackColorHex.empty()) {
            // No fallback color specified and image load failed
            Logger::getInstance().logError("Failed to load image: " + imagePath + " and no fallback color specified");
        }
        else {
            // Use fallback color
            useFallback = true;
            Logger::getInstance().logWarning("Using fallback color for failed image: " + imagePath);
        }
    }
}

Image::~Image() {
    if (textureId) {
        glDeleteTextures(1, &textureId);
    }
}

bool Image::loadImage(const std::string& imagePath) {
    // Reset status
    imageLoaded = false;
    useFallback = false;

    // Load the image using SFML
    if (!sfImage.loadFromFile(imagePath)) {
        Logger::getInstance().logWarning("Failed to load image: " + imagePath);
        return false;
    }

    // Flip image for OpenGL
    sfImage.flipVertically();

    // Set size to match image dimensions initially
    size[0] = static_cast<float>(sfImage.getSize().x);
    size[1] = static_cast<float>(sfImage.getSize().y);

    // Create OpenGL texture
    if (textureId) {
        glDeleteTextures(1, &textureId);
    }

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload texture data directly from SFML image
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        sfImage.getSize().x,
        sfImage.getSize().y,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        sfImage.getPixelsPtr()
    );

    // Check for OpenGL errors
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        Logger::getInstance().logError("OpenGL error loading texture: " + std::to_string(err));
        return false;
    }

    imageLoaded = true;
    useFallback = false;

    Logger::getInstance().logInfo("Loaded image: " + imagePath + " - " +
        std::to_string(sfImage.getSize().x) + "x" +
        std::to_string(sfImage.getSize().y));

    return true;
}

void Image::render() {
    if (!isVisible) return;

    float w = size[0];
    float h = size[1];

    if (imageLoaded && !useFallback) {
        // Adjust dimensions if preserving aspect ratio
        if (preserveAspectRatio && sfImage.getSize().x > 0 && sfImage.getSize().y > 0) {
            float imageAspect = static_cast<float>(sfImage.getSize().x) / sfImage.getSize().y;
            float boxAspect = w / h;
            float tempX = position[0];
            float tempY = position[1];

            if (imageAspect > boxAspect) {
                // Image is wider than box
                float newHeight = w / imageAspect;
                float offsetY = (h - newHeight) / 2;
                h = newHeight;
                tempY += offsetY;
            }
            else {
                // Image is taller than box
                float newWidth = h * imageAspect;
                float offsetX = (w - newWidth) / 2;
                w = newWidth;
                tempX += offsetX;
            }

            // Enable texturing
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, textureId);

            // Apply tint color
            glColor4f(tint[0], tint[1], tint[2], tint[3] * alpha);

            // Draw textured quad
            glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex2f(tempX, tempY);
            glTexCoord2f(1, 0); glVertex2f(tempX + w, tempY);
            glTexCoord2f(1, 1); glVertex2f(tempX + w, tempY + h);
            glTexCoord2f(0, 1); glVertex2f(tempX, tempY + h);
            glEnd();

            // Restore state
            glDisable(GL_TEXTURE_2D);
        }
        else {
            // No aspect ratio preservation
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, textureId);

            // Apply tint color
            glColor4f(tint[0], tint[1], tint[2], tint[3] * alpha);

            // Draw textured quad
            glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex2f(position[0], position[1]);
            glTexCoord2f(1, 0); glVertex2f(position[0] + w, position[1]);
            glTexCoord2f(1, 1); glVertex2f(position[0] + w, position[1] + h);
            glTexCoord2f(0, 1); glVertex2f(position[0], position[1] + h);
            glEnd();

            // Restore state
            glDisable(GL_TEXTURE_2D);
        }
    }
    else if (useFallback) {
        // Render a solid rectangle with the fallback color
        glColor4f(fallbackColor[0], fallbackColor[1], fallbackColor[2], fallbackColor[3] * alpha);

        glBegin(GL_QUADS);
        glVertex2f(position[0], position[1]);
        glVertex2f(position[0] + w, position[1]);
        glVertex2f(position[0] + w, position[1] + h);
        glVertex2f(position[0], position[1] + h);
        glEnd();
    }
}

void Image::setTint(float r, float g, float b, float a) {
    tint[0] = r;
    tint[1] = g;
    tint[2] = b;
    tint[3] = a;
}

void Image::setPreserveAspectRatio(bool preserve) {
    preserveAspectRatio = preserve;
}

void Image::setFallbackColor(float r, float g, float b, float a) {
    fallbackColor[0] = r;
    fallbackColor[1] = g;
    fallbackColor[2] = b;
    fallbackColor[3] = a;
}

void Image::setFallbackColor(const std::string& hexColor) {
    if (hexColor.empty()) {
        // No fallback color
        useFallback = false;
        return;
    }

    float r, g, b;
    hexToRGB(hexColor, r, g, b);
    setFallbackColor(r, g, b, 1.0f);
}

void Image::hexToRGB(const std::string& hexColor, float& r, float& g, float& b) {
    // Default to white
    r = g = b = 1.0f;

    // Check for valid hex color format
    if (hexColor.empty() || hexColor[0] != '#') {
        return;
    }

    std::string hex = hexColor.substr(1); // Remove #

    // Support both 3-digit and 6-digit hex codes
    if (hex.length() == 3) {
        // Expand 3-digit hex to 6-digit
        hex = std::string(1, hex[0]) + std::string(1, hex[0]) +
            std::string(1, hex[1]) + std::string(1, hex[1]) +
            std::string(1, hex[2]) + std::string(1, hex[2]);
    }

    if (hex.length() != 6) {
        return; // Invalid format
    }

    // Convert hex to RGB
    try {
        unsigned int hexValue = std::stoul(hex, nullptr, 16);
        r = ((hexValue >> 16) & 0xFF) / 255.0f;
        g = ((hexValue >> 8) & 0xFF) / 255.0f;
        b = (hexValue & 0xFF) / 255.0f;
    }
    catch (const std::exception& e) {
        Logger::getInstance().logError("Failed to parse hex color: " + hexColor + " - " + e.what());
    }
}
