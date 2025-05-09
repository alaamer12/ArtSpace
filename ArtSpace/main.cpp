#include <GL/glut.h>
#include "screen.h"

int lastTime = 0;

// GLUT callbacks
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set up 2D projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Render current screen
    if (navigator) {
        navigator->renderCurrentScreen();
    }

    glutSwapBuffers();
}

void reshape(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, width, height);

    // Notify all screens of the resize
    if (navigator) {
        navigator->resizeAllScreens(width, height);
    }
}

void mouse(int button, int state, int x, int y) {
    if (navigator) {
        navigator->handleMouseEvent(button, state, x, y);
    }

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    if (navigator) {
        navigator->handleKeyEvent(key);
    }

    // ESC key exits the application
    if (key == 27) { // ESC key
        exit(0);
    }

    glutPostRedisplay();
}

void motion(int x, int y) {
    if (navigator) {
        navigator->handleMouseMove(x, y);
    }

    glutPostRedisplay();
}

void idle() {
    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    // Update current screen
    if (navigator) {
        navigator->updateCurrentScreen(deltaTime);
    }

    glutPostRedisplay();
}

// Initialize all screens and register them with the navigator
void initializeScreens() {
    // Create screen managers
    StartScreenManager* startScreen = new StartScreenManager();
    Level1ScreenManager* level1Screen = new Level1ScreenManager();
    //GameStateScreenManager* stateScreen = new GameStateScreenManager();
    //PauseScreenManager* pauseScreen = new PauseScreenManager();
    EndScreenManager* endScreen = new EndScreenManager();

    // Set up components for each screen
    startScreen->setupScreen(navigator);
    level1Screen->setupScreen(navigator);
    /*stateScreen->setupScreen(navigator);
    pauseScreen->setupScreen(navigator);*/
    endScreen->setupScreen(navigator);

    // Register screens with navigator
    navigator->registerScreen(startScreen);
    navigator->registerScreen(level1Screen);
    /*navigator->registerScreen(stateScreen);
    navigator->registerScreen(pauseScreen);*/
    navigator->registerScreen(endScreen);

    // Start with the start screen
    navigator->navigateTo(Screen::StartScreen);
}

// Main function
int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("ArtSpace Demo");

    // Register callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(motion);
    glutIdleFunc(idle);

    // Initialize navigator
    navigator = new Navigator();

    // Initialize screens
    initializeScreens();

    // Initialize time
    lastTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

    // Start main loop
    glutMainLoop();

    // Clean up (this will never be reached in practice)
    delete navigator;

    return 0;
}