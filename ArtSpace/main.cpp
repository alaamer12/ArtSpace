#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include "room.h"
#include "camera.h"

// Camera position and orientation
HumanCamera camera;
InputSystem* input = InputSystem::getInstance();

// Room object
Room* room;

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Set up camera position and orientation
    

    // Set up lighting
    GLfloat lightPosition[] = { 0.0f, 3.0f, 0.0f, 1.0f };
    GLfloat lightAmbient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);

    // Render the room
    room->render();
    input->registerCallbacks();


    camera.update(0.1f);
    camera.applyTransformation();
    camera.setPosition(0.0f, 1.7f, 0.0f);
    camera.setMouseSensitivity(0.5f);
    glutSwapBuffers();
}

// Reshape function
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}


// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Textured 3D Room");

    // Set up OpenGL state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Create room object
    room = new Room(30.0f, 8.0f, 30.0f);

    // Set properties
    
    room->setWallTexture("C:\\Users\\muhammad\\source\\repos\\ArtSpace\\ArtSpace\\assets\\textures\\wall.bmp");
    room->setFloorTexture("C:\\Users\\muhammad\\source\\repos\\ArtSpace\\ArtSpace\\assets\\textures\\floor2.bmp");
    room->setRoofTexture("C:\\Users\\muhammad\\source\\repos\\ArtSpace\\ArtSpace\\assets\\textures\\wall.bmp");
    if (input->isKeyPressed('w')) {
        // Move forward
    }

    float deltaX, deltaY;
    input->getMouseDelta(deltaX, deltaY);
    // Print instructions
    std::cout << "3D Room Navigation:" << std::endl;
    std::cout << "W/S - Move forward/backward" << std::endl;
    std::cout << "A/D - Rotate left/right" << std::endl;
    std::cout << "Q/E - Move up/down" << std::endl;
    std::cout << "1-4 - Change textures" << std::endl;
    std::cout << "ESC - Exit" << std::endl;

    // Register callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}