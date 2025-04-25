#include "room.h"

Room::Room(float width, float height, float depth) {
    dimensions[0] = width;
    dimensions[1] = height;
    dimensions[2] = depth;

    // TODO: Initialize wall vertices and texture coordinates
    wallVertices = nullptr;
    wallTexCoords = nullptr;
}

Room::~Room() {
    delete[] wallVertices;
    delete[] wallTexCoords;
}

void Room::render() {
    // TODO: Implement OpenGL room rendering
    // TODO: Apply wall and floor textures
    // TODO: Add lighting effects
}

const float* Room::getDimensions() const {
    return dimensions;
}

void Room::setWallTexture(const std::string& texturePath) {
    // TODO: Load and apply wall texture
}

void Room::setFloorTexture(const std::string& texturePath) {
    // TODO: Load and apply floor texture
}
