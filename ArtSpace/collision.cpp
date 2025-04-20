#include "collision.h"
#include <cmath>

CollisionSystem* CollisionSystem::instance = nullptr;

CollisionSystem::CollisionSystem() : room(nullptr) {
}

CollisionSystem* CollisionSystem::getInstance() {
    if (instance == nullptr) {
        instance = new CollisionSystem();
    }
    return instance;
}

CollisionSystem::~CollisionSystem() {
    collidableObjects.clear();
}

void CollisionSystem::setRoom(Room* room) {
    this->room = room;
}

void CollisionSystem::addCollidableObject(Artwork* artwork) {
    if (artwork) {
        collidableObjects.push_back(artwork);
    }
}

void CollisionSystem::removeCollidableObject(Artwork* artwork) {
    // TODO: Implement object removal
}

bool CollisionSystem::checkCollision(const float* position, float radius) {
    return checkWallCollision(position, radius) || checkObjectCollision(position, radius);
}

bool CollisionSystem::checkWallCollision(const float* position, float radius) {
    if (!room) return false;
    
    const float* dimensions = room->getDimensions();
    
    // Simple AABB collision with room boundaries
    return (position[0] - radius < -dimensions[0]/2 ||
            position[0] + radius > dimensions[0]/2 ||
            position[2] - radius < -dimensions[2]/2 ||
            position[2] + radius > dimensions[2]/2);
}

bool CollisionSystem::checkObjectCollision(const float* position, float radius) {
    // TODO: Implement artwork collision detection
    return false;
}

bool CollisionSystem::isValidPosition(const float* position, float radius) {
    return !checkCollision(position, radius);
}

void CollisionSystem::adjustPosition(float* position, float radius) {
    if (!room) return;
    
    const float* dimensions = room->getDimensions();
    
    // Clamp position within room boundaries
    position[0] = std::max(-dimensions[0]/2 + radius, 
                          std::min(dimensions[0]/2 - radius, position[0]));
    position[2] = std::max(-dimensions[2]/2 + radius, 
                          std::min(dimensions[2]/2 - radius, position[2]));
}
