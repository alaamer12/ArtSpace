#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../Dependencies/stb_image.h"

// Image data
unsigned char* imageData = NULL;
unsigned char* frameData = NULL;
int imageWidth, imageHeight, imageChannels;
int frameWidth, frameHeight, frameChannels;
GLuint imageTextureID, frameTextureID;

// Transformation variables
float scale = 1.0f;
float rotation = 0.0f;
float xPosition = 0.0f;
float yPosition = 0.0f;

// Function to load images using stb_image
unsigned char* loadImage(const char* filename, int* width, int* height, int* channels) {
    // Force loading with 4 channels (RGBA)
    unsigned char* data = stbi_load(filename, width, height, channels, 4);
    if (!data) {
        printf("Error loading image %s\n", filename);
        exit(1);
    }
    return data;
}

// Initialize OpenGL settings
void init() {
    glClearColor(0.2, 0.2, 0.2, 1.0);  // Dark gray background
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    imageData = loadImage("C:\\Users\\amrmu\\Downloads\\Pictures\\StarScream.jpg", &imageWidth, &imageHeight, &imageChannels);
    printf("Image loaded: %dx%d with %d channels\n", imageWidth, imageHeight, imageChannels);

    // Load frame image
    frameData = loadImage("E:\\Projects\\Games\\ArtSpace - Copy\\ArtSpace\\assets\\textures\\frames\\Precious.png", &frameWidth, &frameHeight, &frameChannels);
    printf("Frame loaded: %dx%d with %d channels\n", frameWidth, frameHeight, frameChannels);

    // Generate and bind image texture
    glGenTextures(1, &imageTextureID);
    glBindTexture(GL_TEXTURE_2D, imageTextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight,
        0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

    // Generate and bind frame texture
    glGenTextures(1, &frameTextureID);
    glBindTexture(GL_TEXTURE_2D, frameTextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frameWidth, frameHeight,
        0, GL_RGBA, GL_UNSIGNED_BYTE, frameData);
}

// Render the unified artwork (frame + image)
void renderArtwork(int xPos, int yPos, float scale, float rotation) {
    // Calculate inner frame dimensions (where image should be displayed)
    int borderSize = 40;  // pixels
    int innerWidth = frameWidth - (borderSize * 2);
    int innerHeight = frameHeight - (borderSize * 2);
    int innerX = borderSize;
    int innerY = borderSize;

    // Save current transformation matrix
    glPushMatrix();

    // Apply transformations (position, scale, rotation)
    glTranslatef(xPos, yPos, 0.0f);
    glTranslatef(frameWidth * scale / 2, frameHeight * scale / 2, 0.0f);  // Move to center
    glRotatef(rotation, 0.0f, 0.0f, 1.0f);  // Rotate around Z-axis
    glScalef(scale, scale, 1.0f);  // Scale
    glTranslatef(-frameWidth / 2, -frameHeight / 2, 0.0f);  // Move back

    // First render the StarScream image scaled to fit inside the frame
    glBindTexture(GL_TEXTURE_2D, imageTextureID);
    glBegin(GL_QUADS);
    // Fix texture coordinates to flip image right-side up (swapping top and bottom)
    glTexCoord2f(0.0f, 1.0f); glVertex2i(innerX, innerY);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(innerX + innerWidth, innerY);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(innerX + innerWidth, innerY + innerHeight);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(innerX, innerY + innerHeight);
    glEnd();

    // Then render the frame with transparency enabled
    glBindTexture(GL_TEXTURE_2D, frameTextureID);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(0, 0);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(frameWidth, 0);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(frameWidth, frameHeight);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(0, frameHeight);
    glEnd();

    // Restore transformation matrix
    glPopMatrix();
}

// Display callback function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Set up orthographic projection to match window size
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT), -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Calculate position to center the artwork
    int centerX = glutGet(GLUT_WINDOW_WIDTH) / 2;
    int centerY = glutGet(GLUT_WINDOW_HEIGHT) / 2;

    // Render the unified artwork
    renderArtwork(centerX + xPosition, centerY + yPosition, scale, rotation);

    glutSwapBuffers();
}

// Resize callback function
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

// Cleanup function
void cleanup() {
    if (imageData) stbi_image_free(imageData);
    if (frameData) stbi_image_free(frameData);
    if (imageTextureID) glDeleteTextures(1, &imageTextureID);
    if (frameTextureID) glDeleteTextures(1, &frameTextureID);
}

// Key handling callback
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 27:  // ESC key
        exit(0);
        break;
    case 'r':  // Rotate clockwise
        rotation += 5.0f;
        glutPostRedisplay();
        break;
    case 'R':  // Rotate counter-clockwise
        rotation -= 5.0f;
        glutPostRedisplay();
        break;
    case '+':  // Scale up
    case '=':
        scale += 0.1f;
        glutPostRedisplay();
        break;
    case '-':  // Scale down
        scale -= 0.1f;
        if (scale < 0.1f) scale = 0.1f;
        glutPostRedisplay();
        break;
    case 'w':  // Move up
        yPosition += 10.0f;
        glutPostRedisplay();
        break;
    case 's':  // Move down
        yPosition -= 10.0f;
        glutPostRedisplay();
        break;
    case 'a':  // Move left
        xPosition -= 10.0f;
        glutPostRedisplay();
        break;
    case 'd':  // Move right
        xPosition += 10.0f;
        glutPostRedisplay();
        break;
    case ' ':  // Reset transformations
        scale = 1.0f;
        rotation = 0.0f;
        xPosition = 0.0f;
        yPosition = 0.0f;
        glutPostRedisplay();
        break;
    }
}

// Special key handling callback
void specialKeys(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        yPosition += 10.0f;
        glutPostRedisplay();
        break;
    case GLUT_KEY_DOWN:
        yPosition -= 10.0f;
        glutPostRedisplay();
        break;
    case GLUT_KEY_LEFT:
        xPosition -= 10.0f;
        glutPostRedisplay();
        break;
    case GLUT_KEY_RIGHT:
        xPosition += 10.0f;
        glutPostRedisplay();
        break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Transformable Framed StarScream");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    // Register cleanup function with atexit
    atexit(cleanup);

    // Print usage instructions
    printf("Controls:\n");
    printf("  Arrow keys / WASD: Move artwork\n");
    printf("  r/R: Rotate clockwise/counter-clockwise\n");
    printf("  +/-: Scale up/down\n");
    printf("  Space: Reset transformations\n");
    printf("  ESC: Exit\n");

    glutMainLoop();
    return 0;
}
