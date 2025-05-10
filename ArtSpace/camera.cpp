#include "camera.h"

// Constants
const float PI = 3.14159265358979323846f;
const float DEG_TO_RAD = PI / 180.0f;
const float MAX_PITCH = 89.0f;  // Prevent camera flipping at 90 degrees
const float HUMAN_CAMERA_SENSITIVITY = 0.5f;

Camera::Camera() {
    position[0] = 0.0f;  // x
    position[1] = 1.7f;  // y (eye level)
    position[2] = 0.0f;  // z
    
    // Initialize looking forward
    rotation[0] = 0.0f;  // pitch
    rotation[1] = 0.0f;  // yaw
    rotation[2] = 0.0f;  // 
    
    mouseSensitivity = 30.3f;
}

void Camera::update(float deltaTime) {
    InputSystem* input = InputSystem::getInstance();
    
    float deltaX, deltaY;
    input->getMouseDelta(deltaX, deltaY);
    
    // Only update rotation if there's actual mouse movement
    if (deltaX != 0.0f || deltaY != 0.0f) {
        // Update camera rotation based on mouse movement
        // Inverted Y-axis for more natural control
        rotation[0] += deltaY * mouseSensitivity; // Pitch (look up/down) - INVERTED
        rotation[1] += deltaX * mouseSensitivity; // Yaw (look left/right)
        
        // Ensure angles stay within bounds
        clampAngles();
    }
    
    float yawRadians = rotation[1] * DEG_TO_RAD;
    
    // Forward vector
    float forwardX = sin(yawRadians);
    float forwardZ = -cos(yawRadians);
    
    // Right vector
    float rightX = cos(yawRadians);
    float rightZ = sin(yawRadians);
    
    // Movement speed
    float moveSpeed = 0.1f;
    
    // Handle WASD movement
    if (input->isKeyPressed('w') || input->isKeyPressed('W')) {
        // Move forward
        position[0] += forwardX * moveSpeed;
        position[2] += forwardZ * moveSpeed;
    }
    if (input->isKeyPressed('s') || input->isKeyPressed('S')) {
        // Move backward
        position[0] -= forwardX * moveSpeed;
        position[2] -= forwardZ * moveSpeed;
    }
    if (input->isKeyPressed('a') || input->isKeyPressed('A')) {
        // Move left
        position[0] -= rightX * moveSpeed;
        position[2] -= rightZ * moveSpeed;
    }
    if (input->isKeyPressed('d') || input->isKeyPressed('D')) {
        // Move right
        position[0] += rightX * moveSpeed;
        position[2] += rightZ * moveSpeed;
    }
}

void Camera::applyTransformation() {
    // Apply the camera transformation to OpenGL
    glLoadIdentity();
    
    // Apply rotation
    glRotatef(rotation[0], 1.0f, 0.0f, 0.0f);  // Pitch (X-axis)
    glRotatef(rotation[1], 0.0f, 1.0f, 0.0f);  // Yaw (Y-axis)
    
    // Apply translation
    glTranslatef(-position[0], -position[1], -position[2]);
}

void Camera::clampAngles() {
    // Clamp pitch to prevent camera flipping
    if (rotation[0] > MAX_PITCH) rotation[0] = MAX_PITCH;
    if (rotation[0] < -MAX_PITCH) rotation[0] = -MAX_PITCH;
    
    // Normalize yaw to 0-360 degrees
    while (rotation[1] >= 360.0f) rotation[1] -= 360.0f;
    while (rotation[1] < 0.0f) rotation[1] += 360.0f;
}

void Camera::setPosition(float x, float y, float z) {
    position[0] = x;
    position[1] = y;
    position[2] = z;
}

void Camera::setRotation(float pitch, float yaw, float roll) {
    rotation[0] = pitch;
    rotation[1] = yaw;
    rotation[2] = roll;
    clampAngles();
}

void Camera::setMouseSensitivity(float sensitivity) {
    mouseSensitivity = sensitivity;
}

HumanCamera::HumanCamera() : Camera() {
    walkSpeed = 1.5f;         // 1.5 meters per second
    runSpeed = 4.0f;          // 4.0 meters per second
    currentSpeed = 0.0f;
    maxAcceleration = 4.0f;   // meters per second squared
    
    // Initialize velocity
    velocity[0] = 0.0f;
    velocity[1] = 0.0f;
    velocity[2] = 0.0f;
    
    // Head bobbing parameters
    bobAmplitude = 0.025f;    // 2.5cm up/down
    bobFrequency = 2.0f;      // 2 cycles per second at walk speed
    bobPhase = 0.0f;
    
    // Get initial height
    float pos[3];
    getPosition(pos);
    originalHeight = pos[1];
    
    isMoving = false;
    
    turnSmoothness = 0.3f;
    lastYawDelta = 0.0f;
    lastPitchDelta = 0.0f;
    
    setMouseSensitivity(HUMAN_CAMERA_SENSITIVITY);
}

void HumanCamera::update(float deltaTime) {
    InputSystem* input = InputSystem::getInstance();
    
    // Get mouse movement with smoothing
    float deltaX, deltaY;
    input->getMouseDelta(deltaX, deltaY);
    
    deltaX *= getMouseSensitivity();
    deltaY *= getMouseSensitivity();
    
    if (deltaX != 0.0f || deltaY != 0.0f) {
        // Reduce turnSmoothness to make sensitivity changes more noticeable
        float effectiveSmoothness = 0.15f; // Reduced from 0.3f
        
        // Smooth the mouse input
        float smoothX = deltaX * (1.0f - effectiveSmoothness) + lastYawDelta * effectiveSmoothness;
        float smoothY = deltaY * (1.0f - effectiveSmoothness) + lastPitchDelta * effectiveSmoothness;
        
        float pitch, yaw, roll;
        getRotation(pitch, yaw, roll);
        
        pitch += smoothY;
        yaw += smoothX;
        
        setRotation(pitch, yaw, roll);
        
        // Store deltas for next frame smoothing
        lastYawDelta = smoothX;
        lastPitchDelta = smoothY;
    } else {
        // Gradually reduce stored deltas when no new input
        lastYawDelta *= 0.8f;
        lastPitchDelta *= 0.8f;
    }
    
    // Calculate target speed based on input
    float targetSpeed = 0.0f;
    bool isRunning = input->isSpecialKeyPressed(112); // GLUT_KEY_SHIFT value is 112
    
    if (input->isKeyPressed('w') || input->isKeyPressed('W') || 
        input->isKeyPressed('s') || input->isKeyPressed('S') ||
        input->isKeyPressed('a') || input->isKeyPressed('A') ||
        input->isKeyPressed('d') || input->isKeyPressed('D')) {
        
        targetSpeed = isRunning ? runSpeed : walkSpeed;
    }
    
    // Update current speed with acceleration
    if (targetSpeed > currentSpeed) {
        currentSpeed += maxAcceleration * deltaTime;
        if (currentSpeed > targetSpeed) currentSpeed = targetSpeed;
    } else if (targetSpeed < currentSpeed) {
        // Faster deceleration than acceleration for more responsive stopping
        currentSpeed -= maxAcceleration * 2.5f * deltaTime;
        
        // Immediately stop if speed is very low to prevent unwanted sliding
        if (currentSpeed < 0.05f) currentSpeed = 0.0f;
    }
    
    // Track if we're moving for head bob
    isMoving = (currentSpeed > 0.1f);
    
    float yaw, pitch, roll;
    getRotation(pitch, yaw, roll);
    float yawRadians = yaw * DEG_TO_RAD;

    float forwardX = sin(yawRadians);
    float forwardZ = -cos(yawRadians);
    
    float rightX = cos(yawRadians);
    float rightZ = sin(yawRadians);
    
    float moveX = 0.0f;
    float moveZ = 0.0f;
    
    if (input->isKeyPressed('w') || input->isKeyPressed('W')) {
        moveX += forwardX;
        moveZ += forwardZ;
    }
    if (input->isKeyPressed('s') || input->isKeyPressed('S')) {
        moveX -= forwardX;
        moveZ -= forwardZ;
    }
    if (input->isKeyPressed('a') || input->isKeyPressed('A')) {
        moveX -= rightX;
        moveZ -= rightZ;
    }
    if (input->isKeyPressed('d') || input->isKeyPressed('D')) {
        moveX += rightX;
        moveZ += rightZ;
    }
    
    // Normalize if moving in multiple directions
    float length = sqrt(moveX * moveX + moveZ * moveZ);
    if (length > 0.001f) {
        moveX /= length;
        moveZ /= length;
    }
    
    // Apply movement based on current speed
    float distanceThisFrame = currentSpeed * deltaTime;
    float pos[3];
    getPosition(pos);
    
    pos[0] += moveX * distanceThisFrame;
    pos[2] += moveZ * distanceThisFrame;
    
    // Apply head bobbing if moving
    if (isMoving) {
        // Update bob phase based on distance moved
        bobPhase += distanceThisFrame * bobFrequency * PI;
        
        // Calculate head bob offset - more intense when running
        float intensity = currentSpeed / walkSpeed;
        float verticalBob = sin(bobPhase) * bobAmplitude * intensity;
        
        pos[1] = originalHeight + verticalBob;
    } else {
        pos[1] = originalHeight + (pos[1] - originalHeight) * 0.8f;
        bobPhase = 0.0f;
    }
    
    setPosition(pos[0], pos[1], pos[2]);
}

void HumanCamera::getPosition(float pos[3]) {
    float x, y, z;
    Camera::getPosition(x, y, z);
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
}

void HumanCamera::getRotation(float& pitch, float& yaw, float& roll) {
    float rot[3];
    Camera::getRotation(rot);
    pitch = rot[0];
    yaw = rot[1];
    roll = rot[2];
}