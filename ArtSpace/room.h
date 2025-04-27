
#ifndef ROOM_H
#define ROOM_H

#include <GL/glut.h>
#include <string>
#include <vector>

class Room {
public:
    Room(float width, float height, float depth);
    ~Room();

    void render();
    const float* getDimensions() const;
    
    // Room customization
    void setWallTexture(const std::string& texturePath);


    // Texture management functions
    void setFloorTexture(const std::string& texturePath);
    void setRoofTexture(const std::string& texturePath);

private:
    float ROOM_WIDTH;
    float ROOM_HEIGHT;
    float ROOM_DEPTH;

    // Texture IDs
    GLuint wallTextureID;
    GLuint floorTextureID;
    GLuint roofTextureID;
    GLuint doorTextureID;

    // For wall vertices and texture coordinates
    float* wallVertices;
    float* wallTexCoords;

    // Helper functions
    GLuint loadTexture(const std::string& filename);
    void generateCheckerboardTexture(GLuint& textureID, float r, float g, float b);
    void drawTexturedQuad(float x1, float y1, float z1,
        float x2, float y2, float z2,
        float x3, float y3, float z3,
        float x4, float y4, float z4,
        GLuint textureID, float textureRepeat = 1.0f);
};

#endif // ROOM_H
