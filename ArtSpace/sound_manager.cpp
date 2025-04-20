#include "sound_manager.h"
#include "config.h"

SoundManager* SoundManager::instance = nullptr;

SoundManager::SoundManager() 
    : masterVolume(1.0f)
    , musicVolume(0.7f)
    , effectsVolume(1.0f) {
}

SoundManager* SoundManager::getInstance() {
    if (instance == nullptr) {
        instance = new SoundManager();
    }
    return instance;
}

SoundManager::~SoundManager() {
    sounds.clear();
    soundBuffers.clear();
}

bool SoundManager::loadSound(const std::string& name, const std::string& filePath) {
    if (soundBuffers.find(name) != soundBuffers.end()) {
        return true;  // Already loaded
    }

    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filePath)) {
        return false;
    }

    soundBuffers[name] = buffer;
    sounds[name].setBuffer(soundBuffers[name]);
    sounds[name].setVolume(effectsVolume * masterVolume * 100);
    return true;
}

bool SoundManager::loadMusic(const std::string& filePath) {
    if (!backgroundMusic.openFromFile(filePath)) {
        return false;
    }
    backgroundMusic.setVolume(musicVolume * masterVolume * 100);
    backgroundMusic.setLoop(true);
    return true;
}

void SoundManager::unloadSound(const std::string& name) {
    auto soundIt = sounds.find(name);
    if (soundIt != sounds.end()) {
        sounds.erase(soundIt);
    }
    auto bufferIt = soundBuffers.find(name);
    if (bufferIt != soundBuffers.end()) {
        soundBuffers.erase(bufferIt);
    }
}

void SoundManager::playSound(const std::string& name) {
    auto it = sounds.find(name);
    if (it != sounds.end()) {
        it->second.play();
    }
}

void SoundManager::playMusic() {
    backgroundMusic.play();
}

void SoundManager::stopMusic() {
    backgroundMusic.stop();
}

void SoundManager::pauseMusic() {
    backgroundMusic.pause();
}

void SoundManager::resumeMusic() {
    backgroundMusic.play();
}

void SoundManager::setMasterVolume(float volume) {
    masterVolume = volume;
    // Update all sound volumes
    for (auto& sound : sounds) {
        sound.second.setVolume(effectsVolume * masterVolume * 100);
    }
    backgroundMusic.setVolume(musicVolume * masterVolume * 100);
}

void SoundManager::setMusicVolume(float volume) {
    musicVolume = volume;
    backgroundMusic.setVolume(musicVolume * masterVolume * 100);
}

void SoundManager::setEffectsVolume(float volume) {
    effectsVolume = volume;
    for (auto& sound : sounds) {
        sound.second.setVolume(effectsVolume * masterVolume * 100);
    }
}
