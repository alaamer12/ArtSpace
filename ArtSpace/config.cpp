#include "config.h"

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