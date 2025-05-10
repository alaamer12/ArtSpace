#pragma once

#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "camera.h"
#include "artwork.h"
#include "artwork_manager.h"
#include "room.h"
#include "input.h"
#include "config.h"
#include "lever.h"
#include "sound_manager.h"


enum ArtworkID {
    ARTWORK_MEGATRON_ONE,
    ARTWORK_MEGATRON_PRIME,
    ARTWORK_STARSCREAM,
    ARTWORK_COUNT
};


class GameManager {
private:
    // Singleton pattern
    static GameManager* instance;
    
    std::string basePath4P;
    std::string basePath4T;
    std::string basePath4F;
    
    HumanCamera* camera;
    Room* room;
    InputSystem* inputSystem;
    ArtworkManager* artworkManager;
    Lever* lever;
    SoundManager* soundManager;
    float lastTime;
    
    std::string* imageID;
    std::string* frameID;
    ArtworkConfig* artworkConfigs;
    
    int closestArtworkID;
    float closestArtworkDistance;
    bool debugProximity;
    
    std::vector<int> artworkIndexToID;
    
    bool gameWon;
    float winTimer;
    std::vector<float> artworkRotations;
    
    // Win animation states
    bool winAnimationActive;
    float winAnimationSpeed;
    float totalWinRotation;
    bool winFlashingLights;
    float flashTimer;
    
    GameManager();
    
    void initArtworkConfigs();
    
    // Calculate distance between two 3D points
    float calculateDistance(float x1, float y1, float z1, float x2, float y2, float z2);
    
    // Calculate a more perceptual distance between camera and artwork
    float calculateArtworkDistance(Artwork* artwork, float cameraX, float cameraY, float cameraZ);
    
    // Find closest artwork to camera
    void updateClosestArtwork();
    
    std::string getArtworkName(int id);
    
    void rotateClosestArtwork(float angle);
    
    // Check if win condition is met (all artworks at 0 rotation)
    bool checkWinCondition();
    
    // Handle win state (print WIN message and exit)
    void handleWinState(float deltaTime);
    
public:
    ~GameManager();
    
    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;
    
    static GameManager* getInstance();
    
    // Initialization methods
    void initPaths();
    void initRoom();
    void initCamera();
    void initArtworks();
    void initLever();
    void init();
    
    // Main game loop methods
    void update(float deltaTime);
    float getDeltaTime();
    
    void handleKeyPress(unsigned char key, int x, int y);
    void handleKeyRelease(unsigned char key, int x, int y);
    void handleSpecialKeyPress(int key, int x, int y);
    void handleSpecialKeyRelease(int key, int x, int y);
    void handleMouseMotion(int x, int y);
    void handleMouseButton(int button, int state, int x, int y);
    
    void render();
    void cleanup();
    
    // Utility functions
    void printControls();
};

// Initialize static instance to nullptr
GameManager* GameManager::instance = nullptr;

// Constructor
GameManager::GameManager() 
    : camera(nullptr), room(nullptr), inputSystem(nullptr), artworkManager(nullptr), lever(nullptr), 
      soundManager(nullptr), lastTime(0.0f),
      closestArtworkID(-1), closestArtworkDistance(999999.0f), debugProximity(false),
      gameWon(false), winTimer(0.0f),
      winAnimationActive(false), winAnimationSpeed(0.0f), totalWinRotation(0.0f), winFlashingLights(false), flashTimer(0.0f) {
    imageID = new std::string[ARTWORK_COUNT];
    frameID = new std::string[ARTWORK_COUNT];
    artworkConfigs = new ArtworkConfig[ARTWORK_COUNT];
    
    srand(static_cast<unsigned int>(time(nullptr)));
    
}

GameManager::~GameManager() {
    cleanup();
    
    delete[] imageID;
    delete[] frameID;
    delete[] artworkConfigs;
}

GameManager* GameManager::getInstance() {
    if (instance == nullptr) {
        instance = new GameManager();
    }
    return instance;
}

float GameManager::calculateDistance(float x1, float y1, float z1, float x2, float y2, float z2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float dz = z2 - z1;
    return std::sqrt(dx*dx + dy*dy + dz*dz);
}

std::string GameManager::getArtworkName(int id) {
    switch (id) {
        case ARTWORK_MEGATRON_ONE: return "Megatron One";
        case ARTWORK_MEGATRON_PRIME: return "Megatron Prime";
        case ARTWORK_STARSCREAM: return "StarScream";
        default: return "Unknown";
    }
}

float GameManager::calculateArtworkDistance(Artwork* artwork, float cameraX, float cameraY, float cameraZ) {
    if (!artwork) return 999999.0f;
    
    float* artPos = artwork->getPosition();
    
    return calculateDistance(cameraX, cameraY, cameraZ, artPos[0], artPos[1], artPos[2]);
    
}

void GameManager::updateClosestArtwork() {
    if (!camera || !artworkManager || artworkManager->getArtworkCount() == 0) {
        return;
    }
    
    float cameraPos[3];
    camera->getPosition(cameraPos);
    
    float closestDist = 999999.0f;
    int closestIdx = -1;

    // Check each artwork
    for (size_t i = 0; i < artworkManager->getArtworkCount(); i++) {
        Artwork* artwork = artworkManager->getArtwork(i);
        if (artwork) {
            float dist = calculateArtworkDistance(artwork, cameraPos[0], cameraPos[1], cameraPos[2]);
            
            // In debug mode, output all distances
            if (debugProximity) {
                float* artPos = artwork->getPosition();
                int artID = i < artworkIndexToID.size() ? artworkIndexToID[i] : i;
                std::cout << "Artwork " << artID << " (" << getArtworkName(artID) << "): " << std::endl;
                std::cout << "  Position: " << artPos[0] << ", " << artPos[1] << ", " << artPos[2] << std::endl;
                std::cout << "  Distance: " << std::fixed << std::setprecision(2) << dist << std::endl;
            }
            
            if (dist < closestDist) {
                closestDist = dist;
                closestIdx = i;
            }
        }
    }
    
    
    // Map the closestIdx to the correct artwork ID
    int artworkID = closestIdx < artworkIndexToID.size() ? artworkIndexToID[closestIdx] : closestIdx;
    
    // Only update and print if the closest artwork changed or distance changed significantly
    if (artworkID != closestArtworkID || std::abs(closestDist - closestArtworkDistance) > 0.05f) {
        closestArtworkID = artworkID;
        closestArtworkDistance = closestDist;
        
        // Print the closest artwork info
        std::cout << "Closest artwork: " << getArtworkName(closestArtworkID) << ", ";
        std::cout << "distance: " << std::fixed << std::setprecision(2) << closestArtworkDistance << " units" << std::endl;
    }
}

// Initialize paths
void GameManager::initPaths() {
    basePath4P = "assets\\pictures\\";
    basePath4T = "assets\\textures\\";
    basePath4F = "assets\\textures\\frames\\";
    
    // Set up image paths
    imageID[ARTWORK_MEGATRON_ONE] = basePath4P + "Megatron One (1).jpg";
    imageID[ARTWORK_MEGATRON_PRIME] = basePath4P + "Megatron Prime.jpg";
    imageID[ARTWORK_STARSCREAM] = basePath4P + "StarScream.jpg";
    
    // Set up frame paths
    frameID[ARTWORK_MEGATRON_ONE] = basePath4F + "Luxury.png";
    frameID[ARTWORK_MEGATRON_PRIME] = basePath4F + "Legacy.png";
    frameID[ARTWORK_STARSCREAM] = basePath4F + "Precious.png";
}

// Initialize artwork configurations
void GameManager::initArtworkConfigs() {
    // ARTWORK_MEGATRON_ONE configuration
    artworkConfigs[ARTWORK_MEGATRON_ONE].posX = 0.0f;
    artworkConfigs[ARTWORK_MEGATRON_ONE].posY = 0.0f;
    artworkConfigs[ARTWORK_MEGATRON_ONE].posZ = -14.9f;
    artworkConfigs[ARTWORK_MEGATRON_ONE].width = 0.003f;
    artworkConfigs[ARTWORK_MEGATRON_ONE].height = 0.003f;
    artworkConfigs[ARTWORK_MEGATRON_ONE].placement = NORTH_WALL;
    artworkConfigs[ARTWORK_MEGATRON_ONE].hasRotation = true;
    
    // Initialize with random rotation (modulo 15 degrees)
    float randomRotation = (rand() % 12) * 15.0f;
    artworkConfigs[ARTWORK_MEGATRON_ONE].rotAngle = randomRotation;
    artworkConfigs[ARTWORK_MEGATRON_ONE].rotX = 0.0f;
    artworkConfigs[ARTWORK_MEGATRON_ONE].rotY = 0.0f;
    artworkConfigs[ARTWORK_MEGATRON_ONE].rotZ = 1.0f;
    
    // Set image and frame stretch
    artworkConfigs[ARTWORK_MEGATRON_ONE].hasImageStretch = true;
    artworkConfigs[ARTWORK_MEGATRON_ONE].imageStretchX = 1.40f;
    artworkConfigs[ARTWORK_MEGATRON_ONE].imageStretchY = 1.30f;
    artworkConfigs[ARTWORK_MEGATRON_ONE].hasFrameStretch = true;
    artworkConfigs[ARTWORK_MEGATRON_ONE].frameStretchX = 0.90f;
    artworkConfigs[ARTWORK_MEGATRON_ONE].frameStretchY = 1.0f;
    
    // ARTWORK_MEGATRON_PRIME configuration
    artworkConfigs[ARTWORK_MEGATRON_PRIME].posX = 1.0f;
    artworkConfigs[ARTWORK_MEGATRON_PRIME].posY = 0.0f;
    artworkConfigs[ARTWORK_MEGATRON_PRIME].posZ = -14.9f;
    artworkConfigs[ARTWORK_MEGATRON_PRIME].width = 0.005f;
    artworkConfigs[ARTWORK_MEGATRON_PRIME].height = 0.005f;
    artworkConfigs[ARTWORK_MEGATRON_PRIME].placement = SOUTH_WALL;
    artworkConfigs[ARTWORK_MEGATRON_PRIME].hasRotation = true;
    
    // Initialize with random rotation (modulo 15 degrees)
    randomRotation = (rand() % 12) * 15.0f;
    artworkConfigs[ARTWORK_MEGATRON_PRIME].rotAngle = randomRotation;
    artworkConfigs[ARTWORK_MEGATRON_PRIME].rotX = 0.0f;
    artworkConfigs[ARTWORK_MEGATRON_PRIME].rotY = 0.0f;
    artworkConfigs[ARTWORK_MEGATRON_PRIME].rotZ = 1.0f;
    
    // Set image and frame stretch
    artworkConfigs[ARTWORK_MEGATRON_PRIME].hasImageStretch = true;
    artworkConfigs[ARTWORK_MEGATRON_PRIME].imageStretchX = 1.40f;
    artworkConfigs[ARTWORK_MEGATRON_PRIME].imageStretchY = 1.40f;
    artworkConfigs[ARTWORK_MEGATRON_PRIME].hasFrameStretch = true;
    artworkConfigs[ARTWORK_MEGATRON_PRIME].frameStretchX = 0.60f;
    artworkConfigs[ARTWORK_MEGATRON_PRIME].frameStretchY = 0.60f;
    
    // ARTWORK_STARSCREAM configuration
    artworkConfigs[ARTWORK_STARSCREAM].posX = 5.0f;
    artworkConfigs[ARTWORK_STARSCREAM].posY = 0.0f;
    artworkConfigs[ARTWORK_STARSCREAM].posZ = -14.9f;
    artworkConfigs[ARTWORK_STARSCREAM].width = 0.005f;
    artworkConfigs[ARTWORK_STARSCREAM].height = 0.005f;
    artworkConfigs[ARTWORK_STARSCREAM].placement = WEST_WALL;
    artworkConfigs[ARTWORK_STARSCREAM].hasRotation = true;
    
    // Initialize with random rotation (modulo 15 degrees)
    randomRotation = (rand() % 12) * 15.0f;
    artworkConfigs[ARTWORK_STARSCREAM].rotAngle = randomRotation;
    artworkConfigs[ARTWORK_STARSCREAM].rotX = 0.0f;
    artworkConfigs[ARTWORK_STARSCREAM].rotY = 0.0f;
    artworkConfigs[ARTWORK_STARSCREAM].rotZ = 1.0f;
    
    artworkConfigs[ARTWORK_STARSCREAM].hasImageStretch = true;
    artworkConfigs[ARTWORK_STARSCREAM].imageStretchX = 0.90f;
    artworkConfigs[ARTWORK_STARSCREAM].imageStretchY = 0.80f;
    artworkConfigs[ARTWORK_STARSCREAM].hasFrameStretch = true;
    artworkConfigs[ARTWORK_STARSCREAM].frameStretchX = 1.40f;
    artworkConfigs[ARTWORK_STARSCREAM].frameStretchY = 1.10f;
    
    // Clear and initialize our rotation tracking
    artworkRotations.clear();
    
    // Print initial artwork configurations
    std::cout << "Initial artwork rotations: " << std::endl;
    std::cout << "  Megatron One: " << artworkConfigs[ARTWORK_MEGATRON_ONE].rotAngle << "°" << std::endl;
    std::cout << "  Megatron Prime: " << artworkConfigs[ARTWORK_MEGATRON_PRIME].rotAngle << "°" << std::endl;
    std::cout << "  StarScream: " << artworkConfigs[ARTWORK_STARSCREAM].rotAngle << "°" << std::endl;
}

// Initialize room
void GameManager::initRoom() {
    room = new Room(30.0f, 16.0f, 30.0f);

    std::string wallTexturePath = basePath4T + "wall4.bmp";
    std::string floorTexturePath = basePath4T + "floor3.bmp";
    std::string roofTexturePath = basePath4T + "wall5.bmp";

    room->setWallTexture(wallTexturePath);
    room->setFloorTexture(floorTexturePath);
    room->setRoofTexture(roofTexturePath);
}

void GameManager::initCamera() {
    camera = new HumanCamera();
    camera->setPosition(0.0f, 0.0f, 3.0f);
}

void GameManager::initArtworks() {
    artworkIndexToID.clear();
    
    artworkRotations.clear();
    
    // Create artwork #1 using config
    Artwork* painting1 = artworkManager->createArtworkFromConfig(
        imageID[ARTWORK_MEGATRON_ONE], 
        frameID[ARTWORK_MEGATRON_ONE], 
        artworkConfigs[ARTWORK_MEGATRON_ONE]
    );
    artworkIndexToID.push_back(ARTWORK_MEGATRON_ONE);
    artworkRotations.push_back(artworkConfigs[ARTWORK_MEGATRON_ONE].rotAngle);
    
    // Create artwork #2 using config but with manual adjustment
    ArtworkConfig config = artworkConfigs[ARTWORK_MEGATRON_PRIME];
    Artwork* painting2 = artworkManager->createArtworkFromConfig(
        imageID[ARTWORK_MEGATRON_PRIME], 
        frameID[ARTWORK_MEGATRON_PRIME], 
        config
    );
    artworkIndexToID.push_back(ARTWORK_MEGATRON_PRIME);
    artworkRotations.push_back(artworkConfigs[ARTWORK_MEGATRON_PRIME].rotAngle);
    
    // Apply additional manual adjustments
    painting2->translate(-4.0f, 0.0f, 0.0f);
    
    // Create artwork #3 using config
    Artwork* painting3 = artworkManager->createArtworkFromConfig(
        imageID[ARTWORK_STARSCREAM], 
        frameID[ARTWORK_STARSCREAM], 
        artworkConfigs[ARTWORK_STARSCREAM]
    );
    artworkIndexToID.push_back(ARTWORK_STARSCREAM);
    artworkRotations.push_back(artworkConfigs[ARTWORK_STARSCREAM].rotAngle);
    
    // Adjust Y positions of all artworks to be at eye level
    for (size_t i = 0; i < artworkManager->getArtworkCount(); i++) {
        Artwork* artwork = artworkManager->getArtwork(i);
        float* pos = artwork->getPosition();
        artwork->setPosition(pos[0], 1.0f, pos[2]);
    }
}

void GameManager::initLever() {
    float leverX = -15.0f;
    float leverY = 1.0f; 
    float leverZ = 0.0f; 
    
    lever = new Lever(leverX, leverY, leverZ, 0.5f);
}

void GameManager::init() {
    inputSystem = InputSystem::getInstance();
    
    artworkManager = ArtworkManager::getInstance();
    
    initPaths();
    
    initArtworkConfigs();
    
    // Initialize room
    initRoom();
    
    // Initialize camera
    initCamera();
    
    // Initialize artworks
    initArtworks();
    
    // Initialize lever
    initLever();
    
    // Initialize lastTime
    lastTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    
    // Initialize sound manager
    soundManager = SoundManager::getInstance();
    if (!soundManager->init()) {
        std::cerr << "Failed to initialize sound manager!" << std::endl;
    }
    // Play the start game sound
    soundManager->playGameStartSound();
    
    // Print controls
    printControls();
    
    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

float GameManager::getDeltaTime() {
    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    if (deltaTime > 0.1f) deltaTime = 0.1f;
    return deltaTime;
}

void GameManager::update(float deltaTime) {
    camera->update(deltaTime);

    // Constrain camera to room boundaries
    const float* roomDimensions = room->getDimensions();
    float roomWidth = roomDimensions[0];
    float roomHeight = roomDimensions[1];
    float roomDepth = roomDimensions[2];

    float pos[3];
    camera->getPosition(pos);

    float wallOffset = 0.2f;
    float floorOffset = 0.1f;

    if (pos[0] < -roomWidth / 2 + wallOffset) pos[0] = -roomWidth / 2 + wallOffset;
    if (pos[0] > roomWidth / 2 - wallOffset) pos[0] = roomWidth / 2 - wallOffset;
    if (pos[2] < -roomDepth / 2 + wallOffset) pos[2] = -roomDepth / 2 + wallOffset;
    if (pos[2] > roomDepth / 2 - wallOffset) pos[2] = roomDepth / 2 - wallOffset;
    if (pos[1] < -roomHeight / 2 + 1.7f + floorOffset) pos[1] = -roomHeight / 2 + 1.7f + floorOffset;

    camera->setPosition(pos[0], pos[1], pos[2]);
    
    // Update closest artwork tracking
    updateClosestArtwork();
    
    // Update lever animation
    if (lever) {
        lever->update(deltaTime);
    }
    
    // Handle win state if game is won
    if (gameWon) {
        handleWinState(deltaTime);
    }
}

void GameManager::render() {
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Apply camera transformation
    camera->applyTransformation();
    
    // Render room
    room->render();
    
    // Render all artworks
    artworkManager->renderAll();
    
    // Render lever
    if (lever) {
        lever->render();
    }
}

void GameManager::handleKeyPress(unsigned char key, int x, int y) {
    inputSystem->handleKeyPress(key, x, y);
    
    if (key == ' ') {
        if (lever) {
            // Check if the win condition is met
            bool winConditionMet = checkWinCondition();
            
            // Activate the lever with the current win condition
            lever->activate(winConditionMet);
            
            // Play appropriate sound effect
            if (winConditionMet) {
                // User has won
                std::cout << "Correct answer! All artworks are aligned properly." << std::endl;
                soundManager->playGameOverSound();
                gameWon = true;
                winTimer = 0.0f;
            } else {
                // User has not won
                std::cout << "Incorrect! Not all artworks are aligned properly." << std::endl;
                soundManager->playWrongSound();
            }
            glutPostRedisplay();
        }
        return;
    }
    
    // Artwork rotation keys
    if (key == 'k') {
        // Rotate counterclockwise by -15 degrees
        rotateClosestArtwork(15.0f);
        glutPostRedisplay();
        return;
    } else if (key == 'l') {
        // Rotate clockwise by 15 degrees
        rotateClosestArtwork(-15.0f);
        glutPostRedisplay();
        return;
    }
    
    if (artworkManager->getArtworkCount() > 0 && closestArtworkID >= 0 && closestArtworkDistance <= 25.0f) {
        // Get the closest artwork for stretching
        int artworkIndex = -1;
        for (size_t i = 0; i < artworkIndexToID.size(); i++) {
            if (artworkIndexToID[i] == closestArtworkID) {
                artworkIndex = i;
                break;
            }
        }
        
        if (artworkIndex >= 0 && artworkIndex < artworkManager->getArtworkCount()) {
            Artwork* art = artworkManager->getArtwork(artworkIndex);
            
        } else {
            std::cout << "No artwork selected or too far away to modify." << std::endl;
        }
    }
}

void GameManager::handleKeyRelease(unsigned char key, int x, int y) {
    inputSystem->handleKeyRelease(key, x, y);
}

void GameManager::handleSpecialKeyPress(int key, int x, int y) {
    inputSystem->handleSpecialKeyPress(key, x, y);
}

void GameManager::handleSpecialKeyRelease(int key, int x, int y) {
    inputSystem->handleSpecialKeyRelease(key, x, y);
}

void GameManager::handleMouseMotion(int x, int y) {
    inputSystem->handleMouseMotion(x, y);
}

// Handle mouse button
void GameManager::handleMouseButton(int button, int state, int x, int y) {
    // Currently not implemented
}

// Print game controls
void GameManager::printControls() {
    std::cout << "ArtSpace Room & Camera Demo" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  WASD - Move" << std::endl;
    std::cout << "  Mouse - Look around" << std::endl;
    std::cout << "  ESC - Exit" << std::endl;
    std::cout << std::endl;
    std::cout << "Artwork Rotation Controls:" << std::endl;
    std::cout << "  k - Rotate closest artwork counterclockwise by 15 degrees" << std::endl;
    std::cout << "  l - Rotate closest artwork clockwise by 15 degrees" << std::endl;
    std::cout << std::endl;
    std::cout << "Win Condition:" << std::endl;
    std::cout << "  Rotate all artworks to be vertical (0 degrees rotation)" << std::endl;
    std::cout << "  Press SPACEBAR to check your answer once you think you're done" << std::endl;
    std::cout << std::endl;
}

void GameManager::cleanup() {
    // Clean up camera
    if (camera) {
        delete camera;
        camera = nullptr;
    }

    // Clean up room
    if (room) {
        delete room;
        room = nullptr;
    }
    
    // Clean up lever
    if (lever) {
        delete lever;
        lever = nullptr;
    }
    
    // Clear the tracking vectors
    artworkIndexToID.clear();
    artworkRotations.clear();
}

void GameManager::rotateClosestArtwork(float angle) {
    if (closestArtworkID < 0 || closestArtworkDistance > 28.0f) {
        // No artwork nearby or too far away
        std::cout << "Too far from artwork to rotate. Current distance: " 
                  << closestArtworkDistance << " units" << std::endl;
        return;
    }
    
    int artworkIndex = -1;
    for (size_t i = 0; i < artworkIndexToID.size(); i++) {
        if (artworkIndexToID[i] == closestArtworkID) {
            artworkIndex = i;
            break;
        }
    }
    
    if (artworkIndex >= 0 && artworkIndex < artworkManager->getArtworkCount()) {
        Artwork* artwork = artworkManager->getArtwork(artworkIndex);
        if (artwork) {
            // Update our tracked rotation value
            artworkRotations[artworkIndex] += angle;
            
            // Normalize angle to 0-360 range
            while (artworkRotations[artworkIndex] < 0) artworkRotations[artworkIndex] += 360.0f;
            while (artworkRotations[artworkIndex] >= 360.0f) artworkRotations[artworkIndex] -= 360.0f;
            
            // Apply rotation to the artwork
            artwork->rotate(artworkRotations[artworkIndex], 0.0f, 0.0f, 1.0f);
        }
    }
}

// Check if all artworks are at 0 rotation (vertically aligned)
bool GameManager::checkWinCondition() {
    for (size_t i = 0; i < artworkRotations.size(); i++) {
        // Normalize angle to 0-360 range
        float angle = artworkRotations[i];
        while (angle < 0) angle += 360.0f;
        while (angle >= 360.0f) angle -= 360.0f;
        
        // If any artwork is not at 0 degrees, win condition is not met
        if (fabs(angle) > 0.01f) {
            return false;
        }
    }
    return true;
}

void win_animation(
    bool& winAnimationActive,
    float& totalWinRotation,
    ArtworkManager* artworkManager,
    std::vector<float>& artworkRotations,
    bool winFlashingLights,
    float& flashTimer,
    float winTimer,
    float deltaTime
) {
    // Victory animation: rotate all artworks simultaneously
    if (winAnimationActive) {
        float rotationAmount = 180.0f * deltaTime;
        totalWinRotation += rotationAmount;

        for (size_t i = 0; i < artworkManager->getArtworkCount(); i++) {
            Artwork* artwork = artworkManager->getArtwork(i);
            if (artwork) {
                artworkRotations[i] += rotationAmount;
                while (artworkRotations[i] >= 360.0f) artworkRotations[i] -= 360.0f;
                artwork->rotate(artworkRotations[i], 0.0f, 0.0f, 1.0f);
            }
        }

        if (totalWinRotation >= 1080.0f) {
            winAnimationActive = false;
            for (size_t i = 0; i < artworkManager->getArtworkCount(); i++) {
                Artwork* artwork = artworkManager->getArtwork(i);
                if (artwork) {
                    artworkRotations[i] = 0.0f;
                    artwork->rotate(0.0f, 0.0f, 0.0f, 1.0f);
                }
            }
        }
    }

    if (winFlashingLights && flashTimer >= 0.3f) {
        flashTimer = 0.0f;
        GLfloat r = static_cast<GLfloat>(rand()) / RAND_MAX;
        GLfloat g = static_cast<GLfloat>(rand()) / RAND_MAX;
        GLfloat b = static_cast<GLfloat>(rand()) / RAND_MAX;
        GLfloat ambient[] = { r, g, b, 1.0f };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
        glutPostRedisplay();
    }

    if (winTimer >= 5.0f) {
        GLfloat defaultAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, defaultAmbient);
        std::cout << "Game completed successfully!" << std::endl;
        exit(0);
    }
}


void GameManager::handleWinState(float deltaTime) {
    if (!gameWon) return;

    if (winTimer == 0.0f) {
        winAnimationActive = true;
        winAnimationSpeed = 180.0f;
        totalWinRotation = 0.0f;
        winFlashingLights = true;
        flashTimer = 0.0f;
        std::cout << "\n\nYOU WIN!\n\n" << std::endl;
        std::cout << "Enjoy the victory animation..." << std::endl;
    }

    winTimer += deltaTime;
    flashTimer += deltaTime;

    win_animation(
        winAnimationActive,
        totalWinRotation,
        artworkManager,
        artworkRotations,
        winFlashingLights,
        flashTimer,
        winTimer,
        deltaTime
    );
}
