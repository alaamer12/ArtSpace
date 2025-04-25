#pragma once
#include <GL/glut.h>
#include "utility.h"
#include "navigator.h"
#include "screen_manager.h"
#include "screens.h"

// Global variables
Navigator* navigator = nullptr;
int windowWidth = 800;
int windowHeight = 600;
float lastTime = 0;

class StartScreenManager;
class Level1ScreenManager;
class GameStateScreenManager;
class PauseScreenManager;
class EndScreenManager;

// StartScreenManager class
class StartScreenManager : public ScreenManager {
public:
    StartScreenManager() : ScreenManager(Screen::StartScreen) {}

    void setupScreen(Navigator* nav) {
        // Create a title
        TextBox* title = new TextBox("ART SPACE DEMO", "");
        title->setEditable(false);
        title->setPosition(windowWidth / 2 - 100, windowHeight - 100);
        title->setSize(200, 40);
        title->setTextColor(1.0f, 1.0f, 1.0f);
        title->setBackgroundColor(0.2f, 0.2f, 0.4f);
        addComponent(title);

        // Create a start button
        Button* startButton = new Button("Start Game");
        startButton->setPosition(windowWidth / 2 - 75, windowHeight / 2);
        startButton->setSize(150, 50);
        startButton->setOnClick([nav]() {
            nav->navigateTo(Screen::Level1Screen);
            });
        addComponent(startButton);

        // Create an exit button
        Button* exitButton = new Button("Exit");
        exitButton->setPosition(windowWidth / 2 - 75, windowHeight / 2 - 70);
        exitButton->setSize(150, 50);
        exitButton->setBackgroundColor(0.7f, 0.3f, 0.3f);
        exitButton->setOnClick([]() {
            exit(0);
            });
        addComponent(exitButton);
    }

    void resizeScreen(int width, int height) override {
        // Update positions when window is resized
        if (!getComponents().empty()) {
            // Update title position
            getComponents()[0]->setPosition(width / 2 - 100, height - 100);

            // Update start button position
            getComponents()[1]->setPosition(width / 2 - 75, height / 2);

            // Update exit button position
            getComponents()[2]->setPosition(width / 2 - 75, height / 2 - 70);
        }
    }
};

// Level1ScreenManager class
class Level1ScreenManager : public ScreenManager {
private:
    Timer* gameTimer;

public:
    Level1ScreenManager() : ScreenManager(Screen::Level1Screen), gameTimer(nullptr) {}

    void setupScreen(Navigator* nav) {
        // Create a timer
        gameTimer = new Timer();
        gameTimer->setPosition(windowWidth / 2 - 50, windowHeight - 50);
        gameTimer->setSize(100, 30);
        gameTimer->start(); // Auto-start the timer
        addComponent(gameTimer);

        // Create a pause button
        Button* pauseButton = new Button("Pause");
        pauseButton->setPosition(20, windowHeight - 50);
        pauseButton->setSize(80, 30);
        pauseButton->setOnClick([nav]() {
            nav->navigateTo(Screen::PauseScreen);
            });
        addComponent(pauseButton);

        // Create a status button
        Button* statusButton = new Button("Status");
        statusButton->setPosition(windowWidth - 100, windowHeight - 50);
        statusButton->setSize(80, 30);
        statusButton->setOnClick([nav]() {
            nav->navigateTo(Screen::GameStateScreen);
            });
        addComponent(statusButton);

        // Create a game over button (simulating game end)
        Button* gameOverButton = new Button("Game Over");
        gameOverButton->setPosition(windowWidth / 2 - 75, 50);
        gameOverButton->setSize(150, 40);
        gameOverButton->setBackgroundColor(0.7f, 0.3f, 0.3f);
        gameOverButton->setOnClick([nav]() {
            nav->navigateTo(Screen::EndScreen);
            });
        addComponent(gameOverButton);
    }

    void showScreen() override {
        ScreenManager::showScreen();
        if (gameTimer) {
            gameTimer->start(); // Resume timer when screen becomes active
        }
    }

    void hideScreen() override {
        ScreenManager::hideScreen();
        if (gameTimer) {
            gameTimer->stop(); // Pause timer when screen is hidden
        }
    }

    void resizeScreen(int width, int height) override {
        if (!getComponents().empty()) {
            // Update timer position
            getComponents()[0]->setPosition(width / 2 - 50, height - 50);

            // Update pause button position
            getComponents()[1]->setPosition(20, height - 50);

            // Update status button position
            getComponents()[2]->setPosition(width - 100, height - 50);

            // Update game over button position
            getComponents()[3]->setPosition(width / 2 - 75, 50);
        }
    }
};

// GameStateScreenManager class
class GameStateScreenManager : public ScreenManager {
public:
    GameStateScreenManager() : ScreenManager(Screen::GameStateScreen) {}

    void setupScreen(Navigator* nav) {
        // Create a title
        TextBox* title = new TextBox("Game Status", "");
        title->setEditable(false);
        title->setPosition(windowWidth / 2 - 100, windowHeight - 100);
        title->setSize(200, 40);
        title->setTextColor(1.0f, 1.0f, 1.0f);
        title->setBackgroundColor(0.2f, 0.4f, 0.2f);
        addComponent(title);

        // Create some status information
        TextBox* infoBox = new TextBox("Level: 1\nScore: 0\nLives: 3", "");
        infoBox->setEditable(false);
        infoBox->setPosition(windowWidth / 2 - 100, windowHeight / 2 - 50);
        infoBox->setSize(200, 100);
        infoBox->setBackgroundColor(0.2f, 0.2f, 0.2f);
        infoBox->setTextColor(1.0f, 1.0f, 1.0f);
        addComponent(infoBox);

        // Create a back button
        Button* backButton = new Button("Back to Game");
        backButton->setPosition(windowWidth / 2 - 75, windowHeight / 2 - 150);
        backButton->setSize(150, 50);
        backButton->setOnClick([nav]() {
            nav->navigateTo(Screen::Level1Screen);
            });
        addComponent(backButton);
    }

    void resizeScreen(int width, int height) override {
        if (!getComponents().empty()) {
            // Update title position
            getComponents()[0]->setPosition(width / 2 - 100, height - 100);

            // Update info box position
            getComponents()[1]->setPosition(width / 2 - 100, height / 2 - 50);

            // Update back button position
            getComponents()[2]->setPosition(width / 2 - 75, height / 2 - 150);
        }
    }
};

// PauseScreenManager class
class PauseScreenManager : public ScreenManager {
public:
    PauseScreenManager() : ScreenManager(Screen::PauseScreen) {}

    void setupScreen(Navigator* nav) {
        // Create a title with BorderBox
        TextBox* pauseTitle = new TextBox("PAUSED", "");
        pauseTitle->setEditable(false);
        pauseTitle->setSize(180, 40);
        pauseTitle->setTextColor(1.0f, 1.0f, 1.0f);
        pauseTitle->setBackgroundColor(0.3f, 0.3f, 0.3f);

        BorderBox* titleBorder = new BorderBox(pauseTitle, 3.0f, 10.0f);
        titleBorder->setPosition(windowWidth / 2 - 100, windowHeight - 120);
        titleBorder->setSize(200, 60);
        titleBorder->setBorderColor(1.0f, 0.8f, 0.0f);
        addComponent(titleBorder);

        // Create a resume button
        Button* resumeButton = new Button("Resume Game");
        resumeButton->setPosition(windowWidth / 2 - 75, windowHeight / 2);
        resumeButton->setSize(150, 50);
        resumeButton->setOnClick([nav]() {
            nav->navigateTo(Screen::Level1Screen);
            });
        addComponent(resumeButton);

        // Create a main menu button
        Button* menuButton = new Button("Main Menu");
        menuButton->setPosition(windowWidth / 2 - 75, windowHeight / 2 - 70);
        menuButton->setSize(150, 50);
        menuButton->setBackgroundColor(0.5f, 0.5f, 0.7f);
        menuButton->setOnClick([nav]() {
            nav->navigateTo(Screen::StartScreen);
            });
        addComponent(menuButton);
    }

    void resizeScreen(int width, int height) override {
        if (!getComponents().empty()) {
            // Update title border position
            getComponents()[0]->setPosition(width / 2 - 100, height - 120);

            // Update resume button position
            getComponents()[1]->setPosition(width / 2 - 75, height / 2);

            // Update menu button position
            getComponents()[2]->setPosition(width / 2 - 75, height / 2 - 70);
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