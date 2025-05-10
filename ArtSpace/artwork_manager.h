#pragma once
#include <vector>
#include <string>
#include "artwork.h"

struct ArtworkConfig {
    // Position
    float posX = 0.0f, posY = 0.0f, posZ = 0.0f;
    
    // Dimensions
    float width = 0.003f, height = 0.003f;
    
    // Placement
    ArtworkPlacement placement = NORTH_WALL;
    
    // Rotation
    bool hasRotation = false;
    float rotAngle = 0.0f;
    float rotX = 0.0f, rotY = 1.0f, rotZ = 0.0f;
    
    // Image stretching
    bool hasImageStretch = false;
    float imageStretchX = 1.0f, imageStretchY = 1.0f;
    
    // Frame stretching
    bool hasFrameStretch = false;
    float frameStretchX = 1.0f, frameStretchY = 1.0f;
};

class ArtworkManager {
private:
    static ArtworkManager* instance;
    std::vector<Artwork*> artworks;
    
    ArtworkManager();  // Private constructor for singleton

public:
    static ArtworkManager* getInstance();
    ~ArtworkManager();

    // Artwork management
    Artwork* createArtwork(const std::string& imagePath, float x, float y, float z);
    Artwork* createArtwork(const std::string& imagePath, const std::string& framePath, 
                          float x, float y, float z,
                          float width = 0.003f, float height = 0.003f, 
                          ArtworkPlacement placement = NORTH_WALL);
    Artwork* createArtworkFromConfig(const std::string& imagePath, const std::string& framePath, 
                                    const ArtworkConfig& config);
    void removeArtwork(Artwork* artwork);
    void clear();

    Artwork* getArtwork(int id);
    size_t getArtworkCount() const;


    void renderAll();
    void updateAll(float deltaTime);

    Artwork* findNearestArtwork(const float* position, float maxDistance);
};
