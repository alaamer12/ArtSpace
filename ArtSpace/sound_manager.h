#pragma once

#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <map>
#include <memory>

// SoundManager: Handles loading and playing sound effects using SFML
class SoundManager {
private:
    // Singleton instance
    static SoundManager* instance;
    
    // Sound directory
    std::string soundDir;
    
    // Maps for storing sounds and music
    std::map<std::string, std::unique_ptr<sf::SoundBuffer>> soundBuffers;
    std::map<std::string, std::unique_ptr<sf::Sound>> sounds;
    std::unique_ptr<sf::Music> backgroundMusic;
    
    // Private constructor (singleton pattern)
    SoundManager() : soundDir("assets\\sounds\\") {
        // Initialize SFML audio system
        std::cout << "Initializing Sound Manager..." << std::endl;
    }
    
public:
    // Destructor
    ~SoundManager() {
        // Clean up resources
        sounds.clear();
        soundBuffers.clear();
        backgroundMusic.reset();
    }
    
    // Delete copy constructor and assignment operator
    SoundManager(const SoundManager&) = delete;
    SoundManager& operator=(const SoundManager&) = delete;
    
    // Get singleton instance
    static SoundManager* getInstance() {
        if (instance == nullptr) {
            instance = new SoundManager();
        }
        return instance;
    }
    
    // Initialize sound system
    bool init() {
        // Load essential sound effects
        bool success = true;
        
        success &= loadSound("start-game", "start-game.wav");
        success &= loadSound("wrong", "wrong.wav");
        success &= loadSound("game-over", "game-over.wav");
        
        return success;
    }
    
    // Load a sound effect from file
    bool loadSound(const std::string& name, const std::string& filename) {
        std::string fullPath = soundDir + filename;
        
        // Create a new sound buffer
        std::unique_ptr<sf::SoundBuffer> buffer = std::make_unique<sf::SoundBuffer>();
        
        // Load the sound file
        if (!buffer->loadFromFile(fullPath)) {
            std::cerr << "Failed to load sound: " << fullPath << std::endl;
            return false;
        }
        
        // Create a sound object linked to the buffer
        std::unique_ptr<sf::Sound> sound = std::make_unique<sf::Sound>(*buffer);
        
        // Store the buffer and sound
        soundBuffers[name] = std::move(buffer);
        sounds[name] = std::move(sound);
        
        std::cout << "Loaded sound: " << name << " from " << fullPath << std::endl;
        return true;
    }
    
    // Play a sound effect
    void playSound(const std::string& name) {
        auto it = sounds.find(name);
        if (it != sounds.end()) {
            it->second->play();
        } else {
            std::cerr << "Sound not found: " << name << std::endl;
        }
    }
    
    // Stop a sound effect
    void stopSound(const std::string& name) {
        auto it = sounds.find(name);
        if (it != sounds.end()) {
            it->second->stop();
        }
    }
    
    // Load and play background music
    bool playBackgroundMusic(const std::string& filename, bool loop = true) {
        // Create a new music instance if needed
        if (!backgroundMusic) {
            backgroundMusic = std::make_unique<sf::Music>();
        }
        
        // Load and play the music
        std::string fullPath = soundDir + filename;
        if (!backgroundMusic->openFromFile(fullPath)) {
            std::cerr << "Failed to load background music: " << fullPath << std::endl;
            return false;
        }
        
        // Set loop status using the correct method
        backgroundMusic->setLooping(loop);  // Correct method name
        backgroundMusic->play();
        return true;
    }
    
    // Stop the background music
    void stopBackgroundMusic() {
        if (backgroundMusic) {
            backgroundMusic->stop();
        }
    }
    
    // Set sound volume (0-100)
    void setSoundVolume(float volume) {
        for (auto& sound : sounds) {
            sound.second->setVolume(volume);
        }
    }
    
    // Set music volume (0-100)
    void setMusicVolume(float volume) {
        if (backgroundMusic) {
            backgroundMusic->setVolume(volume);
        }
    }
    
    // Play game start sound
    void playGameStartSound() {
        playSound("start-game");
    }
    
    // Play wrong answer sound
    void playWrongSound() {
        playSound("wrong");
    }
    
    // Play game over (win) sound
    void playGameOverSound() {
        playSound("game-over");
    }
};

// Initialize static instance to nullptr
SoundManager* SoundManager::instance = nullptr;