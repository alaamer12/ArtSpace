#include "puzzle.h"

PuzzleSystem::PuzzleSystem() {
    isComplete = false;
    completionProgress = 0.0f;
}

PuzzleSystem::~PuzzleSystem() {
    // Note: Artworks are owned by ArtworkManager, no need to delete them here
    puzzleArtworks.clear();
}

void PuzzleSystem::addArtwork(Artwork* artwork) {
    if (artwork) {
        puzzleArtworks.push_back(artwork);
    }
}

void PuzzleSystem::removeArtwork(Artwork* artwork) {
    // TODO: Implement artwork removal
}

bool PuzzleSystem::checkCompletion() {
    if (puzzleArtworks.empty()) return false;

    int solvedCount = 0;
    for (const auto& artwork : puzzleArtworks) {
        if (artwork->getIsSolved()) {
            solvedCount++;
        }
    }

    completionProgress = static_cast<float>(solvedCount) / puzzleArtworks.size();
    isComplete = (completionProgress >= 1.0f);
    return isComplete;
}

float PuzzleSystem::getProgress() const {
    return completionProgress;
}

void PuzzleSystem::reset() {
    // TODO: Reset puzzle state
    isComplete = false;
    completionProgress = 0.0f;
}

void PuzzleSystem::update(float deltaTime) {
    // TODO: Update puzzle state
    // TODO: Check for completion
    // TODO: Update hints if enabled
}
