#include <GL/glut.h>
#include <iostream>
#include "screen.h"
#include "room.h"
#include "utility.h"
#include "camera.h"
#include "input.h"
#include "config.h"

InputSystem* inputSystem = nullptr;
HumanCamera* camera = nullptr;
Room* room = nullptr;
Navigator* navigator = nullptr;
float lastTime = 0;

StartScreenManager* startScreen = nullptr;
Timer* timer = nullptr;
Level1ScreenManager* level1Screen = nullptr;

// GLUT callbacks
void display() {
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // If in navigation mode, let navigator handle the rendering
    if (navigator && navigator->getCurrentScreen() != Screen::Level1Screen) {
        // Disable depth testing for UI elements
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        
        // Set up 2D projection for UI screens
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, windowWidth, 0, windowHeight);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Enable blending for transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        navigator->renderCurrentScreen();
        
        // Restore depth testing for 3D rendering
        glEnable(GL_DEPTH_TEST);
    }
    else {
        // 3D rendering for Level1Screen
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60.0f, (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Apply camera transformation
        if (camera) {
            camera->applyTransformation();
        }

        // Render the room
        if (room) {
            room->render();
        }
        
        // Now render any 2D UI elements on top of the 3D scene (like timer)
        if (navigator && navigator->getCurrentScreen() == Screen::Level1Screen) {
            // Switch to 2D mode for UI elements
            glDisable(GL_DEPTH_TEST);
            
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluOrtho2D(0, windowWidth, 0, windowHeight);
            
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            
            // Enable blending for transparency
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            // Render UI components only
            navigator->renderCurrentScreen();
            
            // Restore 3D rendering state
            glEnable(GL_DEPTH_TEST);
        }
    }

    glutSwapBuffers();
}

void reshape(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, width, height);

    // Set up perspective projection for 3D view
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (float)width / (float)height, 0.1f, 100.0f);

    Config::getInstance().setScreenWidth(width);
    Config::getInstance().setScreenHeight(height);

    // Notify all screens of the resize if navigator exists
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
    // First check if we're in the Level1Screen (3D view)
    if (navigator && navigator->getCurrentScreen() == Screen::Level1Screen) {
        // Let input system handle key press for camera movement
        inputSystem->handleKeyPress(key, x, y);
    }

    // Always let navigator handle key events for UI
    if (navigator) {
        navigator->handleKeyEvent(key);
    }

    // ESC key exits the application
    if (key == 27) { // ESC key
        exit(0);
    }

    glutPostRedisplay();
}

void keyboardUp(unsigned char key, int x, int y) {
    // Only pass key release to input system when in Level1Screen
    if (navigator && navigator->getCurrentScreen() == Screen::Level1Screen) {
        inputSystem->handleKeyRelease(key, x, y);
    }
}

void motion(int x, int y) {
    // Only let input system handle mouse motion for camera control when in Level1Screen
    if (navigator && navigator->getCurrentScreen() == Screen::Level1Screen) {
        inputSystem->handleMouseMotion(x, y);
    }

    // Always let navigator handle mouse move for UI
    if (navigator) {
        navigator->handleMouseMove(x, y);
    }

    glutPostRedisplay();
}

float getDeltaTime() {
    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    if (deltaTime > 0.1f) deltaTime = 0.1f;
    return deltaTime;
}

void update() {
    float deltaTime = getDeltaTime();
    inputSystem->update();
    
    if (camera) {
        camera->update(deltaTime);

        // Only apply collision detection if room exists
        if (room) {
            const float* roomDimensions = room->getDimensions();
            float roomWidth = roomDimensions[0];
            float roomHeight = roomDimensions[1];
            float roomDepth = roomDimensions[2];

            float pos[3];
            camera->getPosition(pos);

            float wallOffset = 0.2f;
            float floorOffset = 0.1f;

            if (pos[0] < -roomWidth / 2 + wallOffset) pos[0] = -roomWidth / 2 + wallOffset;
            if (pos[0] > roomWidth / 2 - wallOffset) pos[0] = roomWidth / 2 - wallOffset;
            if (pos[2] < -roomDepth / 2 + wallOffset) pos[2] = -roomDepth / 2 + wallOffset;
            if (pos[2] > roomDepth / 2 - wallOffset) pos[2] = roomDepth / 2 - wallOffset;
            if (pos[1] < -roomHeight / 2 + 1.7f + floorOffset) pos[1] = -roomHeight / 2 + 1.7f + floorOffset;

            camera->setPosition(pos[0], pos[1], pos[2]);
        }
    }
}

void idle() {
    update();
    
    // Update current screen if navigator exists
    if (navigator) {
        float deltaTime = getDeltaTime();
        navigator->updateCurrentScreen(deltaTime);
    }
    
    glutPostRedisplay();
}

void initLighting() {
    // Add lighting code here if needed
}

// Add this function to handle screen transitions
void onScreenChange(Screen previousScreen, Screen newScreen) {
    if (newScreen == Screen::Level1Screen) {
        // When entering Level1Screen, center the mouse and possibly hide cursor
        glutWarpPointer(windowWidth / 2, windowHeight / 2);
        glutSetCursor(GLUT_CURSOR_NONE); // Hide cursor in 3D mode
    } else {
        // When leaving Level1Screen, show cursor again
        glutSetCursor(GLUT_CURSOR_INHERIT); // Show normal cursor in UI screens
    }
}

// Initialize all screens and register them with the navigator
void initializeScreens() {
    // Create screen managers
    startScreen = new StartScreenManager();
    timer = new Timer();
    level1Screen = new Level1ScreenManager(timer, room, camera);
    EndScreenManager* endScreen = new EndScreenManager();

    // Set up components for each screen
    startScreen->setupScreen(navigator);
    level1Screen->setupScreen(navigator);
    endScreen->setupScreen(navigator);

    // Register screens with navigator
    navigator->registerScreen(startScreen);
    navigator->registerScreen(level1Screen);
    navigator->registerScreen(endScreen);
    
    // Set callback for screen transitions
    navigator->setOnScreenChangeCallback(onScreenChange);

    // Start with the start screen
    navigator->navigateTo(Screen::StartScreen);
}

void cleanup() {
    if (camera) {
        delete camera;
        camera = nullptr;
    }

    if (room) {
        delete room;
        room = nullptr;
    }
    
    if (startScreen) {
        delete startScreen;
        startScreen = nullptr;
    }
    
    if (timer) {
        delete timer;
        timer = nullptr;
    }
    
    if (level1Screen) {
        delete level1Screen;
        level1Screen = nullptr;
    }
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

    Config& config = Config::getInstance();
    int width = config.getScreenWidth();
    int height = config.getScreenHeight();
    windowWidth = width;
    windowHeight = height;

    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("ArtSpace - Room & Camera Demo");

    // Initialize input system
    inputSystem = InputSystem::getInstance();
    inputSystem->registerCallbacks();

    // Register GLUT callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(motion);
    glutMouseFunc(mouse);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    initLighting();

    // Create the room
    room = new Room(10.0f, 5.0f, 10.0f);
    std::string texturePath = "E:\\Projects\\Games\\ArtSpace\\ArtSpace\\assets\\textures\\floor.bmp";
    room->setWallTexture(texturePath);
    room->setFloorTexture(texturePath);
    room->setRoofTexture(texturePath);

    // Create the camera
    camera = new HumanCamera();
    camera->setPosition(0.0f, 0.0f, 3.0f);

    // Center the mouse cursor
    glutWarpPointer(width / 2, height / 2);

    // Create and initialize the navigation system
    navigator = new Navigator();
    initializeScreens();

    std::cout << "ArtSpace Room & Camera Demo" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  WASD - Move" << std::endl;
    std::cout << "  Mouse - Look around" << std::endl;
    std::cout << "  F - Toggle wireframe mode" << std::endl;
    std::cout << "  +/- - Adjust mouse sensitivity" << std::endl;
    std::cout << "  ESC - Exit" << std::endl;

    lastTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    glutMainLoop();
    
    cleanup();
    return 0;
}