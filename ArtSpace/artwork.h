#pragma once
#include <GL/glut.h>
#include <string>

class Artwork {
private:
    float position[3];
    float rotation[3];
    float dimensions[2];  // width, height
    GLuint textureId;
    bool isInteractable;
    bool isSolved;

public:
    Artwork(const std::string& texturePath, float x, float y, float z);
    ~Artwork();

    // Basic functionality
    void render();
    void update(float deltaTime);
    bool interact();
    void rotate(float angle);
    
    // Puzzle integration
    bool checkSolution();
    void setCorrectRotation(float angle);
    
    // Getters/Setters
    const float* getPosition() const;
    const float* getRotation() const;
    bool getIsSolved() const;
    
    // OpenGL specific
    void bindTexture();
    void unbindTexture();
};
