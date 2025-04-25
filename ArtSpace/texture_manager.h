#pragma once
#include <string>
#include <unordered_map>
#include <GL/glut.h>

class TextureManager {
private:
    static TextureManager* instance;
    std::unordered_map<std::string, GLuint> textures;
    
    TextureManager();  // Private constructor for singleton

public:
    static TextureManager* getInstance();
    ~TextureManager();

    // Texture management
    bool loadTexture(const std::string& name, const std::string& filePath);
    void unloadTexture(const std::string& name);
    void clear();

    // Texture access
    GLuint getTexture(const std::string& name);
    bool isTextureLoaded(const std::string& name) const;

    // Batch operations
    bool loadTexturesFromDirectory(const std::string& directory);
    
    // TODO: Add texture compression options
    // TODO: Add mipmap generation
    // TODO: Add texture atlasing support
};
