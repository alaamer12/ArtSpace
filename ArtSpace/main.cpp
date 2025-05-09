#include <cstdlib>
#include <GL/glut.h>
#include <iostream>
#include "game_manager.h"
#include "config.h"

// Function prototypes for OpenGL callbacks
void display();
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void specialKeyboard(int key, int x, int y);
void specialKeyboardUp(int key, int x, int y);
void mouseMotion(int x, int y);
void mouseButton(int button, int state, int x, int y);
void idle();
void cleanup();

// Display callback function - called when the window needs to be redrawn
void display() {
    // Use the GameManager to render the game
    GameManager::getInstance()->render();
    glutSwapBuffers();
}

// Reshape callback function - called when the window is resized
void reshape(int width, int height) {
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (float)width / (float)height, 0.1f, 100.0f);

    Config::getInstance().setScreenWidth(width);
    Config::getInstance().setScreenHeight(height);
}

// Idle callback function - called when the program is not busy
void idle() {
    // Get delta time and update game state
    float deltaTime = GameManager::getInstance()->getDeltaTime();
    GameManager::getInstance()->update(deltaTime);
    glutPostRedisplay();
}

// Keyboard callback function - called when a key is pressed
void keyboard(unsigned char key, int x, int y) {
    if (key == 27) { // ESC key
        cleanup();
        exit(0);
    }
    
    // Forward to game manager
    GameManager::getInstance()->handleKeyPress(key, x, y);
}

// Keyboard up callback function - called when a key is released
void keyboardUp(unsigned char key, int x, int y) {
    GameManager::getInstance()->handleKeyRelease(key, x, y);
}

// Special keyboard callback function - called when a special key is pressed
void specialKeyboard(int key, int x, int y) {
    // Not currently used
}

// Special keyboard up callback function - called when a special key is released
void specialKeyboardUp(int key, int x, int y) {
    // Not currently used
}

// Mouse motion callback function - called when the mouse is moved
void mouseMotion(int x, int y) {
    GameManager::getInstance()->handleMouseMotion(x, y);
}

// Mouse button callback function - called when a mouse button is pressed/released
void mouseButton(int button, int state, int x, int y) {
    GameManager::getInstance()->handleMouseButton(button, state, x, y);
}

// Cleanup function - called before the program exits
void cleanup() {
    // Clean up game manager
    delete GameManager::getInstance();
}

// Main function
int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

    // Get screen configuration
    Config& config = Config::getInstance();
    int width = config.getScreenWidth();
    int height = config.getScreenHeight();

    // Create window
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("ArtSpace - Room & Camera Demo");

    // Register GLUT callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutMotionFunc(mouseMotion);
    glutPassiveMotionFunc(mouseMotion);
    glutMouseFunc(mouseButton);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Initialize game manager
    GameManager::getInstance()->init();

    // Center the mouse cursor
    glutWarpPointer(width / 2, height / 2);

    // Start the main loop
    glutMainLoop();
    
    // Clean up resources
    cleanup();
    
    return 0;
}