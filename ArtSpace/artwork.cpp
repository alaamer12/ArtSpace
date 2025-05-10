#include "Artwork.h"



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
    
    // Initialize stretching values to 1.0 (no stretch)
    this->imageStretchX = 1.0f;
    this->imageStretchY = 1.0f;
    this->frameStretchX = 1.0f;
    this->frameStretchY = 1.0f;

    // Frame properties
    this->hasFrame = true;
    this->frameWidth = 0.3f;
    this->frameR = 0.7f;
    this->frameG = 0.5f;
    this->frameB = 0.3f;

    this->artworkImage = new Image("#ffffff");  // White fallback color
    this->artworkImage->setPreserveAspectRatio(true);
    
    this->frameImage = nullptr;
}

// Constructor with image path
Artwork::Artwork(const std::string& imagePath, float x, float y, float z,
    float width, float height, ArtworkPlacement placement)
    : Artwork(x, y, z, width, height, placement) {
    // Load image
    setImage(imagePath);
}

// Constructor with image and frame path
Artwork::Artwork(const std::string& imagePath, const std::string& framePath, float x, float y, float z,
    float width, float height, ArtworkPlacement placement)
    : Artwork(x, y, z, width, height, placement) {
    // Load image and frame
    setImage(imagePath);
    setFrame(framePath);
}

// Destructor
Artwork::~Artwork() {
    if (artworkImage) {
        delete artworkImage;
    }
    if (frameImage) {
        delete frameImage;
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

    // Apply global scaling
    glScalef(scaleX, scaleY, scaleZ);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Render the artwork image with its stretch
    glPushMatrix();
    glTranslatef(-width / 2, -height / 2, 0.0f);
    // Apply image-specific stretching
    glScalef(width * imageStretchX, height * imageStretchY, 1.0f);
    artworkImage->render();
    glPopMatrix();

    // Draw frame if needed
    if (hasFrame) {
        if (frameImage) {
            // If we have a frame image, use it with its own stretch
            glPushMatrix();
            glTranslatef(-width / 2, -height / 2, 0.02f); // Small z offset to be in front
            // Apply frame-specific stretching
            glScalef(width * frameStretchX, height * frameStretchY, 1.0f);
            frameImage->render();
            glPopMatrix();
        } else {
            // Otherwise use the colored frame (we'll apply the frame stretch here too)
            drawFrame();
        }
    }

    // Disable blending
    glDisable(GL_BLEND);

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

// New stretching functions
void Artwork::stretchImage(float stretchX, float stretchY) {
    // Ensure stretch values don't go below a minimum threshold
    imageStretchX = (stretchX < 0.1f) ? 0.1f : stretchX;
    imageStretchY = (stretchY < 0.1f) ? 0.1f : stretchY;
    
    // If preserveAspectRatio is enabled, we need to make sure the Image
    // class knows not to override our stretching
    if (artworkImage) {
        artworkImage->setPreserveAspectRatio(false);
    }
}

void Artwork::stretchFrame(float stretchX, float stretchY) {
    // Ensure stretch values don't go below a minimum threshold
    frameStretchX = (stretchX < 0.1f) ? 0.1f : stretchX;
    frameStretchY = (stretchY < 0.1f) ? 0.1f : stretchY;
    
    if (frameImage) {
        frameImage->setPreserveAspectRatio(false);
    }
}

void Artwork::resetImageStretch() {
    imageStretchX = 1.0f;
    imageStretchY = 1.0f;
    // Optionally restore aspect ratio preservation if that was the original setting
    if (artworkImage) {
        artworkImage->setPreserveAspectRatio(true);
    }
}

void Artwork::resetFrameStretch() {
    frameStretchX = 1.0f;
    frameStretchY = 1.0f;
    // Optionally restore aspect ratio preservation if that was the original setting
    if (frameImage) {
        frameImage->setPreserveAspectRatio(true);
    }
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
        if (!artworkImage->loadImage(imagePath)) {
            Logger::getInstance().logWarning("Artwork::setImage - Failed to load image: " + imagePath);
        }
    }
    else {
        // Create new Image object
        artworkImage = new Image(imagePath);
        artworkImage->setPreserveAspectRatio(true);
        if (!artworkImage->isImageLoaded()) {
            Logger::getInstance().logWarning("Artwork::setImage - Failed to load image for new Image object: " + imagePath);
        }
    }
}

void Artwork::setFrame(const std::string& framePath) {
    if (frameImage) {
        if (!frameImage->loadImage(framePath)) {
            Logger::getInstance().logWarning("Artwork::setFrame - Failed to load frame image: " + framePath);
        }
    }
    else {
        // Create new Image object for the frame
        frameImage = new Image(framePath);
        frameImage->setPreserveAspectRatio(true);
        if (!frameImage->isImageLoaded()) {
            Logger::getInstance().logWarning("Artwork::setFrame - Failed to load frame image for new Image object: " + framePath);
        }
    }
    hasFrame = true; // Ensure hasFrame is true if a frame path is provided
}

void Artwork::setFrame(bool hasFrame, float frameWidth, float r, float g, float b) {
    this->hasFrame = hasFrame;
    this->frameWidth = frameWidth;
    this->frameR = r;
    this->frameG = g;
    this->frameB = b;
    
    // If we're disabling the frame, we can free the frame image
    if (!hasFrame && frameImage) {
        delete frameImage;
        frameImage = nullptr;
    }
}

void Artwork::setTint(float r, float g, float b, float a) {
    if (artworkImage) {
        artworkImage->setTint(r, g, b, a);
    }
}

void Artwork::setPreserveAspectRatio(bool preserve) {
    if (artworkImage) {
        artworkImage->setPreserveAspectRatio(preserve);
        
        // If preserving aspect ratio, reset stretching values
        if (preserve) {
            imageStretchX = 1.0f;
            imageStretchY = 1.0f;
        }
    }
    if (frameImage) {
        frameImage->setPreserveAspectRatio(preserve);
        
        // If preserving aspect ratio, reset stretching values
        if (preserve) {
            frameStretchX = 1.0f;
            frameStretchY = 1.0f;
        }
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

    // Apply frame stretching to the frame dimensions
    halfW *= frameStretchX;
    halfH *= frameStretchY;

    float outerHalfW = halfW + frameWidth * frameStretchX;
    float outerHalfH = halfH + frameWidth * frameStretchY;

    glColor3f(frameR, frameG, frameB); // Use the frame color

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