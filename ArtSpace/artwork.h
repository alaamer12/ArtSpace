#ifndef ARTWORK_H
#define ARTWORK_H

#include <GL/glut.h>
#include <string>
#include "utility.h" // Include your custom Image class

enum ArtworkPlacement {
    NORTH_WALL,
    EAST_WALL,
    SOUTH_WALL,
    WEST_WALL
};

class Artwork {
public:
    // Constructor with position and dimensions
    Artwork(float x, float y, float z, float width, float height,
        ArtworkPlacement placement = NORTH_WALL);

    // Constructor with image path
    Artwork(const std::string& imagePath, float x, float y, float z,
        float width, float height, ArtworkPlacement placement = NORTH_WALL);

    // Constructor with image and frame paths
    Artwork(const std::string& imagePath, const std::string& framePath, float x, float y, float z,
        float width, float height, ArtworkPlacement placement = NORTH_WALL);

    ~Artwork();

    // Core rendering function
    void render();

    // Transformation functions
    void translate(float dx, float dy, float dz);
    void rotate(float angle, float x, float y, float z);
    void scale(float sx, float sy, float sz);
    
    // New stretching and scaling functions
    void stretchImage(float stretchX, float stretchY);
    void stretchFrame(float stretchX, float stretchY);
    void resetImageStretch();
    void resetFrameStretch();

    // Setters
    void setPosition(float x, float y, float z);
    void setDimensions(float width, float height);
    void setPlacement(ArtworkPlacement placement);
    void setImage(const std::string& imagePath);
    void setFrame(const std::string& framePath);
    void setFrame(bool hasFrame, float frameWidth = 0.1f,
        float r = 0.7f, float g = 0.5f, float b = 0.3f);
    void setTint(float r, float g, float b, float a = 1.0f);
    void setPreserveAspectRatio(bool preserve);

    // Getters
    float* getPosition() const;
    float* getDimensions() const;
    ArtworkPlacement getPlacement() const;
    bool isImageLoaded() const;
    
    // New getters for stretch values
    float getImageStretchX() const { return imageStretchX; }
    float getImageStretchY() const { return imageStretchY; }
    float getFrameStretchX() const { return frameStretchX; }
    float getFrameStretchY() const { return frameStretchY; }

private:
    // Position
    float posX, posY, posZ;

    // Dimensions
    float width, height;

    // Rotation/transform attributes
    float rotAngle;
    float rotX, rotY, rotZ;
    float scaleX, scaleY, scaleZ;
    
    // New stretching attributes
    float imageStretchX, imageStretchY;
    float frameStretchX, frameStretchY;

    ArtworkPlacement placement;

    // Image objects
    Image* artworkImage;    // The actual artwork
    Image* frameImage;      // The frame image

    // Frame properties
    bool hasFrame;
    float frameWidth;
    float frameR, frameG, frameB;

    // Helper functions
    void drawFrame();
    void applyPlacement();
};

#endif // ARTWORK_H