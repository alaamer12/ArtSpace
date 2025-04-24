#include <SFML/Graphics.hpp>
#include "utility.h"
#include <GL/glut.h>
#include <cstdio>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>  // For std::stringstream

// Initialize static instance for Logger
Logger Logger::instance;

// Logger implementation
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

// Text rendering implementations
void calculateTextPosition(float boxX, float boxY, float boxWidth, float boxHeight,
    const std::string& text, TextAlignment alignment,
    float& outX, float& outY) {
    // Calculate text width using GLUT
    int textWidth = 0;
    for (char c : text) {
        textWidth += glutBitmapWidth(GLUT_BITMAP_HELVETICA_12, c);
    }

    // Calculate x position based on alignment
    switch (alignment) {
    case TextAlignment::MidLeft:
        outX = boxX + 5; // Small padding from left edge
        break;
    case TextAlignment::MidMid:
        outX = boxX + (boxWidth - textWidth) / 2;
        break;
    case TextAlignment::MidRight:
        outX = boxX + boxWidth - textWidth - 5; // Small padding from right edge
        break;
    }

    // Calculate y position (vertically centered)
    outY = boxY + (boxHeight - 12) / 2; // Assuming font height of 12 pixels
}

void renderText(float x, float y, const std::string& text, float* color) {
    glPushAttrib(GL_CURRENT_BIT); // Save current color

    if (color) {
        glColor4f(color[0], color[1], color[2], color[3]);
    }
    else {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // Default: white
    }

    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }

    glPopAttrib(); // Restore previous color
}

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
    textColor[0] = 1.0f;
    textColor[1] = 1.0f;
    textColor[2] = 1.0f;
    textColor[3] = 1.0f;

    backgroundColor[0] = 0.3f;
    backgroundColor[1] = 0.3f;
    backgroundColor[2] = 0.7f;
    backgroundColor[3] = 0.8f;

    hoverColor[0] = 0.4f;
    hoverColor[1] = 0.4f;
    hoverColor[2] = 0.8f;
    hoverColor[3] = 0.8f;
}

void Button::render() {
    if (!isVisible) return;

    // Draw button background
    glBegin(GL_QUADS);
    if (isHovered) {
        glColor4f(hoverColor[0], hoverColor[1], hoverColor[2], hoverColor[3] * alpha);
    }
    else {
        glColor4f(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3] * alpha);
    }

    glVertex2f(position[0], position[1]);
    glVertex2f(position[0] + size[0], position[1]);
    glVertex2f(position[0] + size[0], position[1] + size[1]);
    glVertex2f(position[0], position[1] + size[1]);
    glEnd();

    // Draw button text
    renderText(position[0], position[1], size[0], size[1], text, TextAlignment::MidMid, textColor);
}

void Button::update(float deltaTime) {
    // Nothing to update for basic button
}

void Button::setText(const std::string& buttonText) {
    text = buttonText;
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
    // Flip y-coordinate (GLUT has origin at top-left, we use bottom-left)
    y = glutGet(GLUT_WINDOW_HEIGHT) - y;

    if (!isVisible) return false;

    // Check if mouse is inside the button
    bool inside = x >= position[0] && x <= position[0] + size[0] &&
        y >= position[1] && y <= position[1] + size[1];

    if (inside) {
        if (button == GLUT_LEFT_BUTTON) {
            if (state == GLUT_DOWN) {
                isPressed = true;
            }
            else if (state == GLUT_UP && isPressed) {
                isPressed = false;
                if (onClick) {
                    onClick(); // Execute callback
                    return true;
                }
            }
        }
    }
    else {
        isPressed = false;
    }

    return inside;
}

bool Button::handleMouseMove(int x, int y) {
    // Flip y-coordinate
    y = glutGet(GLUT_WINDOW_HEIGHT) - y;

    if (!isVisible) return false;

    // Check if mouse is inside the button
    bool inside = x >= position[0] && x <= position[0] + size[0] &&
        y >= position[1] && y <= position[1] + size[1];

    isHovered = inside;

    return inside;
}

// TextBox implementation
TextBox::TextBox(const std::string& initialText, const std::string& placeholderText)
    : text(initialText)
    , placeholder(placeholderText)
    , isEditable(true)
    , hasFocus(false)
    , cursorPosition(0)
    , cursorBlinkTime(0)
    , showCursor(false) {
    // Default colors
    textColor[0] = 0.0f;
    textColor[1] = 0.0f;
    textColor[2] = 0.0f;
    textColor[3] = 1.0f;

    backgroundColor[0] = 0.9f;
    backgroundColor[1] = 0.9f;
    backgroundColor[2] = 0.9f;
    backgroundColor[3] = 0.8f;

    borderColor[0] = 0.5f;
    borderColor[1] = 0.5f;
    borderColor[2] = 0.5f;
    borderColor[3] = 1.0f;

    if (!text.empty()) {
        cursorPosition = text.length();
    }
}

void TextBox::render() {
    if (!isVisible) return;

    // Draw background
    glBegin(GL_QUADS);
    glColor4f(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3] * alpha);
    glVertex2f(position[0], position[1]);
    glVertex2f(position[0] + size[0], position[1]);
    glVertex2f(position[0] + size[0], position[1] + size[1]);
    glVertex2f(position[0], position[1] + size[1]);
    glEnd();

    // Draw border
    glBegin(GL_LINE_LOOP);
    glColor4f(borderColor[0], borderColor[1], borderColor[2], borderColor[3] * alpha);
    glVertex2f(position[0], position[1]);
    glVertex2f(position[0] + size[0], position[1]);
    glVertex2f(position[0] + size[0], position[1] + size[1]);
    glVertex2f(position[0], position[1] + size[1]);
    glEnd();

    // Draw text or placeholder
    if (!text.empty()) {
        renderText(position[0] + 5, position[1], size[0] - 10, size[1], text, TextAlignment::MidLeft, textColor);
    }
    else if (!placeholder.empty() && !hasFocus) {
        float placeholderColor[4] = {
            textColor[0] * 0.7f,
            textColor[1] * 0.7f,
            textColor[2] * 0.7f,
            textColor[3] * 0.7f
        };
        renderText(position[0] + 5, position[1], size[0] - 10, size[1], placeholder, TextAlignment::MidLeft, placeholderColor);
    }

    // Draw cursor if focused
    if (hasFocus && isEditable && showCursor) {
        // Calculate cursor position
        int cursorX = position[0] + 5; // Start padding
        for (int i = 0; i < cursorPosition; i++) {
            cursorX += glutBitmapWidth(GLUT_BITMAP_HELVETICA_12, text[i]);
        }

        // Draw cursor
        glBegin(GL_LINES);
        glColor4f(textColor[0], textColor[1], textColor[2], textColor[3] * alpha);
        glVertex2f(cursorX, position[1] + 5);
        glVertex2f(cursorX, position[1] + size[1] - 5);
        glEnd();
    }
}

void TextBox::update(float deltaTime) {
    if (hasFocus && isEditable) {
        cursorBlinkTime += deltaTime;
        if (cursorBlinkTime > 0.5f) {
            cursorBlinkTime = 0;
            showCursor = !showCursor;
        }
    }
    else {
        showCursor = false;
    }
}

void TextBox::setText(const std::string& newText) {
    text = newText;
    if (cursorPosition > text.length()) {
        cursorPosition = text.length();
    }
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

    // Process key input
    switch (key) {
    case 8: // Backspace
        if (cursorPosition > 0) {
            text.erase(cursorPosition - 1, 1);
            cursorPosition--;
        }
        break;

    case 13: // Enter
        // Just lose focus for now
        hasFocus = false;
        break;

    case 27: // Escape
        hasFocus = false;
        break;

    default:
        // Only handle printable characters
        if (key >= 32 && key <= 126) {
            text.insert(cursorPosition, 1, key);
            cursorPosition++;
        }
        break;
    }

    // Reset cursor blink
    cursorBlinkTime = 0;
    showCursor = true;

    return true;
}

bool TextBox::handleMouseEvent(int button, int state, int x, int y) {
    // Flip y-coordinate
    y = glutGet(GLUT_WINDOW_HEIGHT) - y;

    if (!isVisible) return false;

    // Check if mouse is inside the text box
    bool inside = x >= position[0] && x <= position[0] + size[0] &&
        y >= position[1] && y <= position[1] + size[1];

    if (inside && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        hasFocus = true;

        // Set cursor position based on click position
        if (isEditable) {
            int clickX = x - (position[0] + 5); // Adjust for padding
            if (clickX <= 0) {
                cursorPosition = 0;
            }
            else {
                // Find the closest character to the click position
                int textWidth = 0;
                for (cursorPosition = 0; cursorPosition < text.length(); cursorPosition++) {
                    textWidth += glutBitmapWidth(GLUT_BITMAP_HELVETICA_12, text[cursorPosition]);
                    if (textWidth >= clickX) break;
                }
            }

            // Reset cursor blink
            cursorBlinkTime = 0;
            showCursor = true;
        }

        return true;
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Click outside the text box
        hasFocus = false;
        return false;
    }

    return inside;
}

// Timer implementation
Timer::Timer()
    : currentTime(0)
    , isRunning(false) {
    // Default text color: white
    textColor[0] = 1.0f;
    textColor[1] = 1.0f;
    textColor[2] = 1.0f;
    textColor[3] = 1.0f;

    updateTimeText();
}

void Timer::start() {
    isRunning = true;
}

void Timer::stop() {
    isRunning = false;
}

void Timer::reset() {
    currentTime = 0;
    updateTimeText();
}

void Timer::update(float deltaTime) {
    if (isRunning) {
        currentTime += deltaTime;
        updateTimeText();
    }
}

void Timer::render() {
    if (!isVisible) return;

    // Just render the time text
    renderText(position[0], position[1], size[0], size[1],
        timeText, TextAlignment::MidMid, textColor);
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

void Timer::updateTimeText() {
    int minutes = static_cast<int>(currentTime) / 60;
    int seconds = static_cast<int>(currentTime) % 60;
    int millisecs = static_cast<int>((currentTime - static_cast<int>(currentTime)) * 100);

    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << minutes << ":"
        << std::setfill('0') << std::setw(2) << seconds << "."
        << std::setfill('0') << std::setw(2) << millisecs;

    timeText = ss.str();
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

// BorderBox implementation
BorderBox::BorderBox(UIComponent* component, float border, float pad)
    : content(component)
    , borderWidth(border) {
    // Default colors
    borderColor[0] = 0.5f;
    borderColor[1] = 0.5f;
    borderColor[2] = 0.5f;
    borderColor[3] = 1.0f;

    backgroundColor[0] = 0.2f;
    backgroundColor[1] = 0.2f;
    backgroundColor[2] = 0.2f;
    backgroundColor[3] = 0.8f;

    // If content exists, adjust size to fit content plus padding
    if (content) {
        const float* contentSize = content->getSize();
        size[0] = contentSize[0] + 2 * (borderWidth + pad);
        size[1] = contentSize[1] + 2 * (borderWidth + pad);
        updateContentPosition();
    }
}

BorderBox::~BorderBox() {
    // BorderBox doesn't own the content, so don't delete it
}

void BorderBox::render() {
    if (!isVisible) return;

    // Draw background
    glBegin(GL_QUADS);
    glColor4f(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3] * alpha);
    glVertex2f(position[0], position[1]);
    glVertex2f(position[0] + size[0], position[1]);
    glVertex2f(position[0] + size[0], position[1] + size[1]);
    glVertex2f(position[0], position[1] + size[1]);
    glEnd();

    // Draw border
    glLineWidth(borderWidth);
    glBegin(GL_LINE_LOOP);
    glColor4f(borderColor[0], borderColor[1], borderColor[2], borderColor[3] * alpha);
    glVertex2f(position[0], position[1]);
    glVertex2f(position[0] + size[0], position[1]);
    glVertex2f(position[0] + size[0], position[1] + size[1]);
    glVertex2f(position[0], position[1] + size[1]);
    glEnd();
    glLineWidth(1.0f); // Reset line width

    // Render content
    if (content && content->getVisible()) {
        content->render();
    }
}

void BorderBox::update(float deltaTime) {
    if (content) {
        content->update(deltaTime);
    }
}

void BorderBox::setContent(UIComponent* component) {
    content = component;
    updateContentPosition();
}

void BorderBox::updateContentPosition() {
    if (content) {
        // Center content in the border box
        const float* contentSize = content->getSize();
        float contentX = position[0] + (size[0] - contentSize[0]) / 2;
        float contentY = position[1] + (size[1] - contentSize[1]) / 2;
        content->setPosition(contentX, contentY);
    }
}

void BorderBox::setBorderWidth(float width) {
    borderWidth = width;
    updateContentPosition();
}

void BorderBox::setBorderColor(float r, float g, float b, float a) {
    borderColor[0] = r;
    borderColor[1] = g;
    borderColor[2] = b;
    borderColor[3] = a;
}

void BorderBox::setBackgroundColor(float r, float g, float b, float a) {
    backgroundColor[0] = r;
    backgroundColor[1] = g;
    backgroundColor[2] = b;
    backgroundColor[3] = a;
}

void BorderBox::setPosition(float x, float y) {
    position[0] = x;
    position[1] = y;
    updateContentPosition();
}

void BorderBox::setSize(float width, float height) {
    size[0] = width;
    size[1] = height;
    updateContentPosition();
}

bool BorderBox::handleMouseEvent(int button, int state, int x, int y) {
    // Flip y-coordinate
    y = glutGet(GLUT_WINDOW_HEIGHT) - y;

    if (!isVisible) return false;

    // Check if mouse is inside
    bool inside = x >= position[0] && x <= position[0] + size[0] &&
        y >= position[1] && y <= position[1] + size[1];

    // If inside and we have content, forward the event
    if (inside && content) {
        // Check if content can handle mouse events
        Button* buttonContent = dynamic_cast<Button*>(content);
        if (buttonContent) {
            return buttonContent->handleMouseEvent(button, state, x, y);
        }

        TextBox* textboxContent = dynamic_cast<TextBox*>(content);
        if (textboxContent) {
            return textboxContent->handleMouseEvent(button, state, x, y);
        }

        // Try another BorderBox
        BorderBox* borderboxContent = dynamic_cast<BorderBox*>(content);
        if (borderboxContent) {
            return borderboxContent->handleMouseEvent(button, state, x, y);
        }
    }

    return inside;
}

bool BorderBox::handleKeyEvent(unsigned char key) {
    if (!isVisible) return false;

    // Forward to content if possible
    if (content) {
        TextBox* textboxContent = dynamic_cast<TextBox*>(content);
        if (textboxContent) {
            return textboxContent->handleKeyEvent(key);
        }

        // Try another BorderBox
        BorderBox* borderboxContent = dynamic_cast<BorderBox*>(content);
        if (borderboxContent) {
            return borderboxContent->handleKeyEvent(key);
        }
    }

    return false;
}

bool BorderBox::handleMouseMove(int x, int y) {
    // Flip y-coordinate
    y = glutGet(GLUT_WINDOW_HEIGHT) - y;

    if (!isVisible) return false;

    // Check if mouse is inside
    bool inside = x >= position[0] && x <= position[0] + size[0] &&
        y >= position[1] && y <= position[1] + size[1];

    // If inside and we have content, forward the event
    if (inside && content) {
        // Check if content can handle mouse move
        Button* buttonContent = dynamic_cast<Button*>(content);
        if (buttonContent) {
            return buttonContent->handleMouseMove(x, y);
        }

        // Try another BorderBox
        BorderBox* borderboxContent = dynamic_cast<BorderBox*>(content);
        if (borderboxContent) {
            return borderboxContent->handleMouseMove(x, y);
        }
    }

    return inside;
}
