#include <GL/glut.h>
#include <iostream>
#include "camera.h"
#include "room.h"
#include "input.h"
#include "config.h"

HumanCamera* camera = nullptr;
Room* room = nullptr;
InputSystem* inputSystem = nullptr;
float lastTime = 0;

void display();
void reshape(int width, int height);
void update();
void keyboard(unsigned char key, int x, int y);
void specialKeyboard(int key, int x, int y);
void mouseMotion(int x, int y);
void mouseButton(int button, int state, int x, int y);
void idle();
void cleanup();
void initLighting();

void display() {
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    camera->applyTransformation();

    room->render();
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

float getDeltaTime() {
    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    if (deltaTime > 0.1f) deltaTime = 0.1f;
    return deltaTime;
}

void idle() {
    update();
    glutPostRedisplay();
}

void update() {
    float deltaTime = getDeltaTime();
    inputSystem->update();
    camera->update(deltaTime);

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

void keyboard(unsigned char key, int x, int y) {
}

void keyboardUp(unsigned char key, int x, int y) {
    inputSystem->handleKeyRelease(key, x, y);
}

void specialKeyboard(int key, int x, int y) {
}

void specialKeyboardUp(int key, int x, int y) {
}

void mouseMotion(int x, int y) {
    inputSystem->handleMouseMotion(x, y);
}

void mouseButton(int button, int state, int x, int y) {

}

void initLighting() {
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

    inputSystem = InputSystem::getInstance();
    inputSystem->registerCallbacks();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    glEnable(GL_DEPTH_TEST);
    initLighting();

    room = new Room(10.0f, 5.0f, 10.0f);

    std::string texturePath = "E:\\Projects\\Games\\ArtSpace\\ArtSpace\\assets\\textures\\floor.bmp";
    room->setWallTexture(texturePath);
    room->setFloorTexture(texturePath);
    room->setRoofTexture(texturePath);

    camera = new HumanCamera();
    camera->setPosition(0.0f, 0.0f, 3.0f);

    glutWarpPointer(width / 2, height / 2);

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
