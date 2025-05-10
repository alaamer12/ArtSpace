#pragma once
#include <GL/glut.h>
#include <cstdlib>  // For exit() function
#include "utility.h"
#include "navigator.h"
#include "screen_manager.h"
#include "screens.h"
#include "room.h"
#include "camera.h"

// Global variables
//Navigator* navigator = nullptr;
int windowWidth = 800;
int windowHeight = 600;

class StartScreenManager;
class Level1ScreenManager;
class EndScreenManager;

// StartScreenManager class
class StartScreenManager : public ScreenManager {
public:
    StartScreenManager() : ScreenManager(Screen::StartScreen) {}

    void setupScreen(Navigator* nav) {
        // Add a welcome message
        TextBox* welcomeText = new TextBox("Welcome to Artspace", "");
        welcomeText->setEditable(false);
        welcomeText->setPosition(windowWidth / 2 - 150, windowHeight / 2 + 50); // Centered-ish
        welcomeText->setSize(300, 50);
        welcomeText->setTextColor(0.9f, 0.9f, 0.9f); // Light text
        welcomeText->setBackgroundColor(0.1f, 0.1f, 0.1f, 0.0f); // Transparent background
        addComponent(welcomeText);

        // Add a Start button
        Button* startButton = new Button("Start");
        startButton->setPosition(windowWidth / 2 - 75, windowHeight / 2 - 25); // Below welcome
        startButton->setSize(150, 50);
        startButton->setOnClick([nav]() {
            if (nav) {
                nav->navigateTo(Screen::Level1Screen);
            }
        });
        addComponent(startButton);

        // Add an Exit button
        Button* exitButton = new Button("Exit");
        exitButton->setPosition(windowWidth / 2 - 75, windowHeight / 2 - 100); // Below start
        exitButton->setSize(150, 50);
        exitButton->setBackgroundColor(0.7f, 0.3f, 0.3f); // Reddish color
        exitButton->setOnClick([]() {
            // Add a log message before exiting
            Logger::getInstance().logInfo("Exit button clicked. Closing application.");
            exit(0); // Standard C exit function - works everywhere
        });
        addComponent(exitButton);
    }

    void resizeScreen(int width, int height) override {
        // Update window dimensions
        windowWidth = width;
        windowHeight = height;

        // Recalculate positions of components if they exist
        if (getComponents().size() >= 3) { // Ensure all components are added
            // Welcome Text
            getComponents()[0]->setPosition(width / 2 - 150, height / 2 + 50);
            getComponents()[0]->setSize(300, 50);

            // Start Button
            getComponents()[1]->setPosition(width / 2 - 75, height / 2 - 25);
            getComponents()[1]->setSize(150, 50);

            // Exit Button
            getComponents()[2]->setPosition(width / 2 - 75, height / 2 - 100);
            getComponents()[2]->setSize(150, 50);
        }
    }
};

// Level1ScreenManager class
class Level1ScreenManager : public ScreenManager {
private:
    Timer* gameTimer;
    Room* room;
    HumanCamera* camera;
    bool initialized;

public:
  Level1ScreenManager(Timer* gameTimer, Room* room, HumanCamera* camera)
        : ScreenManager(Screen::Level1Screen),
          gameTimer(gameTimer), room(room), camera(camera),
          initialized(false) {}

    ~Level1ScreenManager() {
        // Do NOT delete them
    }

    void setupScreen(Navigator* nav) {
        // Create a timer for the game
        
        gameTimer->setPosition(10, windowHeight - 40);
        gameTimer->setSize(150, 30);
        gameTimer->setTextColor(0.8f, 0.8f, 0.8f, 1.0f);
        addComponent(gameTimer);

        // Initialize room and camera (will be done in showScreen)
        initialized = false;
    }

    void showScreen() override {
        ScreenManager::showScreen();
        
        if (!initialized) {
            // Only create room and camera once
            // Create the room
            room = new Room(10.0f, 5.0f, 10.0f);

            // Use a relative path for textures
            std::string texturePath = "assets/textures/floor.bmp";
            room->setWallTexture(texturePath);
            room->setFloorTexture(texturePath);
            room->setRoofTexture(texturePath);

            // Create the camera
            camera = new HumanCamera();
            camera->setPosition(0.0f, 0.0f, 3.0f);

            // Center the mouse cursor
            glutWarpPointer(windowWidth / 2, windowHeight / 2);
            
            // Start the timer
            if (gameTimer) {
                gameTimer->reset();
                gameTimer->start();
            }
            
            initialized = true;
        }
    }

    void hideScreen() override {
        ScreenManager::hideScreen();
        
        // Stop the timer when hiding the screen
        if (gameTimer) {
            gameTimer->stop();
        }
    }

    void renderScreen() {
        // First render the room in 3D space
        if (room && camera) {
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            camera->applyTransformation();
            room->render();
        }

        // Now render UI components (timer, etc.)
        ScreenManager::renderScreen();
    }

    void updateScreen(float deltaTime) {
        // First update UI components
        ScreenManager::updateScreen(deltaTime);

        // Then update game logic
        if (camera) {
            camera->update(deltaTime);
            
            // Apply collision detection with room walls
            if (room) {
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
            }
        }
    }

    void resizeScreen(int width, int height) override {
        // Update the timer position
        if (gameTimer) {
            gameTimer->setPosition(10, height - 40);
        }
    }
};

// EndScreenManager class
class EndScreenManager : public ScreenManager {
public:
    EndScreenManager() : ScreenManager(Screen::EndScreen) {}

    void setupScreen(Navigator* nav) {
        // Create a title
        TextBox* title = new TextBox("GAME OVER", "");
        title->setEditable(false);
        title->setPosition(windowWidth / 2 - 100, windowHeight - 100);
        title->setSize(200, 40);
        title->setTextColor(1.0f, 0.3f, 0.3f);
        title->setBackgroundColor(0.2f, 0.2f, 0.2f);
        addComponent(title);

        // Create a score display
        TextBox* scoreBox = new TextBox("Final Score: 150", "");
        scoreBox->setEditable(false);
        scoreBox->setPosition(windowWidth / 2 - 100, windowHeight / 2 + 20);
        scoreBox->setSize(200, 40);
        scoreBox->setTextColor(1.0f, 1.0f, 1.0f);
        scoreBox->setBackgroundColor(0.2f, 0.2f, 0.2f);
        addComponent(scoreBox);

        // Create a play again button
        Button* playAgainButton = new Button("Play Again");
        playAgainButton->setPosition(windowWidth / 2 - 75, windowHeight / 2 - 50);
        playAgainButton->setSize(150, 50);
        playAgainButton->setOnClick([nav]() {
            nav->navigateTo(Screen::Level1Screen);
            });
        addComponent(playAgainButton);

        // Create a main menu button
        Button* menuButton = new Button("Main Menu");
        menuButton->setPosition(windowWidth / 2 - 75, windowHeight / 2 - 120);
        menuButton->setSize(150, 50);
        menuButton->setBackgroundColor(0.5f, 0.5f, 0.7f);
        menuButton->setOnClick([nav]() {
            nav->navigateTo(Screen::StartScreen);
            });
        addComponent(menuButton);
    }

    void resizeScreen(int width, int height) override {
        if (!getComponents().empty()) {
            // Update title position
            getComponents()[0]->setPosition(width / 2 - 100, height - 100);

            // Update score box position
            getComponents()[1]->setPosition(width / 2 - 100, height / 2 + 20);

            // Update play again button position
            getComponents()[2]->setPosition(width / 2 - 75, height / 2 - 50);

            // Update menu button position
            getComponents()[3]->setPosition(width / 2 - 75, height / 2 - 120);
        }
    }
};