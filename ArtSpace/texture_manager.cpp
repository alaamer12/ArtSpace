#include "texture_manager.h"
#include "config.h"
#include <filesystem>

TextureManager* TextureManager::instance = nullptr;

TextureManager::TextureManager() {
}

TextureManager* TextureManager::getInstance() {
    if (instance == nullptr) {
        instance = new TextureManager();
    }
    return instance;
}

TextureManager::~TextureManager() {
    clear();
}

bool TextureManager::loadTexture(const std::string& name, const std::string& filePath) {
    if (textures.find(name) != textures.end()) {
        return true;  // Already loaded
    }

    sf::Texture texture;
    if (!texture.loadFromFile(filePath)) {
        return false;
    }

    // Enable smooth textures by default
    texture.setSmooth(true);
    textures[name] = texture;
    return true;
}

void TextureManager::unloadTexture(const std::string& name) {
    auto it = textures.find(name);
    if (it != textures.end()) {
        textures.erase(it);
    }
}

void TextureManager::clear() {
    textures.clear();
}

sf::Texture* TextureManager::getTexture(const std::string& name) {
    auto it = textures.find(name);
    if (it != textures.end()) {
        return &it->second;
    }
    return nullptr;
}

bool TextureManager::isTextureLoaded(const std::string& name) const {
    return textures.find(name) != textures.end();
}

bool TextureManager::loadTexturesFromDirectory(const std::string& directory) {
    namespace fs = std::filesystem;
    bool success = true;

    try {
        for (const auto& entry : fs::directory_iterator(directory)) {
            if (entry.is_regular_file()) {
                const auto& path = entry.path();
                if (path.extension() == ".png" || path.extension() == ".jpg") {
                    std::string name = path.stem().string();
                    if (!loadTexture(name, path.string())) {
                        success = false;
                    }
                }
            }
        }
    }
    catch (const fs::filesystem_error&) {
        return false;
    }

    return success;
}
