#include "artwork_manager.h"
#include <iostream>
#include <cmath>

// Initialize static instance
ArtworkManager* ArtworkManager::instance = nullptr;

// Constructor
ArtworkManager::ArtworkManager() {
    // Initialize any resources needed by the manager
}

// Destructor
ArtworkManager::~ArtworkManager() {
    // Clean up all artworks
    clear();
}

// Get singleton instance
ArtworkManager* ArtworkManager::getInstance() {
    if (instance == nullptr) {
        instance = new ArtworkManager();
    }
    return instance;
}

// Create an artwork with an image
Artwork* ArtworkManager::createArtwork(const std::string& imagePath, float x, float y, float z) {
    Artwork* newArtwork = new Artwork(imagePath, x, y, z, 0.003f, 0.003f, NORTH_WALL);
    artworks.push_back(newArtwork);
    return newArtwork;
}

// Create an artwork with an image and frame
Artwork* ArtworkManager::createArtwork(const std::string& imagePath, const std::string& framePath, 
                                      float x, float y, float z,
                                      float width, float height, 
                                      ArtworkPlacement placement) {
    Artwork* newArtwork = new Artwork(imagePath, framePath, x, y, z, width, height, placement);
    artworks.push_back(newArtwork);
    return newArtwork;
}

// Create artwork from config
Artwork* ArtworkManager::createArtworkFromConfig(const std::string& imagePath, const std::string& framePath, 
                                                const ArtworkConfig& config) {
    Artwork* newArtwork = new Artwork(imagePath, framePath, 
                                      config.posX, config.posY, config.posZ, 
                                      config.width, config.height, 
                                      config.placement);
    
    // Apply additional configurations
    if (config.hasRotation) {
        newArtwork->rotate(config.rotAngle, config.rotX, config.rotY, config.rotZ);
    }
    
    if (config.hasImageStretch) {
        newArtwork->stretchImage(config.imageStretchX, config.imageStretchY);
    }
    
    if (config.hasFrameStretch) {
        newArtwork->stretchFrame(config.frameStretchX, config.frameStretchY);
    }
    
    artworks.push_back(newArtwork);
    return newArtwork;
}

// Remove a specific artwork
void ArtworkManager::removeArtwork(Artwork* artwork) {
    for (auto it = artworks.begin(); it != artworks.end(); ++it) {
        if (*it == artwork) {
            delete *it;
            artworks.erase(it);
            break;
        }
    }
}

// Clear all artworks
void ArtworkManager::clear() {
    for (auto artwork : artworks) {
        delete artwork;
    }
    artworks.clear();
}

// Render all artworks
void ArtworkManager::renderAll() {
    for (auto artwork : artworks) {
        artwork->render();
    }
}

// Update all artworks
void ArtworkManager::updateAll(float deltaTime) {
    // If artworks need any updates (like animations), implement here
}

// Find nearest artwork to a position
Artwork* ArtworkManager::findNearestArtwork(const float* position, float maxDistance) {
    Artwork* nearest = nullptr;
    float minDistance = maxDistance;
    
    for (auto artwork : artworks) {
        float* artPos = artwork->getPosition();
        float distance = std::sqrt(
            std::pow(position[0] - artPos[0], 2) + 
            std::pow(position[1] - artPos[1], 2) + 
            std::pow(position[2] - artPos[2], 2)
        );
        
        if (distance < minDistance) {
            minDistance = distance;
            nearest = artwork;
        }
    }
    
    return nearest;
}

// Get artwork by ID
Artwork* ArtworkManager::getArtwork(int id) {
    if (id >= 0 && id < artworks.size()) {
        return artworks[id];
    }
    return nullptr;
}

// Get total number of artworks
size_t ArtworkManager::getArtworkCount() const {
    return artworks.size();
}
