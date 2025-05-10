#pragma once
#include <GL/glut.h>
#include <cmath>
#include "input.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Camera {
private:
    float position[3];    // x, y, z
    float rotation[3];    // pitch, yaw, roll
    float mouseSensitivity;
    
    // Clamp angles to prevent camera flipping
    void clampAngles();

public:
    Camera();

    virtual void update(float deltaTime);
    
    void applyTransformation();
    
    // Get/set camera properties
    void setPosition(float x, float y, float z);
    void getPosition(float& x, float& y, float& z) { x = position[0]; y = position[1]; z = position[2]; }
    void getPosition(float pos[3]) { pos[0] = position[0]; pos[1] = position[1]; pos[2] = position[2]; }
    
    void setRotation(float pitch, float yaw, float roll);
    void getRotation(float rot[3]) { rot[0] = rotation[0]; rot[1] = rotation[1]; rot[2] = rotation[2]; }
    
    void getLookDirection(float dir[3]) {
        // Convert rotation angles to radians
        float yawRad = rotation[1] * M_PI / 180.0f;
        float pitchRad = rotation[0] * M_PI / 180.0f;
        
        dir[0] = sin(yawRad) * cos(pitchRad);
        dir[1] = sin(pitchRad);
        dir[2] = -cos(yawRad) * cos(pitchRad);
    }
    
    void setMouseSensitivity(float sensitivity);
    float getMouseSensitivity() { return mouseSensitivity; }
};

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
    
    float turnSmoothness;
    float lastYawDelta;
    float lastPitchDelta;

public:
    HumanCamera();
    
    void update(float deltaTime) override;
    
    void getPosition(float pos[3]);
    void getRotation(float& pitch, float& yaw, float& roll);
    
    // Method to easily adjust sensitivity
    void adjustSensitivity(float factor);
};
