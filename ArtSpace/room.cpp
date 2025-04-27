#include "room.h"
<<<<<<< Updated upstream

Room::Room(float width, float height, float depth) {
    dimensions[0] = width;
    dimensions[1] = height;
    dimensions[2] = depth;

    // TODO: Initialize wall vertices and texture coordinates
=======
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>

// BMP file header structure
#pragma pack(push, 1)
struct BMPHeader {
    char signature[2];
    uint32_t fileSize;
    uint32_t reserved;
    uint32_t dataOffset;
    uint32_t headerSize;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitsPerPixel;
    uint32_t compression;
    uint32_t dataSize;
    int32_t hResolution;
    int32_t vResolution;
    uint32_t colors;
    uint32_t importantColors;
};
#pragma pack(pop)

Room::Room(float width, float height, float depth) {
    ROOM_WIDTH = width;
    ROOM_HEIGHT = height;
    ROOM_DEPTH = depth;

    // Initialize texture IDs to 0
    wallTextureID = 0;
    floorTextureID = 0;
    roofTextureID = 0;
    doorTextureID = 0;

    // Generate default textures
    generateCheckerboardTexture(wallTextureID, 0.8f, 0.8f, 0.8f);    // Light gray
    generateCheckerboardTexture(floorTextureID, 0.6f, 0.4f, 0.2f);   // Brown
    generateCheckerboardTexture(roofTextureID, 0.9f, 0.9f, 0.9f);    // White
    generateCheckerboardTexture(doorTextureID, 0.5f, 0.35f, 0.05f);  // Dark wood

    // We don't need to allocate these as we're using the drawTexturedQuad helper
>>>>>>> Stashed changes
    wallVertices = nullptr;
    wallTexCoords = nullptr;
}

Room::~Room() {
<<<<<<< Updated upstream
=======
    // Delete textures
    if (wallTextureID != 0) glDeleteTextures(1, &wallTextureID);
    if (floorTextureID != 0) glDeleteTextures(1, &floorTextureID);
    if (roofTextureID != 0) glDeleteTextures(1, &roofTextureID);
    if (doorTextureID != 0) glDeleteTextures(1, &doorTextureID);

    // Free memory
>>>>>>> Stashed changes
    delete[] wallVertices;
    delete[] wallTexCoords;
}

void Room::render() {
<<<<<<< Updated upstream
    // TODO: Implement OpenGL room rendering
    // TODO: Apply wall and floor textures
    // TODO: Add lighting effects
}

const float* Room::getDimensions() const {
=======
    // Enable texture mapping
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);  // Reset color to white for proper texture display

    // Define how many times to repeat the texture
    float wallRepeat = 1.0f;  // Repeat texture twice on walls
    float floorRepeat = 4.0f; // Repeat texture 4 times on floor 
    float ceilingRepeat = 1.0f; // Repeat texture 4 times on  ceiling

    // Draw back wall
    drawTexturedQuad(
        -ROOM_WIDTH / 2, -ROOM_HEIGHT / 2, -ROOM_DEPTH / 2,
        ROOM_WIDTH / 2, -ROOM_HEIGHT / 2, -ROOM_DEPTH / 2,
        ROOM_WIDTH / 2, ROOM_HEIGHT / 2, -ROOM_DEPTH / 2,
        -ROOM_WIDTH / 2, ROOM_HEIGHT / 2, -ROOM_DEPTH / 2,
        wallTextureID, wallRepeat
    );

    // Draw left wall
    drawTexturedQuad(
        -ROOM_WIDTH / 2, -ROOM_HEIGHT / 2, -ROOM_DEPTH / 2,
        -ROOM_WIDTH / 2, -ROOM_HEIGHT / 2, ROOM_DEPTH / 2,
        -ROOM_WIDTH / 2, ROOM_HEIGHT / 2, ROOM_DEPTH / 2,
        -ROOM_WIDTH / 2, ROOM_HEIGHT / 2, -ROOM_DEPTH / 2,
        wallTextureID, wallRepeat
    );

    // Draw right wall
    drawTexturedQuad(
        ROOM_WIDTH / 2, -ROOM_HEIGHT / 2, -ROOM_DEPTH / 2,
        ROOM_WIDTH / 2, -ROOM_HEIGHT / 2, ROOM_DEPTH / 2,
        ROOM_WIDTH / 2, ROOM_HEIGHT / 2, ROOM_DEPTH / 2,
        ROOM_WIDTH / 2, ROOM_HEIGHT / 2, -ROOM_DEPTH / 2,
        wallTextureID, wallRepeat
    );

    // Draw front wall (complete wall without door opening)
    drawTexturedQuad(
        -ROOM_WIDTH / 2, -ROOM_HEIGHT / 2, ROOM_DEPTH / 2,
        ROOM_WIDTH / 2, -ROOM_HEIGHT / 2, ROOM_DEPTH / 2,
        ROOM_WIDTH / 2, ROOM_HEIGHT / 2, ROOM_DEPTH / 2,
        -ROOM_WIDTH / 2, ROOM_HEIGHT / 2, ROOM_DEPTH / 2,
        wallTextureID, wallRepeat
    );

    
    // Draw floor
    drawTexturedQuad(
        -ROOM_WIDTH / 2, -ROOM_HEIGHT / 2, -ROOM_DEPTH / 2,
        -ROOM_WIDTH / 2, -ROOM_HEIGHT / 2, ROOM_DEPTH / 2,
        ROOM_WIDTH / 2, -ROOM_HEIGHT / 2, ROOM_DEPTH / 2,
        ROOM_WIDTH / 2, -ROOM_HEIGHT / 2, -ROOM_DEPTH / 2,
        floorTextureID, floorRepeat
    );

    // Draw ceiling/roof
    drawTexturedQuad(
        -ROOM_WIDTH / 2, ROOM_HEIGHT / 2, -ROOM_DEPTH / 2,
        ROOM_WIDTH / 2, ROOM_HEIGHT / 2, -ROOM_DEPTH / 2,
        ROOM_WIDTH / 2, ROOM_HEIGHT / 2, ROOM_DEPTH / 2,
        -ROOM_WIDTH / 2, ROOM_HEIGHT / 2, ROOM_DEPTH / 2,
        roofTextureID, ceilingRepeat
    );

    glDisable(GL_TEXTURE_2D);
}

const float* Room::getDimensions() const {
    static float dimensions[3];
    dimensions[0] = ROOM_WIDTH;
    dimensions[1] = ROOM_HEIGHT;
    dimensions[2] = ROOM_DEPTH;
>>>>>>> Stashed changes
    return dimensions;
}

void Room::setWallTexture(const std::string& texturePath) {
<<<<<<< Updated upstream
    // TODO: Load and apply wall texture
}

void Room::setFloorTexture(const std::string& texturePath) {
    // TODO: Load and apply floor texture
}
=======
    // Delete old texture if it exists
    if (wallTextureID != 0) {
        glDeleteTextures(1, &wallTextureID);
    }

    // Load new texture
    wallTextureID = loadTexture(texturePath);

    // If loading failed, create a default texture
    if (wallTextureID == 0) {
        generateCheckerboardTexture(wallTextureID, 0.8f, 0.8f, 0.8f); // Light gray
        std::cout << "Failed to load wall texture. Using default." << std::endl;
    }
}

void Room::setFloorTexture(const std::string& texturePath) {
    // Delete old texture if it exists
    if (floorTextureID != 0) {
        glDeleteTextures(1, &floorTextureID);
    }

    // Load new texture
    floorTextureID = loadTexture(texturePath);

    // If loading failed, create a default texture
    if (floorTextureID == 0) {
        generateCheckerboardTexture(floorTextureID, 0.6f, 0.4f, 0.2f); // Brown
        std::cout << "Failed to load floor texture. Using default." << std::endl;
    }
}

void Room::setRoofTexture(const std::string& texturePath) {
    // Delete old texture if it exists
    if (roofTextureID != 0) {
        glDeleteTextures(1, &roofTextureID);
    }

    // Load new texture
    roofTextureID = loadTexture(texturePath);

    // If loading failed, create a default texture
    if (roofTextureID == 0) {
        generateCheckerboardTexture(roofTextureID, 0.9f, 0.9f, 0.9f); // White
        std::cout << "Failed to load roof texture. Using default." << std::endl;
    }
}



GLuint Room::loadTexture(const std::string& filename) {
    // Open the BMP file
    std::ifstream file(filename.c_str(), std::ios::binary);
    if (!file) {
        std::cerr << "Error: Could not open " << filename << std::endl;
        return 0;
    }

    // Read the header
    BMPHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(BMPHeader));

    // Verify it's a BMP file
    if (header.signature[0] != 'B' || header.signature[1] != 'M') {
        std::cerr << "Error: " << filename << " is not a valid BMP file" << std::endl;
        file.close();
        return 0;
    }

    // Move to pixel data
    file.seekg(header.dataOffset, std::ios::beg);

    // Allocate memory for image data (3 bytes per pixel for RGB)
    int imageSize = header.width * header.height * 3;
    unsigned char* imageData = new unsigned char[imageSize];

    // Read image data
    file.read(reinterpret_cast<char*>(imageData), imageSize);
    file.close();

    // BMP stores images bottom-to-top and BGR instead of RGB, so we need to convert
    unsigned char* flippedData = new unsigned char[imageSize];
    for (int y = 0; y < header.height; y++) {
        for (int x = 0; x < header.width; x++) {
            int srcIndex = ((header.height - 1 - y) * header.width + x) * 3;
            int destIndex = (y * header.width + x) * 3;

            // Swap B and R components
            flippedData[destIndex] = imageData[srcIndex + 2]; // R
            flippedData[destIndex + 1] = imageData[srcIndex + 1]; // G
            flippedData[destIndex + 2] = imageData[srcIndex]; // B
        }
    }

    // Generate the OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Create the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, header.width, header.height, 0, GL_RGB, GL_UNSIGNED_BYTE, flippedData);

    // Clean up
    delete[] imageData;
    delete[] flippedData;

    return textureID;
}

void Room::generateCheckerboardTexture(GLuint& textureID, float r, float g, float b) {
    // Create a simple colored checkerboard texture
    const int texSize = 64;
    unsigned char textureData[texSize][texSize][3];

    for (int i = 0; i < texSize; i++) {
        for (int j = 0; j < texSize; j++) {
            int c = ((((i & 0x8) == 0) ^ ((j & 0x8) == 0))) * 255;

            textureData[i][j][0] = static_cast<unsigned char>(c * r); // R
            textureData[i][j][1] = static_cast<unsigned char>(c * g); // G
            textureData[i][j][2] = static_cast<unsigned char>(c * b); // B
        }
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texSize, texSize, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
}

void Room::drawTexturedQuad(float x1, float y1, float z1,
    float x2, float y2, float z2,
    float x3, float y3, float z3,
    float x4, float y4, float z4,
    GLuint textureID, float textureRepeat) {

    glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(GL_QUADS);

    // Calculate normal for proper lighting
    float nx = (y2 - y1) * (z3 - z1) - (z2 - z1) * (y3 - y1);
    float ny = (z2 - z1) * (x3 - x1) - (x2 - x1) * (z3 - z1);
    float nz = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);

    // Normalize
    float length = sqrt(nx * nx + ny * ny + nz * nz);
    if (length > 0) {
        nx /= length;
        ny /= length;
        nz /= length;
    }

    glNormal3f(nx, ny, nz);

    // Bottom-left vertex
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x1, y1, z1);

    // Bottom-right vertex
    glTexCoord2f(textureRepeat, 0.0f);
    glVertex3f(x2, y2, z2);

    // Top-right vertex
    glTexCoord2f(textureRepeat, textureRepeat);
    glVertex3f(x3, y3, z3);

    // Top-left vertex
    glTexCoord2f(0.0f, textureRepeat);
    glVertex3f(x4, y4, z4);

    glEnd();
}
>>>>>>> Stashed changes
