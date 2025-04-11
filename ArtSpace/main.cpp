#define _USE_MATH_DEFINES // Must be defined before <cmath> to get M_PI
#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <cmath>

// Room dimensions (Global constants)
const float roomSize = 15.0f;
const float wallHeight = 5.0f;

// Window dimensions (Global, updated in reshape)
int windowWidth = 1024;
int windowHeight = 768;
int windowCenterX = windowWidth / 2;
int windowCenterY = windowHeight / 2;

// Camera variables
float eyeX = 0.0f;
float eyeY = 1.7f; // Standard eye height
float eyeZ = 10.0f; // Start inside the room, looking forward
float yawAngle = -M_PI / 2.0f; // Initial angle facing towards negative Z (radians)
float pitchAngle = 0.0f; // Initial pitch angle (level)
float moveSpeed = 0.3f;
float turnSpeed = 0.05f; // Radians per key press (Arrow keys)
float mouseSensitivity = 0.002f; // Adjust as needed

bool isPaused = false; // Keep pause functionality if desired

// --- Forward declarations ---
void specialKeyboard(int key, int x, int y);
void mouseMotion(int x, int y); // Added forward declaration

// --- Keyboard handler for movement ---
void keyboard(unsigned char key, int x, int y) {
	float lookX = cos(yawAngle);
	float lookZ = sin(yawAngle);
	float strafeX = -lookZ;
	float strafeZ = lookX;

	switch (key) {
	case 'w':
	case 'W':
		// Move forward
		eyeX += lookX * moveSpeed;
		eyeZ += lookZ * moveSpeed;
		break;
	case 's':
	case 'S':
		// Move backward
		eyeX -= lookX * moveSpeed;
		eyeZ -= lookZ * moveSpeed;
		break;
	case 'a':
	case 'A':
		// Strafe left
		eyeX += strafeX * moveSpeed;
		eyeZ += strafeZ * moveSpeed;
		break;
	case 'd':
	case 'D':
		// Strafe right
		eyeX -= strafeX * moveSpeed;
		eyeZ -= strafeZ * moveSpeed;
		break;
	case 'p': // Optional pause key
	case 'P':
		isPaused = !isPaused;
		break;
	case 27: // Escape key to exit (optional)
		exit(0);
		break;
	}
	glutPostRedisplay(); // Request redraw after movement/turn
}

// --- Special keyboard handler for turning ---
void specialKeyboard(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		yawAngle -= turnSpeed;
		break;
	case GLUT_KEY_RIGHT:
		yawAngle += turnSpeed;
		break;
	}
	// Keep yaw angle within reasonable bounds if needed (optional)
	// if (yawAngle > 2 * M_PI) yawAngle -= 2 * M_PI;
	// if (yawAngle < 0) yawAngle += 2 * M_PI;
	glutPostRedisplay();
}

// --- Mouse motion handler for looking ---
void mouseMotion(int x, int y) {
	// Calculate mouse movement delta from the center
	int deltaX = x - windowCenterX;
	int deltaY = y - windowCenterY;

	// Update angles based on delta and sensitivity
	yawAngle += deltaX * mouseSensitivity;
	pitchAngle -= deltaY * mouseSensitivity; // Subtract because Y is inverted (0 is top)

	// Clamp pitch angle to avoid looking straight up/down or flipping
	const float pitchLimit = 1.55f; // Approx 89 degrees in radians
	if (pitchAngle > pitchLimit) {
		pitchAngle = pitchLimit;
	}
	if (pitchAngle < -pitchLimit) {
		pitchAngle = -pitchLimit;
	}

	// Keep yaw angle reasonable (optional, prevents large numbers)
	if (yawAngle > 2 * M_PI) yawAngle -= 2 * M_PI;
	if (yawAngle < 0) yawAngle += 2 * M_PI;

	// Warp pointer back to the center of the window
	if (deltaX != 0 || deltaY != 0) { // Only warp if mouse actually moved
		glutWarpPointer(windowCenterX, windowCenterY);
	}

	glutPostRedisplay(); // Request redraw after view change
}

void mouse(int button, int state, int x, int y) {
	// Mouse clicks currently do nothing
}

void update(int value) {
	// No animation updates needed in this version unless isPaused affects something
	if (!isPaused) {
		// Potentially add other time-based logic here if needed later
	}
	glutTimerFunc(16, update, 0); // Keep timer running for responsiveness
}

// --- Helper to draw skirting board ---
void drawSkirtingBoard(float length, float thickness, float height) {
	glPushMatrix();
	glColor3f(0.3f, 0.2f, 0.1f); // Dark wood-like color
	glScalef(length, height, thickness);
	glutSolidCube(1.0f); // Use 1.0 cube scaled
	glPopMatrix();
}

// --- Function to draw the room ---
void drawRoom() {
	glPushMatrix();

	// Floor (Slightly darker green)
	glColor3f(0.1f, 0.4f, 0.1f);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0); 
	glVertex3f(-roomSize, 0.0f, -roomSize);
	glVertex3f(roomSize, 0.0f, -roomSize);
	glVertex3f(roomSize, 0.0f, roomSize);
	glVertex3f(-roomSize, 0.0f, roomSize);
	glEnd();

	// Ceiling (Off-white)
	glColor3f(0.9f, 0.9f, 0.85f);
	glBegin(GL_QUADS);
	glNormal3f(0.0, -1.0, 0.0); 
	glVertex3f(-roomSize, wallHeight, -roomSize);
	glVertex3f(-roomSize, wallHeight, roomSize);
	glVertex3f(roomSize, wallHeight, roomSize);
	glVertex3f(roomSize, wallHeight, -roomSize);
	glEnd();

	// Wall Colors - Muted Tones
	GLfloat colorWallNear[3] = {0.8f, 0.8f, 0.75f}; // Beige/Off-white near
	GLfloat colorWallFar[3] = {0.75f, 0.75f, 0.8f};  // Slightly cooler far
	GLfloat colorWallLeft[3] = {0.7f, 0.8f, 0.7f};  // Slightly greenish left
	GLfloat colorWallRight[3] = {0.8f, 0.7f, 0.7f}; // Slightly reddish right

	// Define a slightly darker color for bottom vertices for subtle gradient
	float gradientFactor = 0.95f;

	// Front Wall (-Z)
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	glColor3f(colorWallNear[0] * gradientFactor, colorWallNear[1] * gradientFactor, colorWallNear[2] * gradientFactor);
	glVertex3f(-roomSize, 0.0f, -roomSize);
	glVertex3f(roomSize, 0.0f, -roomSize);
	glColor3fv(colorWallNear); // Use full color for top vertices
	glVertex3f(roomSize, wallHeight, -roomSize);
	glVertex3f(-roomSize, wallHeight, -roomSize);
	glEnd();

	// Back Wall (+Z)
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, -1.0);
	glColor3f(colorWallFar[0] * gradientFactor, colorWallFar[1] * gradientFactor, colorWallFar[2] * gradientFactor);
	glVertex3f(-roomSize, 0.0f, roomSize);
	glColor3fv(colorWallFar);
	glVertex3f(-roomSize, wallHeight, roomSize);
	glVertex3f(roomSize, wallHeight, roomSize);
	glColor3f(colorWallFar[0] * gradientFactor, colorWallFar[1] * gradientFactor, colorWallFar[2] * gradientFactor);
	glVertex3f(roomSize, 0.0f, roomSize);
	glEnd();

	// Left Wall (-X)
	glBegin(GL_QUADS);
	glNormal3f(1.0, 0.0, 0.0);
	glColor3f(colorWallLeft[0] * gradientFactor, colorWallLeft[1] * gradientFactor, colorWallLeft[2] * gradientFactor);
	glVertex3f(-roomSize, 0.0f, -roomSize);
	glColor3fv(colorWallLeft);
	glVertex3f(-roomSize, wallHeight, -roomSize);
	glVertex3f(-roomSize, wallHeight, roomSize);
	glColor3f(colorWallLeft[0] * gradientFactor, colorWallLeft[1] * gradientFactor, colorWallLeft[2] * gradientFactor);
	glVertex3f(-roomSize, 0.0f, roomSize);
	glEnd();

	// Right Wall (+X)
	glBegin(GL_QUADS);
	glNormal3f(-1.0, 0.0, 0.0);
	glColor3f(colorWallRight[0] * gradientFactor, colorWallRight[1] * gradientFactor, colorWallRight[2] * gradientFactor);
	glVertex3f(roomSize, 0.0f, -roomSize);
	glColor3f(colorWallRight[0] * gradientFactor, colorWallRight[1] * gradientFactor, colorWallRight[2] * gradientFactor);
	glVertex3f(roomSize, 0.0f, roomSize);
	glColor3fv(colorWallRight);
	glVertex3f(roomSize, wallHeight, roomSize);
	glVertex3f(roomSize, wallHeight, -roomSize);
	glEnd();

	// --- Skirting Boards --- 
	float skirtingHeight = 0.2f;
	float skirtingThickness = 0.1f;
	float skirtingOffset = 0.01f; // To avoid Z-fighting

	// Front wall skirting
	glPushMatrix();
	glTranslatef(0.0f, skirtingHeight / 2.0f, -roomSize + skirtingThickness / 2.0f + skirtingOffset);
	drawSkirtingBoard(roomSize * 2.0f, skirtingThickness, skirtingHeight);
	glPopMatrix();

	// Back wall skirting
	glPushMatrix();
	glTranslatef(0.0f, skirtingHeight / 2.0f, roomSize - skirtingThickness / 2.0f - skirtingOffset);
	drawSkirtingBoard(roomSize * 2.0f, skirtingThickness, skirtingHeight);
	glPopMatrix();

	// Left wall skirting
	glPushMatrix();
	glTranslatef(-roomSize + skirtingThickness / 2.0f + skirtingOffset, skirtingHeight / 2.0f, 0.0f);
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f); // Rotate to align with wall
	drawSkirtingBoard(roomSize * 2.0f, skirtingThickness, skirtingHeight);
	glPopMatrix();

	// Right wall skirting
	glPushMatrix();
	glTranslatef(roomSize - skirtingThickness / 2.0f - skirtingOffset, skirtingHeight / 2.0f, 0.0f);
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f); // Rotate to align with wall
	drawSkirtingBoard(roomSize * 2.0f, skirtingThickness, skirtingHeight);
	glPopMatrix();


	glPopMatrix(); // End of room matrix
}

// --- End of Room Drawing ---

void draw() {
	// Clear color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity(); // Reset modelview matrix

	// Calculate LookAt point based on camera position and yaw/pitch angles
	float lx = cos(yawAngle) * cos(pitchAngle);
	float ly = sin(pitchAngle);
	float lz = sin(yawAngle) * cos(pitchAngle);
	float lookAtX = eyeX + lx;
	float lookAtY = eyeY + ly;
	float lookAtZ = eyeZ + lz;

	// Set up the camera using dynamic values
	gluLookAt(eyeX, eyeY, eyeZ,       
			  lookAtX, lookAtY, lookAtZ, 
			  0.0, 1.0, 0.0);      

	// Enable features for 3D
	glEnable(GL_DEPTH_TEST); // Enable depth testing
	glEnable(GL_LIGHTING);   // Enable lighting
	glEnable(GL_COLOR_MATERIAL); // Enable color tracking
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); // Apply color

	// --- Set up Lights --- 
	// Light 0 (Main Ceiling Light - White, Attenuated)
	glEnable(GL_LIGHT0);     
	GLfloat light0_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat light0_diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f }; 
	GLfloat light0_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f }; 
	GLfloat light0_position[] = { 0.0f, wallHeight * 0.9f, 0.0f, 1.0f }; // Positional (w=1.0)

	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	// Attenuation for Light 0 (makes it dimmer further away)
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.02f);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.005f);

	// Light 1 (Secondary Fill Light - Warm, Dimmer, Different Position)
	glEnable(GL_LIGHT1);
	GLfloat light1_ambient[] = { 0.1f, 0.08f, 0.05f, 1.0f }; // Very dim warm ambient
	GLfloat light1_diffuse[] = { 0.3f, 0.25f, 0.15f, 1.0f }; // Dim warm diffuse
	GLfloat light1_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f }; // Low specular
	GLfloat light1_position[] = { -roomSize * 0.8f, wallHeight * 0.5f, roomSize * 0.8f, 1.0f }; // Positional, in a corner

	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	// No attenuation on this one for simplicity, acts as fill light

	// Set material properties (Make walls matte)
	GLfloat mat_specular[] = { 0.05f, 0.05f, 0.05f, 1.0f }; // Very low specular reflection
	GLfloat mat_shininess[] = { 2.0f }; // Very low shininess
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	// No global rotation needed now
	// glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);

	// --- Draw the scene --- 
	drawRoom();


	// Disable features after drawing
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_COLOR_MATERIAL);

	glutSwapBuffers(); // Swap buffers for double buffering
}

void reshape(int w, int h) {
	// Prevent divide by zero
	if (h == 0) h = 1;

	windowWidth = w;
	windowHeight = h;
	windowCenterX = w / 2;
	windowCenterY = h / 2;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Keep perspective projection
	gluPerspective(60.0, (double)w / (double)h, 0.1, 100.0); // Wider FOV, adjusted near/far planes
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void backGround() {
	// Background color doesn't matter much when inside the room
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void init_glut(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
}

void create_window() {
	glutInitWindowSize(windowWidth, windowHeight); // Use global vars
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Navigable Room - Mouse Look"); // Updated title
	glutSetCursor(GLUT_CURSOR_NONE); // Hide the cursor
}

void register_functions() {
	glutDisplayFunc(draw);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);    // Handles W, A, S, D
	glutSpecialFunc(specialKeyboard); // Handles Arrow Keys (optional turning)
	glutPassiveMotionFunc(mouseMotion); // Handles mouse look
	glutMouseFunc(mouse);          // Handles clicks (currently unused)
	glutTimerFunc(0, update, 0);   // Keep timer running
	backGround();
}

int main(int argc, char** argv) {
	// Define M_PI if not available (for non-C99 compilers perhaps)
	// #ifndef M_PI
    // #define M_PI 3.14159265358979323846
	// #endif // Removed this block

	srand(static_cast<unsigned int>(time(nullptr)));

	init_glut(argc, argv);
	create_window();
	register_functions();
	glutMainLoop();
	return 0;
}