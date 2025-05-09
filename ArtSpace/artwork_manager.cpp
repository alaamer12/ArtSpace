#include "artwork_manager.h"
#include <cmath>

ArtworkManager* ArtworkManager::instance = nullptr;

ArtworkManager::ArtworkManager() {
}

ArtworkManager* ArtworkManager::getInstance() {
    if (instance == nullptr) {
        instance = new ArtworkManager();
    }
    return instance;
}

ArtworkManager::~ArtworkManager() {
    clear();
}

Artwork* ArtworkManager::createArtwork(const std::string& texturePath, float x, float y, float z) {
    return 0;
}

void ArtworkManager::removeArtwork(Artwork* artwork) {
    // TODO: Implement artwork removal with proper cleanup
}

void ArtworkManager::clear() {
    for (auto artwork : artworks) {
        delete artwork;
    }
    artworks.clear();
}

void ArtworkManager::renderAll() {
    for (auto artwork : artworks) {
        artwork->render();
    }
}

void ArtworkManager::updateAll(float deltaTime) {
    for (auto artwork : artworks) {
       // artwork->update(deltaTime);
    }
}

Artwork* ArtworkManager::findNearestArtwork(const float* position, float maxDistance) {
    Artwork* nearest = nullptr;
    float minDistSq = maxDistance * maxDistance;

    for (auto artwork : artworks) {
        const float* artPos = artwork->getPosition();
        float dx = artPos[0] - position[0];
        float dy = artPos[1] - position[1];
        float dz = artPos[2] - position[2];
        float distSq = dx*dx + dy*dy + dz*dz;

        if (distSq < minDistSq) {
            minDistSq = distSq;
            nearest = artwork;
        }
    }

    return nearest;
}
