#include "game_state.h"

GameStateManager* GameStateManager::instance = nullptr;

GameStateManager::GameStateManager() 
    : currentState(GameState::MAIN_MENU)
    , currentLevel(1)
    , gameTime(0.0f)
    , isPaused(false) {
}

GameStateManager* GameStateManager::getInstance() {
    if (instance == nullptr) {
        instance = new GameStateManager();
    }
    return instance;
}

GameStateManager::~GameStateManager() {
}

GameState GameStateManager::getState() const {
    return currentState;
}

void GameStateManager::setState(GameState state) {
    currentState = state;
    // TODO: Handle state transition effects
    // TODO: Notify observers of state change
}

void GameStateManager::togglePause() {
    isPaused = !isPaused;
    currentState = isPaused ? GameState::PAUSED : GameState::PLAYING;
}

int GameStateManager::getCurrentLevel() const {
    return currentLevel;
}

void GameStateManager::setCurrentLevel(int level) {
    if (level > 0) {
        currentLevel = level;
    }
}

bool GameStateManager::loadLevel(int level) {
    // TODO: Implement level loading logic
    // TODO: Reset level-specific states
    // TODO: Load level configuration
    return false;
}

void GameStateManager::update(float deltaTime) {
    if (!isPaused) {
        gameTime += deltaTime;
    }
    // TODO: Update game systems based on current state
}

void GameStateManager::reset() {
    currentLevel = 1;
    gameTime = 0.0f;
    isPaused = false;
    currentState = GameState::MAIN_MENU;
}
