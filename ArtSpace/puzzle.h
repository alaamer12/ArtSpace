#pragma once
#include <vector>
#include "artwork.h"

class PuzzleSystem {
private:
    std::vector<Artwork*> puzzleArtworks;
    bool isComplete;
    float completionProgress;

public:
    PuzzleSystem();
    ~PuzzleSystem();

    // Core puzzle functionality
    void addArtwork(Artwork* artwork);
    void removeArtwork(Artwork* artwork);
    bool checkCompletion();
    float getProgress() const;

    // Puzzle state management
    void reset();
    void update(float deltaTime);
    
    // TODO: Add difficulty levels
    // TODO: Add hint system
    // TODO: Add completion rewards/effects
};
