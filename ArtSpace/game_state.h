#pragma once
#include <string>

enum class GameState {
    MAIN_MENU,
    PLAYING,
    PAUSED,
    LEVEL_COMPLETE,
    GAME_OVER
};

class GameStateManager {
private:
    static GameStateManager* instance;
    GameState currentState;
    int currentLevel;
    float gameTime;
    bool isPaused;

    GameStateManager();  // Private constructor for singleton

public:
    static GameStateManager* getInstance();
    ~GameStateManager();

    // State management
    GameState getState() const;
    void setState(GameState state);
    void togglePause();

    // Level management
    int getCurrentLevel() const;
    void setCurrentLevel(int level);
    bool loadLevel(int level);
    
    // Game loop
    void update(float deltaTime);
    void reset();
    
    // TODO: Add save/load game state
    // TODO: Add statistics tracking
    // TODO: Add achievement system
};
