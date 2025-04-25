#pragma once
#include <string>
#include <GL/glut.h>

class Room {
private:
    float dimensions[3];  // width, height, depth
    float* wallVertices;
    float* wallTexCoords;
    GLuint wallTextureId;
    GLuint floorTextureId;

public:
    Room(float width, float height, float depth);
    ~Room();

    void render();
    const float* getDimensions() const;
    
    // Room customization
    void setWallTexture(const std::string& texturePath);
    void setFloorTexture(const std::string& texturePath);
};
