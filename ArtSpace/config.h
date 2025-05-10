#pragma once
#include <string>
#include "utility.h"
#include <algorithm>
#include <iostream>
#include <fstream>


class Config {
private:
    static Config instance;
    
    struct DisplaySettings {
        int screenWidth;
        int screenHeight;
        bool fullscreen;
    };
    
    struct CameraSettings {
        float mouseSensitivity;
        float moveSpeed;
        float interactionDistance;
    };
    
    struct GameplaySettings {
        float rotationStep;
        std::string assetPath;
    };
    
    DisplaySettings displaySettings;
    CameraSettings cameraSettings;
    GameplaySettings gameplaySettings;
  
    static const int MIN_SCREEN_WIDTH = 800;
    static const int MAX_SCREEN_WIDTH = 3840;
    static const int MIN_SCREEN_HEIGHT = 600;
    static const int MAX_SCREEN_HEIGHT = 2160;
   
    static constexpr float MIN_MOUSE_SENSITIVITY = 0.05f;
    static constexpr float MAX_MOUSE_SENSITIVITY = 0.5f;
    static constexpr float MIN_MOVE_SPEED = 1.0f;
    static constexpr float MAX_MOVE_SPEED = 10.0f;
    static constexpr float MIN_INTERACTION_DISTANCE = 0.5f;
    static constexpr float MAX_INTERACTION_DISTANCE = 5.0f;
    
    static constexpr float MIN_ROTATION_STEP = 5.0f;
    static constexpr float MAX_ROTATION_STEP = 180.0f;

    Config();

        
    int validateScreenWidth(int width) const;
    int validateScreenHeight(int height) const;
    float validateMouseSensitivity(float sensitivity) const;
    float validateMoveSpeed(float speed) const;
    float validateInteractionDistance(float distance) const;
    float validateRotationStep(float step) const;
    
public:
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

    static Config& getInstance();

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
   
};
