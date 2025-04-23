#include "config.h"
#include <algorithm>
#include <iostream>
#include <fstream>

// Initialize static instance
Config Config::instance;

Config::Config()
    : displaySettings{1024, 768, false}
    , cameraSettings{0.5f, 5.0f, 3.0f}
    , gameplaySettings{45.0f, "assets/"} {
}

// Singleton access
Config& Config::getInstance() {
    return instance;
}

Config::~Config() {
}

// Display settings
int Config::getScreenWidth() const { 
    return displaySettings.screenWidth; 
}

void Config::setScreenWidth(int width) { 
    displaySettings.screenWidth = validateScreenWidth(width); 
}

int Config::getScreenHeight() const { 
    return displaySettings.screenHeight; 
}

void Config::setScreenHeight(int height) { 
    displaySettings.screenHeight = validateScreenHeight(height); 
}

bool Config::isFullscreen() const { 
    return displaySettings.fullscreen; 
}

void Config::setFullscreen(bool enable) { 
    displaySettings.fullscreen = enable; 
}

// Camera settings
float Config::getMouseSensitivity() const { 
    return cameraSettings.mouseSensitivity; 
}

void Config::setMouseSensitivity(float sensitivity) { 
    cameraSettings.mouseSensitivity = validateMouseSensitivity(sensitivity); 
}

float Config::getMoveSpeed() const { 
    return cameraSettings.moveSpeed; 
}

void Config::setMoveSpeed(float speed) { 
    cameraSettings.moveSpeed = validateMoveSpeed(speed); 
}

float Config::getInteractionDistance() const { 
    return cameraSettings.interactionDistance; 
}

void Config::setInteractionDistance(float distance) { 
    cameraSettings.interactionDistance = validateInteractionDistance(distance); 
}

// Gameplay settings
float Config::getRotationStep() const { 
    return gameplaySettings.rotationStep; 
}

void Config::setRotationStep(float step) { 
    gameplaySettings.rotationStep = validateRotationStep(step); 
}

const std::string& Config::getAssetPath() const {
    return gameplaySettings.assetPath;
}

void Config::setAssetPath(const std::string& path) {
    gameplaySettings.assetPath = path;
    Logger::getInstance().logInfo("Asset path set to: " + path);
}

// Validation methods that enforce limits
int Config::validateScreenWidth(int width) const {
    if (width < MIN_SCREEN_WIDTH) {
        Logger::getInstance().logWarning("Screen width " + std::to_string(width) + 
                                          " is below minimum. Using minimum value: " + 
                                          std::to_string(MIN_SCREEN_WIDTH));
        return MIN_SCREEN_WIDTH;
    }
    else if (width > MAX_SCREEN_WIDTH) {
        Logger::getInstance().logWarning("Screen width " + std::to_string(width) + 
                                          " exceeds maximum. Using maximum value: " + 
                                          std::to_string(MAX_SCREEN_WIDTH));
        return MAX_SCREEN_WIDTH;
    }
    return width;
}

int Config::validateScreenHeight(int height) const {
    if (height < MIN_SCREEN_HEIGHT) {
        Logger::getInstance().logWarning("Screen height " + std::to_string(height) + 
                                          " is below minimum. Using minimum value: " + 
                                          std::to_string(MIN_SCREEN_HEIGHT));
        return MIN_SCREEN_HEIGHT;
    }
    else if (height > MAX_SCREEN_HEIGHT) {
        Logger::getInstance().logWarning("Screen height " + std::to_string(height) + 
                                          " exceeds maximum. Using maximum value: " + 
                                          std::to_string(MAX_SCREEN_HEIGHT));
        return MAX_SCREEN_HEIGHT;
    }
    return height;
}

float Config::validateMouseSensitivity(float sensitivity) const {
    if (sensitivity < MIN_MOUSE_SENSITIVITY) {
        Logger::getInstance().logWarning("Mouse sensitivity " + std::to_string(sensitivity) + 
                                          " is below minimum. Using minimum value: " + 
                                          std::to_string(MIN_MOUSE_SENSITIVITY));
        return MIN_MOUSE_SENSITIVITY;
    }
    else if (sensitivity > MAX_MOUSE_SENSITIVITY) {
        Logger::getInstance().logWarning("Mouse sensitivity " + std::to_string(sensitivity) + 
                                          " exceeds maximum. Using maximum value: " + 
                                          std::to_string(MAX_MOUSE_SENSITIVITY));
        return MAX_MOUSE_SENSITIVITY;
    }
    return sensitivity;
}

float Config::validateMoveSpeed(float speed) const {
    if (speed < MIN_MOVE_SPEED) {
        Logger::getInstance().logWarning("Move speed " + std::to_string(speed) + 
                                          " is below minimum. Using minimum value: " + 
                                          std::to_string(MIN_MOVE_SPEED));
        return MIN_MOVE_SPEED;
    }
    else if (speed > MAX_MOVE_SPEED) {
        Logger::getInstance().logWarning("Move speed " + std::to_string(speed) + 
                                          " exceeds maximum. Using maximum value: " + 
                                          std::to_string(MAX_MOVE_SPEED));
        return MAX_MOVE_SPEED;
    }
    return speed;
}

float Config::validateInteractionDistance(float distance) const {
    if (distance < MIN_INTERACTION_DISTANCE) {
        Logger::getInstance().logWarning("Interaction distance " + std::to_string(distance) + 
                                         " is below minimum. Using minimum value: " + 
                                         std::to_string(MIN_INTERACTION_DISTANCE));
        return MIN_INTERACTION_DISTANCE;
    }
    else if (distance > MAX_INTERACTION_DISTANCE) {
        Logger::getInstance().logWarning("Interaction distance " + std::to_string(distance) + 
                                         " exceeds maximum. Using maximum value: " + 
                                         std::to_string(MAX_INTERACTION_DISTANCE));
        return MAX_INTERACTION_DISTANCE;
    }
    return distance;
}

float Config::validateRotationStep(float step) const {
    if (step < MIN_ROTATION_STEP) {
        Logger::getInstance().logWarning("Rotation step " + std::to_string(step) + 
                                         " is below minimum. Using minimum value: " + 
                                         std::to_string(MIN_ROTATION_STEP));
        return MIN_ROTATION_STEP;
    }
    else if (step > MAX_ROTATION_STEP) {
        Logger::getInstance().logWarning("Rotation step " + std::to_string(step) + 
                                         " exceeds maximum. Using maximum value: " + 
                                         std::to_string(MAX_ROTATION_STEP));
        return MAX_ROTATION_STEP;
    }
    return step;
}

// Configuration management
void Config::applyOptimalSettings() {
    // These won't trigger warnings since they're within limits
    setMouseSensitivity(0.15f);
    setMoveSpeed(6.0f);
    setInteractionDistance(2.5f);
    setRotationStep(45.0f);
    
    Logger::getInstance().logInfo("Applied optimal settings for single-level game");
}

void Config::resetToDefaults() {
    displaySettings.screenWidth = 1280;
    displaySettings.screenHeight = 720;
    displaySettings.fullscreen = false;
    cameraSettings.mouseSensitivity = 0.1f;
    cameraSettings.moveSpeed = 5.0f;
    cameraSettings.interactionDistance = 2.0f;
    gameplaySettings.rotationStep = 90.0f;
    gameplaySettings.assetPath = "assets/";
    
    Logger::getInstance().logInfo("Reset all settings to defaults");
}

bool Config::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        Logger::getInstance().logError("Failed to open config file: " + filename);
        return false;
    }

    // Simple key-value parsing
    std::string line;
    while (std::getline(file, line)) {
        // Skip comments and empty lines
        if (line.empty() || line[0] == '#') continue;

        size_t delimPos = line.find('=');
        if (delimPos == std::string::npos) continue;

        std::string key = line.substr(0, delimPos);
        std::string value = line.substr(delimPos + 1);

        // Trim whitespace
        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);

        // Parse values
        if (key == "screenWidth") {
            setScreenWidth(std::stoi(value));
        } else if (key == "screenHeight") {
            setScreenHeight(std::stoi(value));
        } else if (key == "fullscreen") {
            setFullscreen(value == "true" || value == "1");
        } else if (key == "mouseSensitivity") {
            setMouseSensitivity(std::stof(value));
        } else if (key == "moveSpeed") {
            setMoveSpeed(std::stof(value));
        } else if (key == "interactionDistance") {
            setInteractionDistance(std::stof(value));
        } else if (key == "rotationStep") {
            setRotationStep(std::stof(value));
        } else if (key == "assetPath") {
            setAssetPath(value);
        }
    }

    file.close();
    Logger::getInstance().logInfo("Config loaded from file: " + filename);
    return true;
}

bool Config::saveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        Logger::getInstance().logError("Failed to open config file for writing: " + filename);
        return false;
    }

    file << "# ArtSpace Configuration File\n";
    file << "# Generated automatically\n\n";

    // Display settings
    file << "screenWidth=" << displaySettings.screenWidth << "\n";
    file << "screenHeight=" << displaySettings.screenHeight << "\n";
    file << "fullscreen=" << (displaySettings.fullscreen ? "true" : "false") << "\n\n";

    // Camera settings
    file << "mouseSensitivity=" << cameraSettings.mouseSensitivity << "\n";
    file << "moveSpeed=" << cameraSettings.moveSpeed << "\n";
    file << "interactionDistance=" << cameraSettings.interactionDistance << "\n\n";

    // Gameplay settings
    file << "rotationStep=" << gameplaySettings.rotationStep << "\n";
    file << "assetPath=" << gameplaySettings.assetPath << "\n";

    file.close();
    Logger::getInstance().logInfo("Config saved to file: " + filename);
    return true;
}
