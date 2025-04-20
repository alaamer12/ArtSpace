#include "config.h"

Config* Config::instance = nullptr;

Config::Config()
    : screenWidth(1280)
    , screenHeight(720)
    , fullscreen(false)
    , mouseSensitivity(0.1f)
    , moveSpeed(5.0f)
    , interactionDistance(2.0f)
    , rotationStep(90.0f)
    , assetPath("assets/") {
}

Config* Config::getInstance() {
    if (instance == nullptr) {
        instance = new Config();
    }
    return instance;
}

Config::~Config() {
}

// Display settings
int Config::getScreenWidth() const { return screenWidth; }
void Config::setScreenWidth(int width) { screenWidth = width; }
int Config::getScreenHeight() const { return screenHeight; }
void Config::setScreenHeight(int height) { screenHeight = height; }
bool Config::isFullscreen() const { return fullscreen; }
void Config::setFullscreen(bool enable) { fullscreen = enable; }

// Camera settings
float Config::getMouseSensitivity() const { return mouseSensitivity; }
void Config::setMouseSensitivity(float sensitivity) { mouseSensitivity = sensitivity; }
float Config::getMoveSpeed() const { return moveSpeed; }
void Config::setMoveSpeed(float speed) { moveSpeed = speed; }
float Config::getInteractionDistance() const { return interactionDistance; }
void Config::setInteractionDistance(float distance) { interactionDistance = distance; }

// Gameplay settings
float Config::getRotationStep() const { return rotationStep; }
void Config::setRotationStep(float step) { rotationStep = step; }
const std::string& Config::getAssetPath() const { return assetPath; }
void Config::setAssetPath(const std::string& path) { assetPath = path; }

bool Config::loadLevelConfig(const std::string& levelFile) {
    // TODO: Implement level config loading from file
    return false;
}

std::string Config::getLevelConfig(const std::string& key) const {
    auto it = levelConfigs.find(key);
    return (it != levelConfigs.end()) ? it->second : "";
}
