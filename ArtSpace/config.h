#pragma once
#include <string>
#include "utility.h"  // Include utility.h for Logger

// Optimized Config class for single-level game
class Config {
private:
    // Singleton instance (statically allocated)
    static Config instance;
    
    // Display settings struct
    struct DisplaySettings {
        int screenWidth;
        int screenHeight;
        bool fullscreen;
    };
    
    // Camera settings struct
    struct CameraSettings {
        float mouseSensitivity;
        float moveSpeed;
        float interactionDistance;
    };
    
    // Gameplay settings struct
    struct GameplaySettings {
        float rotationStep;
        std::string assetPath;
    };
    
    // Settings structs
    DisplaySettings displaySettings;
    CameraSettings cameraSettings;
    GameplaySettings gameplaySettings;
    
    // Display limits
    static const int MIN_SCREEN_WIDTH = 800;
    static const int MAX_SCREEN_WIDTH = 3840;
    static const int MIN_SCREEN_HEIGHT = 600;
    static const int MAX_SCREEN_HEIGHT = 2160;
    
    // Camera limits
    static constexpr float MIN_MOUSE_SENSITIVITY = 0.05f;
    static constexpr float MAX_MOUSE_SENSITIVITY = 0.5f;
    static constexpr float MIN_MOVE_SPEED = 1.0f;
    static constexpr float MAX_MOVE_SPEED = 10.0f;
    static constexpr float MIN_INTERACTION_DISTANCE = 0.5f;
    static constexpr float MAX_INTERACTION_DISTANCE = 5.0f;
    
    // Gameplay limits
    static constexpr float MIN_ROTATION_STEP = 5.0f;
    static constexpr float MAX_ROTATION_STEP = 180.0f;

    // Private constructor (singleton)
    Config();

        
    // Limit validation methods
    int validateScreenWidth(int width) const;
    int validateScreenHeight(int height) const;
    float validateMouseSensitivity(float sensitivity) const;
    float validateMoveSpeed(float speed) const;
    float validateInteractionDistance(float distance) const;
    float validateRotationStep(float step) const;
    
public:
    // Delete copy constructor and assignment operator
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

    // Get singleton instance
    static Config& getInstance();

    // Destructor
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

    
    // Configuration presets
    void applyOptimalSettings();
    void resetToDefaults();
    
    // Configuration file operations
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename);
    
};
