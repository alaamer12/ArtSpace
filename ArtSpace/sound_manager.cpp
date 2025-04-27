#include "sound_manager.h"
#include "config.h"

SoundManager* SoundManager::instance = nullptr;

SoundManager::SoundManager() 
    : Volume(1.0f) {
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
    // TODO
<<<<<<< Updated upstream
=======
    return 1;
>>>>>>> Stashed changes
}

void SoundManager::unloadSound(const std::string& name) {
    // TODO
}

void SoundManager::playSound(const std::string& name) {
    // TODO
}

void SoundManager::setVolume(float volume) {
    // TODO
}