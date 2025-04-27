#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include "room.h"

// Camera position and orientation
float cameraX = 0.0f;
float cameraY = 0.0f;
float cameraZ = 5.0f;
float cameraAngle = 3.14159f;  // Start facing backward

// Room object
Room* room;

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Set up camera position and orientation
    float lookX = cameraX + sin(cameraAngle);
    float lookZ = cameraZ + cos(cameraAngle);
    gluLookAt(cameraX, cameraY, cameraZ, lookX, cameraY, lookZ, 0.0f, 1.0f, 0.0f);

    // Set up lighting
    GLfloat lightPosition[] = { 0.0f, 3.0f, 0.0f, 1.0f };
    GLfloat lightAmbient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);

    // Render the room
    room->render();

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

// Keyboard function for navigation
void keyboard(unsigned char key, int x, int y) {
    float moveSpeed = 0.2f;
    float rotateSpeed = 0.1f;

    const float* dimensions = room->getDimensions();
    float ROOM_WIDTH = dimensions[0];
    float ROOM_HEIGHT = dimensions[1];
    float ROOM_DEPTH = dimensions[2];

    switch (key) {
    case 'w': // Move forward
        cameraX += sin(cameraAngle) * moveSpeed;
        cameraZ += cos(cameraAngle) * moveSpeed;
        break;
    case 's': // Move backward
        cameraX -= sin(cameraAngle) * moveSpeed;
        cameraZ -= cos(cameraAngle) * moveSpeed;
        break;
    case 'a': // Rotate left
        cameraAngle += rotateSpeed;
        break;
    case 'd': // Rotate right
        cameraAngle -= rotateSpeed;
        break;
    case 'q': // Move up
        cameraY += moveSpeed;
        break;
    case 'e': // Move down
        cameraY -= moveSpeed;
        break;
    case 27: // ESC key - exit
        delete room;
        exit(0);
        break;
    }

    // Constrain the camera within the room
    if (cameraX > ROOM_WIDTH / 2 - 1.0f) cameraX = ROOM_WIDTH / 2 - 1.0f;
    if (cameraX < -ROOM_WIDTH / 2 + 1.0f) cameraX = -ROOM_WIDTH / 2 + 1.0f;
    if (cameraY > ROOM_HEIGHT / 2 - 1.0f) cameraY = ROOM_HEIGHT / 2 - 1.0f;
    if (cameraY < -ROOM_HEIGHT / 2 + 1.0f) cameraY = -ROOM_HEIGHT / 2 + 1.0f;
    if (cameraZ > ROOM_DEPTH / 2 - 1.0f) cameraZ = ROOM_DEPTH / 2 - 1.0f;
    if (cameraZ < -ROOM_DEPTH / 2 + 1.0f) cameraZ = -ROOM_DEPTH / 2 + 1.0f;

    glutPostRedisplay();
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

    room->setWallTexture("C:\\Users\\muhammad\\source\\repos\\ArtSpace\\ArtSpace\\assets\\textures\\wall.bmp");
    room->setFloorTexture("C:\\Users\\muhammad\\source\\repos\\ArtSpace\\ArtSpace\\assets\\textures\\floor2.bmp");
    room->setRoofTexture("C:\\Users\\muhammad\\source\\repos\\ArtSpace\\ArtSpace\\assets\\textures\\wall.bmp");

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
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}