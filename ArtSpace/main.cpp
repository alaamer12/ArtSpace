#define STB_IMAGE_IMPLEMENTATION
#include "../Dependencies/stb_image.h"
#include <GL/glut.h>
#include <iostream>

// Texture variables
GLuint textureID;
int width, height, channels;

void loadTexture(const char* filePath) {
    // Load image using stb_image
    unsigned char* image = stbi_load(filePath, &width, &height, &channels, STBI_rgb_alpha);
    
    if (!image) {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
        return;
    }

    // Generate and bind OpenGL texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    // Free image data
    stbi_image_free(image);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Enable texture mapping
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Draw textured quad
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-0.5f, -0.5f);  // Bottom Left
    glTexCoord2f(1.0f, 0.0f); glVertex2f(0.5f, -0.5f);   // Bottom Right
    glTexCoord2f(1.0f, 1.0f); glVertex2f(0.5f, 0.5f);    // Top Right
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-0.5f, 0.5f);   // Top Left
    glEnd();

    // Disable texture mapping
    glDisable(GL_TEXTURE_2D);

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        gluOrtho2D(-1.0, 1.0, -1.0 * (GLfloat)h / (GLfloat)w, 1.0 * (GLfloat)h / (GLfloat)w);
    else
        gluOrtho2D(-1.0 * (GLfloat)w / (GLfloat)h, 1.0 * (GLfloat)w / (GLfloat)h, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Texture Tutorial");

    // Set clear color to dark gray
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    // Load texture
    // Replace "your_image.jpg" with your actual image path
    loadTexture("D:\\Private\\Media\\Ai Generated Images\\ss.png");

    // Register callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    // Start main loop
    glutMainLoop();
    return 0;
}



