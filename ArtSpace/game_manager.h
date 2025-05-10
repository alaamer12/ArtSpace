#pragma once

#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include "camera.h"
#include "artwork.h"
#include "artwork_manager.h"
#include "room.h"
#include "input.h"
#include "config.h"
#include "lever.h"
#include "sound_manager.h"

// Define artwork IDs for easy reference
enum ArtworkID {
    ARTWORK_MEGATRON_ONE,
    ARTWORK_MEGATRON_PRIME,
    ARTWORK_STARSCREAM,
    // Add more artwork IDs as needed
    ARTWORK_COUNT
};

// Single Game Manager class to encapsulate all game logic
class GameManager {
private:
    // Singleton pattern
    static GameManager* instance;
    
    // Path definitions
    std::string basePath4P;
    std::string basePath4T;
    std::string basePath4F;
    
    // Game objects
    HumanCamera* camera;
    Room* room;
    InputSystem* inputSystem;
    ArtworkManager* artworkManager;
    Lever* lever;
    SoundManager* soundManager;
    float lastTime;
    
    // Array of image and frame paths
    std::string* imageID;
    std::string* frameID;
    ArtworkConfig* artworkConfigs;
    
    // Closest artwork tracking
    int closestArtworkID;
    float closestArtworkDistance;
    bool debugProximity;
    
    // Store the mapping between index in artworkManager and actual artwork IDs
    std::vector<int> artworkIndexToID;
    
    // Win condition tracking
    bool gameWon;
    float winTimer;
    std::vector<float> artworkRotations;
    
    // Win animation states
    bool winAnimationActive;
    float winAnimationSpeed;
    float totalWinRotation;
    bool winFlashingLights;
    float flashTimer;
    
    // Constructor is private for singleton
    GameManager();
    
    // Initialize artwork configurations
    void initArtworkConfigs();
    
    // Calculate distance between two 3D points
    float calculateDistance(float x1, float y1, float z1, float x2, float y2, float z2);
    
    // Calculate a more perceptual distance between camera and artwork
    float calculateArtworkDistance(Artwork* artwork, float cameraX, float cameraY, float cameraZ);
    
    // Find closest artwork to camera
    void updateClosestArtwork();
    
    // Get artwork name from ID
    std::string getArtworkName(int id);
    
    // Rotate closest artwork
    void rotateClosestArtwork(float angle);
    
    // Check if win condition is met (all artworks at 0 rotation)
    bool checkWinCondition();
    
    // Handle win state (print WIN message and exit)
    void handleWinState(float deltaTime);
    
public:
    // Destructor
    ~GameManager();
    
    // Delete copy constructor and assignment
    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;
    
    // Get singleton instance
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
    
    // Input handling
    void handleKeyPress(unsigned char key, int x, int y);
    void handleKeyRelease(unsigned char key, int x, int y);
    void handleSpecialKeyPress(int key, int x, int y);
    void handleSpecialKeyRelease(int key, int x, int y);
    void handleMouseMotion(int x, int y);
    void handleMouseButton(int button, int state, int x, int y);
    
    // Rendering
    void render();
    
    // Cleanup
    void cleanup();
    
    // Getters
    HumanCamera* getCamera() const { return camera; }
    Room* getRoom() const { return room; }
    ArtworkManager* getArtworkManager() const { return artworkManager; }
    InputSystem* getInputSystem() const { return inputSystem; }
    int getClosestArtworkID() const { return closestArtworkID; }
    float getClosestArtworkDistance() const { return closestArtworkDistance; }
    Lever* getLever() const { return lever; }
    
    // Toggle debug mode
    void toggleDebugProximity() { debugProximity = !debugProximity; }
    
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
    // Initialize arrays
    imageID = new std::string[ARTWORK_COUNT];
    frameID = new std::string[ARTWORK_COUNT];
    artworkConfigs = new ArtworkConfig[ARTWORK_COUNT];
    
    // Initialize random seed
    srand(static_cast<unsigned int>(time(nullptr)));
    
    // Init will be called separately
}

// Destructor
GameManager::~GameManager() {
    cleanup();
    
    // Clean up arrays
    delete[] imageID;
    delete[] frameID;
    delete[] artworkConfigs;
}

// Get singleton instance
GameManager* GameManager::getInstance() {
    if (instance == nullptr) {
        instance = new GameManager();
    }
    return instance;
}

// Calculate distance between two 3D points
float GameManager::calculateDistance(float x1, float y1, float z1, float x2, float y2, float z2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float dz = z2 - z1;
    return std::sqrt(dx*dx + dy*dy + dz*dz);
}

// Get artwork name from ID
std::string GameManager::getArtworkName(int id) {
    switch (id) {
        case ARTWORK_MEGATRON_ONE: return "Megatron One";
        case ARTWORK_MEGATRON_PRIME: return "Megatron Prime";
        case ARTWORK_STARSCREAM: return "StarScream";
        default: return "Unknown";
    }
}

// Calculate perceptual distance between camera and artwork
float GameManager::calculateArtworkDistance(Artwork* artwork, float cameraX, float cameraY, float cameraZ) {
    if (!artwork) return 999999.0f;
    
    float* artPos = artwork->getPosition();
    
    // Basic Euclidean distance - This is more reliable for identification
    return calculateDistance(cameraX, cameraY, cameraZ, artPos[0], artPos[1], artPos[2]);
    
}

// Find closest artwork to camera
void GameManager::updateClosestArtwork() {
    if (!camera || !artworkManager || artworkManager->getArtworkCount() == 0) {
        return;
    }
    
    float cameraPos[3];
    camera->getPosition(cameraPos);
    
    float closestDist = 999999.0f;
    int closestIdx = -1;
    
    // Debug output all artwork distances if in debug mode
    if (debugProximity) {
        std::cout << "---------- Artwork Distances ----------" << std::endl;
        std::cout << "Camera position: " << cameraPos[0] << ", " << cameraPos[1] << ", " << cameraPos[2] << std::endl;
    }
    
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
    
    if (debugProximity) {
        std::cout << "Closest: " << closestIdx << " with distance " << closestDist << std::endl;
        std::cout << "----------------------------------------" << std::endl;
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

// Initialize camera
void GameManager::initCamera() {
    camera = new HumanCamera();
    camera->setPosition(0.0f, 0.0f, 3.0f);
}

// Initialize artworks
void GameManager::initArtworks() {
    // Clear the artwork index to ID mapping
    artworkIndexToID.clear();
    
    // Clear rotation tracking
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
        artwork->setPosition(pos[0], 1.0f, pos[2]); // Setting y to be at eye level
    }

    std::cout << "Initialized " << artworkManager->getArtworkCount() << " artworks" << std::endl;
    std::cout << "Artwork mapping: " << std::endl;
    for (size_t i = 0; i < artworkIndexToID.size(); i++) {
        std::cout << "  Index " << i << " -> ID " << artworkIndexToID[i] 
                  << " (" << getArtworkName(artworkIndexToID[i]) << ")" 
                  << ", Rotation: " << artworkRotations[i] << "°" << std::endl;
    }
}

void GameManager::initLever() {
    float leverX = -15.0f;
    float leverY = 1.0f; 
    float leverZ = 0.0f; 
    
    lever = new Lever(leverX, leverY, leverZ, 0.5f);
    
    std::cout << "Initialized lever on West wall" << std::endl;
}

// Main initialization
void GameManager::init() {
    // Get input system instance
    inputSystem = InputSystem::getInstance();
    
    // Get artwork manager instance
    artworkManager = ArtworkManager::getInstance();
    
    // Initialize paths
    initPaths();
    
    // Initialize artwork configurations
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

// Get delta time
float GameManager::getDeltaTime() {
    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    if (deltaTime > 0.1f) deltaTime = 0.1f;
    return deltaTime;
}

// Update game state
void GameManager::update(float deltaTime) {
    
    // Update camera
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

// Render the game
void GameManager::render() {
    // Set clear color
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up modelview matrix
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

// Handle key press
void GameManager::handleKeyPress(unsigned char key, int x, int y) {
    // First pass the key to the input system for movement handling
    inputSystem->handleKeyPress(key, x, y);
    
    // Handle spacebar for lever activation and sound
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
        // Rotate counterclockwise by 15 degrees
        rotateClosestArtwork(-15.0f);
        glutPostRedisplay();
        return;
    } else if (key == 'l') {
        // Rotate clockwise by 15 degrees
        rotateClosestArtwork(15.0f);
        glutPostRedisplay();
        return;
    }
    
    // Toggle debug proximity with 'p' key
    if (key == 'p') {
        toggleDebugProximity();
        std::cout << "Debug proximity " << (debugProximity ? "enabled" : "disabled") << std::endl;
        return;
    }
    
    // Check if we have artwork to manipulate
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
            
            if (art) {
                switch (key) {
                    // Image stretching
                    case 'x': // Increase X stretch of image
                        art->stretchImage(art->getImageStretchX() + 0.1f, art->getImageStretchY());
                        std::cout << getArtworkName(closestArtworkID) << " - Image X stretch: " << art->getImageStretchX() << std::endl;
                        glutPostRedisplay();
                        break;
                    case 'X': // Decrease X stretch of image
                        art->stretchImage(art->getImageStretchX() - 0.1f, art->getImageStretchY());
                        std::cout << getArtworkName(closestArtworkID) << " - Image X stretch: " << art->getImageStretchX() << std::endl;
                        glutPostRedisplay();
                        break;
                    case 'y': // Increase Y stretch of image
                        art->stretchImage(art->getImageStretchX(), art->getImageStretchY() + 0.1f);
                        std::cout << getArtworkName(closestArtworkID) << " - Image Y stretch: " << art->getImageStretchY() << std::endl;
                        glutPostRedisplay();
                        break;
                    case 'Y': // Decrease Y stretch of image
                        art->stretchImage(art->getImageStretchX(), art->getImageStretchY() - 0.1f);
                        std::cout << getArtworkName(closestArtworkID) << " - Image Y stretch: " << art->getImageStretchY() << std::endl;
                        glutPostRedisplay();
                        break;
                        
                    // Frame stretching
                    case 'f': // Increase X stretch of frame
                        art->stretchFrame(art->getFrameStretchX() + 0.1f, art->getFrameStretchY());
                        std::cout << getArtworkName(closestArtworkID) << " - Frame X stretch: " << art->getFrameStretchX() << std::endl;
                        glutPostRedisplay();
                        break;
                    case 'F': // Decrease X stretch of frame
                        art->stretchFrame(art->getFrameStretchX() - 0.1f, art->getFrameStretchY());
                        std::cout << getArtworkName(closestArtworkID) << " - Frame X stretch: " << art->getFrameStretchX() << std::endl;
                        glutPostRedisplay();
                        break;
                    case 'g': // Increase Y stretch of frame
                        art->stretchFrame(art->getFrameStretchX(), art->getFrameStretchY() + 0.1f);
                        std::cout << getArtworkName(closestArtworkID) << " - Frame Y stretch: " << art->getFrameStretchY() << std::endl;
                        glutPostRedisplay();
                        break;
                    case 'G': // Decrease Y stretch of frame
                        art->stretchFrame(art->getFrameStretchX(), art->getFrameStretchY() - 0.1f);
                        std::cout << getArtworkName(closestArtworkID) << " - Frame Y stretch: " << art->getFrameStretchY() << std::endl;
                        glutPostRedisplay();
                        break;
                        
                    // Reset stretching
                    case 'r': // Reset image stretching
                        art->resetImageStretch();
                        std::cout << getArtworkName(closestArtworkID) << " - Image stretching reset" << std::endl;
                        glutPostRedisplay();
                        break;
                    case 'R': // Reset frame stretching
                        art->resetFrameStretch();
                        std::cout << getArtworkName(closestArtworkID) << " - Frame stretching reset" << std::endl;
                        glutPostRedisplay();
                        break;
                }
            }
        } else {
            std::cout << "No artwork selected or too far away to modify." << std::endl;
        }
    }
}

// Handle key release
void GameManager::handleKeyRelease(unsigned char key, int x, int y) {
    inputSystem->handleKeyRelease(key, x, y);
}

// Handle special key press
void GameManager::handleSpecialKeyPress(int key, int x, int y) {
    inputSystem->handleSpecialKeyPress(key, x, y);
}

// Handle special key release
void GameManager::handleSpecialKeyRelease(int key, int x, int y) {
    inputSystem->handleSpecialKeyRelease(key, x, y);
}

// Handle mouse motion
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
    std::cout << "  F - Toggle wireframe mode" << std::endl;
    std::cout << "  +/- - Adjust mouse sensitivity" << std::endl;
    std::cout << "  ESC - Exit" << std::endl;
    std::cout << std::endl;
    std::cout << "Artwork Rotation Controls:" << std::endl;
    std::cout << "  k - Rotate closest artwork counterclockwise by 15 degrees" << std::endl;
    std::cout << "  l - Rotate closest artwork clockwise by 15 degrees" << std::endl;
    std::cout << std::endl;
    std::cout << "Lever Controls:" << std::endl;
    std::cout << "  SPACEBAR - Activate the lever to check if all artworks are aligned" << std::endl;
    std::cout << "             Bulb 1 will always turn green when lever is activated" << std::endl;
    std::cout << "             Bulb 2 will turn green if you've won (all artworks at 0°)" << std::endl;
    std::cout << "             Bulb 2 will turn temporarily red if answer is incorrect" << std::endl;
    std::cout << std::endl;
    std::cout << "Win Condition:" << std::endl;
    std::cout << "  Rotate all artworks to be vertical (0 degrees rotation)" << std::endl;
    std::cout << "  Press SPACEBAR to check your answer once you think you're done" << std::endl;
    std::cout << std::endl;
    std::cout << "Stretching Controls (closest artwork):" << std::endl;
    std::cout << "  x/X - Increase/decrease image width" << std::endl;
    std::cout << "  y/Y - Increase/decrease image height" << std::endl;
    std::cout << "  f/F - Increase/decrease frame width" << std::endl;
    std::cout << "  g/G - Increase/decrease frame height" << std::endl;
    std::cout << "  r/R - Reset image/frame stretching" << std::endl;
    
    // Add information about the closest artwork feature
    std::cout << std::endl;
    std::cout << "Proximity Features:" << std::endl;
    std::cout << "  The console will display the closest artwork to you as you move." << std::endl;
    std::cout << "  Press 'p' to toggle detailed proximity debugging information." << std::endl;
    std::cout << "  You must be within 25 units of an artwork to interact with it." << std::endl;
}

// Cleanup
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
    
    // Note: InputSystem and ArtworkManager are singletons and will 
    // be cleaned up by their own destructors when the program ends
}

// Rotate closest artwork
void GameManager::rotateClosestArtwork(float angle) {
    if (closestArtworkID < 0 || closestArtworkDistance > 25.0f) {
        // No artwork nearby or too far away
        std::cout << "Too far from artwork to rotate. Current distance: " 
                  << closestArtworkDistance << " units" << std::endl;
        return;
    }
    
    // Find the artwork in the manager using our mapping
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
            
            std::cout << "Rotated " << getArtworkName(closestArtworkID) << " by " << angle 
                    << " degrees to " << artworkRotations[artworkIndex] << " degrees" << std::endl;
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

// Handle win state
void GameManager::handleWinState(float deltaTime) {
    if (!gameWon) return;
    
    // Initialize win animation on first frame
    if (winTimer == 0.0f) {
        winAnimationActive = true;
        winAnimationSpeed = 180.0f; // Degrees per second
        totalWinRotation = 0.0f;
        winFlashingLights = true;
        flashTimer = 0.0f;
        std::cout << "\n\nYOU WIN!\n\n" << std::endl;
        std::cout << "Enjoy the victory animation..." << std::endl;
    }
    
    winTimer += deltaTime;
    flashTimer += deltaTime;
    
    // Victory animation: rotate all artworks simultaneously
    if (winAnimationActive) {
        float rotationAmount = winAnimationSpeed * deltaTime;
        totalWinRotation += rotationAmount;
        
        // Rotate all artworks
        for (size_t i = 0; i < artworkManager->getArtworkCount(); i++) {
            Artwork* artwork = artworkManager->getArtwork(i);
            if (artwork) {
                // Update tracked rotation (doesn't matter for win state, but keeping consistent)
                artworkRotations[i] += rotationAmount;
                while (artworkRotations[i] >= 360.0f) artworkRotations[i] -= 360.0f;
                
                // Apply rotation animation
                artwork->rotate(artworkRotations[i], 0.0f, 0.0f, 1.0f);
            }
        }
        
        // After 3 complete rotations (1080 degrees), stop the rotation animation
        if (totalWinRotation >= 1080.0f) {
            winAnimationActive = false;
            
            // Reset all artworks to perfect alignment
            for (size_t i = 0; i < artworkManager->getArtworkCount(); i++) {
                Artwork* artwork = artworkManager->getArtwork(i);
                if (artwork) {
                    artworkRotations[i] = 0.0f;
                    artwork->rotate(0.0f, 0.0f, 0.0f, 1.0f);
                }
            }
        }
    }
    
    // Flashing light effects - modify ambient light every 0.3 seconds
    if (winFlashingLights && flashTimer >= 0.3f) {
        flashTimer = 0.0f;
        
        // Generate random light colors for victory effect
        GLfloat r = static_cast<GLfloat>(rand()) / RAND_MAX;
        GLfloat g = static_cast<GLfloat>(rand()) / RAND_MAX;
        GLfloat b = static_cast<GLfloat>(rand()) / RAND_MAX;
        
        GLfloat ambient[] = { r, g, b, 1.0f };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
        
        glutPostRedisplay();
    }
    
    // After 5 seconds, exit the game
    if (winTimer >= 5.0f) {
        // Reset lighting before exiting
        GLfloat defaultAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, defaultAmbient);
        
        std::cout << "Game completed successfully!" << std::endl;
        exit(0);
    }
} 
