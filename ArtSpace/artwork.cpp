#include "Artwork.h"
#include <cstdlib>
#include <iostream>


// Constructor with position and dimensions
Artwork::Artwork(float x, float y, float z, float width, float height, ArtworkPlacement placement) {
    this->posX = x;
    this->posY = y;
    this->posZ = z;
    this->width = width;
    this->height = height;
    this->placement = placement;

    // Default values
    this->rotAngle = 0.0f;
    this->rotX = 0.0f;
    this->rotY = 1.0f;
    this->rotZ = 0.0f;
    this->scaleX = 1.0f;
    this->scaleY = 1.0f;
    this->scaleZ = 1.0f;

    // Frame properties
    this->hasFrame = true;
    this->frameWidth = 0.3f;
    this->frameR = 0.7f;
    this->frameG = 0.5f;
    this->frameB = 0.3f;

    this->artworkImage = new Image("#ffffff");  // White fallback color
    this->artworkImage->setPreserveAspectRatio(true);
}

// Constructor with image path
Artwork::Artwork(const std::string& imagePath, float x, float y, float z,
    float width, float height, ArtworkPlacement placement)
    : Artwork(x, y, z, width, height, placement) {
    // Load image
    setImage(imagePath);
}

// Destructor
Artwork::~Artwork() {
    if (artworkImage) {
        delete artworkImage;
    }
}

// Core rendering function
void Artwork::render() {
    glPushMatrix();

    // Apply placement transformation based on wall
    applyPlacement();

    // Apply position
    glTranslatef(posX, posY, posZ);

    // Apply rotation
    glRotatef(rotAngle, rotX, rotY, rotZ);

    // Apply scaling
    glScalef(scaleX, scaleY, scaleZ);

    // Draw frame if needed
    if (hasFrame) {
        drawFrame();
    }

    // Translate to center the image on the drawing point
    glPushMatrix();
    glTranslatef(-width / 2, -height / 2, 0.01f); // Small z offset to avoid z-fighting

    // Scale to fit our specified dimensions
    glScalef(width, height, 1.0f);

    // Render the image
    artworkImage->render();

    glPopMatrix();

    glPopMatrix();
}

// Transformation functions
void Artwork::translate(float dx, float dy, float dz) {
    posX += dx;
    posY += dy;
    posZ += dz;
}

void Artwork::rotate(float angle, float x, float y, float z) {
    rotAngle = angle;
    rotX = x;
    rotY = y;
    rotZ = z;
}

void Artwork::scale(float sx, float sy, float sz) {
    scaleX = sx;
    scaleY = sy;
    scaleZ = sz;
}

// Setters
void Artwork::setPosition(float x, float y, float z) {
    posX = x;
    posY = y;
    posZ = z;
}

void Artwork::setDimensions(float width, float height) {
    this->width = width;
    this->height = height;
}

void Artwork::setPlacement(ArtworkPlacement placement) {
    this->placement = placement;
}

void Artwork::setImage(const std::string& imagePath) {
    // If artworkImage already exists, just load the new image
    if (artworkImage) {
        artworkImage->loadImage(imagePath);
    }
    else {
        // Create new Image object
        artworkImage = new Image(imagePath);
    }
}

void Artwork::setFrame(bool hasFrame, float frameWidth, float r, float g, float b) {
    this->hasFrame = hasFrame;
    this->frameWidth = frameWidth;
    this->frameR = r;
    this->frameG = g;
    this->frameB = b;
}

void Artwork::setTint(float r, float g, float b, float a) {
    if (artworkImage) {
        artworkImage->setTint(r, g, b, a);
    }
}

void Artwork::setPreserveAspectRatio(bool preserve) {
    if (artworkImage) {
        artworkImage->setPreserveAspectRatio(preserve);
    }
}

// Getters
float* Artwork::getPosition() const {
    static float pos[3];
    pos[0] = posX;
    pos[1] = posY;
    pos[2] = posZ;
    return pos;
}

float* Artwork::getDimensions() const {
    static float dim[2];
    dim[0] = width;
    dim[1] = height;
    return dim;
}

ArtworkPlacement Artwork::getPlacement() const {
    return placement;
}

bool Artwork::isImageLoaded() const {
    return artworkImage && artworkImage->isImageLoaded();
}

void Artwork::drawFrame() {

    float halfW = width / 2.0f;
    float halfH = height / 2.0f;

    float outerHalfW = halfW + frameWidth;
    float outerHalfH = halfH + frameWidth;

    glColor3f(0.5f, 0.3f, 0.1f); // Example frame color

    glBegin(GL_QUADS);

    // Bottom strip
    glVertex3f(-outerHalfW, -outerHalfH, 0.0f);
    glVertex3f(outerHalfW, -outerHalfH, 0.0f);
    glVertex3f(outerHalfW, -halfH, 0.0f);
    glVertex3f(-outerHalfW, -halfH, 0.0f);

    // Top strip
    glVertex3f(-outerHalfW, halfH, 0.0f);
    glVertex3f(outerHalfW, halfH, 0.0f);
    glVertex3f(outerHalfW, outerHalfH, 0.0f);
    glVertex3f(-outerHalfW, outerHalfH, 0.0f);

    // Left strip
    glVertex3f(-outerHalfW, -halfH, 0.0f);
    glVertex3f(-halfW, -halfH, 0.0f);
    glVertex3f(-halfW, halfH, 0.0f);
    glVertex3f(-outerHalfW, halfH, 0.0f);

    // Right strip
    glVertex3f(halfW, -halfH, 0.0f);
    glVertex3f(outerHalfW, -halfH, 0.0f);
    glVertex3f(outerHalfW, halfH, 0.0f);
    glVertex3f(halfW, halfH, 0.0f);

    glEnd();
}



void Artwork::applyPlacement() {
    // Apply transformation based on wall placement
    switch (placement) {
    case NORTH_WALL:
        // North wall (default, no additional rotation needed)
        glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
        break;

    case EAST_WALL:
        // East wall (rotate 90 degrees around Y)
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        break;

    case SOUTH_WALL:
        // South wall (rotate 180 degrees around Y)
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        break;

    case WEST_WALL:
        // West wall (rotate 270 degrees around Y)
        glRotatef(270.0f, 0.0f, 1.0f, 0.0f);
        break;
    }
}