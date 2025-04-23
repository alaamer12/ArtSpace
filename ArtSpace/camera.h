#pragma once
#include <GL/glut.h>
#include "input.h"

class Camera {
private:
    float position[3];    // x, y, z
    float rotation[3];    // pitch, yaw, roll
    float mouseSensitivity;
    
    // Clamp angles to prevent camera flipping
    void clampAngles();

public:
    Camera();

    // Main update function - responds to input system passively
    virtual void update(float deltaTime);
    
    // Apply camera transformation to OpenGL
    void applyTransformation();
    
    // Get/set camera properties
    void setPosition(float x, float y, float z);
    void getPosition(float& x, float& y, float& z) { x = position[0]; y = position[1]; z = position[2]; }
    
    void setRotation(float pitch, float yaw, float roll);
    void getRotation(float rot[3]) { rot[0] = rotation[0]; rot[1] = rotation[1]; rot[2] = rotation[2]; }
    
    void setMouseSensitivity(float sensitivity);
    float getMouseSensitivity() { return mouseSensitivity; }
};

// HumanCamera class that simulates realistic human movement
class HumanCamera : public Camera {
private:
    // Movement parameters
    float walkSpeed;
    float runSpeed;
    float currentSpeed;
    float maxAcceleration;
    float velocity[3];
    
    // Head bobbing parameters
    float bobAmplitude;
    float bobFrequency;
    float bobPhase;
    float originalHeight;
    bool isMoving;
    
    // Momentum and smoothing
    float turnSmoothness;
    float lastYawDelta;
    float lastPitchDelta;

public:
    HumanCamera();
    
    // Override the update method to implement human-like motion
    void update(float deltaTime) override;
    
    // Helper methods
    void getPosition(float pos[3]);
    void getRotation(float& pitch, float& yaw, float& roll);
    
    // Method to easily adjust sensitivity
    void adjustSensitivity(float factor);
};
