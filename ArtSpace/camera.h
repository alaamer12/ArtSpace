/**
 * @file camera.h
 * @brief Camera system with basic and human-like camera controls
 * 
 * This module provides camera classes for 3D OpenGL applications, offering both basic camera
 * functionality and a more sophisticated human-like camera with realistic movement physics.
 * The cameras handle position, rotation, and transformation in 3D space.
 * 
 * Main classes:
 * - Camera: Base camera class with basic positioning and rotation
 * - HumanCamera: Extended camera with realistic human movement, including head bobbing,
 *                momentum, acceleration, and smooth turning
 * 
 * Usage examples:
 * 
 * 1. Using the base Camera:
 *    Camera camera;
 *    camera.setPosition(0.0f, 1.75f, 5.0f);
 *    camera.setRotation(0.0f, 180.0f, 0.0f);
 *    
 *    // In render function:
 *    camera.update(deltaTime);
 *    camera.applyTransformation();
 * 
 * 2. Using the HumanCamera:
 *    HumanCamera humanCamera;
 *    humanCamera.setPosition(0.0f, 1.75f, 5.0f);
 *    humanCamera.adjustSensitivity(0.5f); // Reduce sensitivity
 *    
 *    // In render function:
 *    humanCamera.update(deltaTime);
 *    humanCamera.applyTransformation();
 *    
 * The camera system works with the InputSystem to process mouse and keyboard input
 * for camera movement and rotation.
 */

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
