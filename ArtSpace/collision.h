#pragma once
#include "room.h"
#include "artwork.h"
#include <vector>

class CollisionSystem {
private:
    static CollisionSystem* instance;
    Room* room;
    std::vector<Artwork*> collidableObjects;

    CollisionSystem();  // Private constructor for singleton

public:
    static CollisionSystem* getInstance();
    ~CollisionSystem();

    void setRoom(Room* room);
    void addCollidableObject(Artwork* artwork);
    void removeCollidableObject(Artwork* artwork);
    
    // Collision checks
    bool checkCollision(const float* position, float radius);
    bool checkWallCollision(const float* position, float radius);
    bool checkObjectCollision(const float* position, float radius);
    
    // Movement validation
    bool isValidPosition(const float* position, float radius);
    void adjustPosition(float* position, float radius);
    
    // TODO: Add spatial partitioning for performance
    // TODO: Add collision response system
    // TODO: Add debug visualization
};
