#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <unordered_map>

class SoundManager {
private:
    static SoundManager* instance;
    std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;
    std::unordered_map<std::string, sf::Sound> sounds;
    sf::Music backgroundMusic;
    float Volume;

    SoundManager();  // Private constructor for singleton

public:
    static SoundManager* getInstance();
    ~SoundManager();

    // Sound loading and management
    bool loadSound(const std::string& name, const std::string& filePath);
    void unloadSound(const std::string& name);

    // Playback control
    void playSound(const std::string& name);

    // Volume control
    void setVolume(float volume);
};
