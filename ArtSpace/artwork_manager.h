#pragma once
#include <vector>
#include <string>
#include "artwork.h"

class ArtworkManager {
private:
    static ArtworkManager* instance;
    std::vector<Artwork*> artworks;
    
    ArtworkManager();  // Private constructor for singleton

public:
    static ArtworkManager* getInstance();
    ~ArtworkManager();

    // Artwork management
    Artwork* createArtwork(const std::string& texturePath, float x, float y, float z);
    void removeArtwork(Artwork* artwork);
    void clear();

    // Rendering and updates
    void renderAll();
    void updateAll(float deltaTime);

    // Interaction
    Artwork* findNearestArtwork(const float* position, float maxDistance);
    
    // TODO: Add artwork loading from configuration
    // TODO: Add artwork placement validation
    // TODO: Add artwork grouping for puzzle sequences
};
