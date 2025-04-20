#pragma once
#include <GL/glut.h>

class Camera {
private:
    float position[3];  // x, y, z
    float rotation[3];  // pitch, yaw, roll
    float mouseSensitivity;

public:
    Camera();
    ~Camera();

    // Basic camera controls
    void update(float deltaTime);
    void processMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
    void processKeyboard(unsigned char key, float deltaTime);

    // Getters/Setters
    const float* getPosition() const;
    const float* getRotation() const;
    void setPosition(float x, float y, float z);
    void setRotation(float pitch, float yaw, float roll);

    // OpenGL specific functions
    void applyViewMatrix();
    void setupProjection(float fov, float aspectRatio, float nearPlane, float farPlane);
};
