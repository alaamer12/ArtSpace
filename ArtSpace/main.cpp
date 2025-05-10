#include <cstdlib>
#include <GL/glut.h>
#include <iostream>
#include "game_manager.h"
#include "config.h"

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


void display() {

    GameManager::getInstance()->render();
    glutSwapBuffers();
}


void reshape(int width, int height) {
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (float)width / (float)height, 0.1f, 100.0f);

    Config::getInstance().setScreenWidth(width);
    Config::getInstance().setScreenHeight(height);
}


void idle() {

    float deltaTime = GameManager::getInstance()->getDeltaTime();
    GameManager::getInstance()->update(deltaTime);
    glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y) {
    if (key == 27) { // ESC key
        cleanup();
        cleanup();
        exit(0);
    }
    
    // Forward to game manager
    GameManager::getInstance()->handleKeyPress(key, x, y);
    
    // Forward to game manager
    GameManager::getInstance()->handleKeyPress(key, x, y);
}

// Keyboard up callback function - called when a key is released
void keyboardUp(unsigned char key, int x, int y) {
    GameManager::getInstance()->handleKeyRelease(key, x, y);
}


void specialKeyboard(int key, int x, int y) {

}


void specialKeyboardUp(int key, int x, int y) {

}


void mouseMotion(int x, int y) {
    GameManager::getInstance()->handleMouseMotion(x, y);
}

void mouseButton(int button, int state, int x, int y) {
    GameManager::getInstance()->handleMouseButton(button, state, x, y);
}


void cleanup() {

    delete GameManager::getInstance();
}


int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

    
    Config& config = Config::getInstance();
    int width = config.getScreenWidth();
    int height = config.getScreenHeight();

    
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("ArtSpace - Room & Camera Demo");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutMotionFunc(mouseMotion);
    glutPassiveMotionFunc(mouseMotion);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);
    glutPassiveMotionFunc(mouseMotion);
    glutMouseFunc(mouseButton);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    GameManager::getInstance()->init();

    glutWarpPointer(width / 2, height / 2);

    glutMainLoop();
    
    // Clean up resources
    cleanup();
    
    
    return 0;
}