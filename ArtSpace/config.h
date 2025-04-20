#pragma once
#include <string>
#include <unordered_map>

class Config {
private:
    static Config* instance;
    
    // Display settings
    int screenWidth;
    int screenHeight;
    bool fullscreen;
    
    // Camera settings
    float mouseSensitivity;
    float moveSpeed;
    float interactionDistance;
    
    // Gameplay settings
    float rotationStep;
    std::string assetPath;
    std::unordered_map<std::string, std::string> levelConfigs;

    Config();  // Private constructor for singleton

public:
    static Config* getInstance();
    ~Config();

    // Display settings
    int getScreenWidth() const;
    void setScreenWidth(int width);
    int getScreenHeight() const;
    void setScreenHeight(int height);
    bool isFullscreen() const;
    void setFullscreen(bool enable);

    // Camera settings
    float getMouseSensitivity() const;
    void setMouseSensitivity(float sensitivity);
    float getMoveSpeed() const;
    void setMoveSpeed(float speed);
    float getInteractionDistance() const;
    void setInteractionDistance(float distance);

    // Gameplay settings
    float getRotationStep() const;
    void setRotationStep(float step);
    const std::string& getAssetPath() const;
    void setAssetPath(const std::string& path);

    // Level configuration
    bool loadLevelConfig(const std::string& levelFile);
    std::string getLevelConfig(const std::string& key) const;
    
    // TODO: Add config file loading/saving
    // TODO: Add user preferences
    // TODO: Add graphics quality settings
};
