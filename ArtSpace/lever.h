//#pragma once
//#include <GL/glut.h>
//#include <cmath> // For sin/cos if needed, and smooth animation
//
//// --- Global Variables ---
//
//// Animation state
//float leverAngle = -30.0f; // Initial angle (e.g., OFF state)
//float targetAngle = -30.0f; // Target angle for animation
//bool isAnimating = false;   // Flag to indicate if animation is running
//const float ANIMATION_SPEED = 1.0f; // Degrees per frame
//
//// Camera position
//float cameraAngleX = 20.0f;
//float cameraAngleY = 30.0f;
//float cameraDistance = 15.0f;
//
//// Mouse interaction state for camera control
//int lastMouseX, lastMouseY;
//bool isDragging = false;
//
//// --- Material Properties for Bulbs ---
//GLfloat bulb_off_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // No emission (off)
//GLfloat bulb_green_emission[] = { 0.1f, 0.8f, 0.1f, 1.0f }; // Green emission (on)
//GLfloat bulb_ambient_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f }; // Base color of the bulb (whitish grey)
//GLfloat bulb_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
//GLfloat bulb_shininess = 32.0f;
//
//// --- Function Prototypes ---
//void display();
//void reshape(int w, int h);
//void keyboard(unsigned char key, int x, int y);
//void mouse(int button, int state, int x, int y);
//void motion(int x, int y);
//void idle();
//void initGL();
//void drawSwitch(float angle);
//void drawCylinder(float radius, float height, int slices); // Helper for cylinder
//
//// --- Main Function ---
//int main(int argc, char** argv) {
//    glutInit(&argc, argv); // Initialize GLUT
//    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Double buffering, RGB color, Depth buffer
//    glutInitWindowSize(800, 600); // Set window size
//    glutInitWindowPosition(100, 100); // Set window position
//    glutCreateWindow("OpenGL 3D Switch Lever with Bulbs"); // Create window with title
//
//    initGL(); // Initialize OpenGL settings
//
//    // Register callback functions
//    glutDisplayFunc(display);       // Drawing callback
//    glutReshapeFunc(reshape);       // Window reshape callback
//    glutKeyboardFunc(keyboard);     // Keyboard input callback
//    glutMouseFunc(mouse);           // Mouse button callback
//    glutMotionFunc(motion);         // Mouse motion callback
//    glutIdleFunc(idle);             // Idle callback for animation
//
//    glutMainLoop(); // Enter the GLUT event processing loop
//    return 0;
//}
//
//// --- OpenGL Initialization ---
//void initGL() {
//    glClearColor(0.1f, 0.1f, 0.2f, 1.0f); // Set background color (dark blueish)
//    glEnable(GL_DEPTH_TEST); // Enable depth testing for 3D rendering
//    glEnable(GL_LIGHTING);   // Enable lighting
//    glEnable(GL_LIGHT0);     // Enable light source 0
//    // We'll set material properties explicitly, so disable color material for more control
//    // glEnable(GL_COLOR_MATERIAL);
//    // glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
//
//    // Define light properties for light 0
//    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
//    GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
//    GLfloat light_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
//    GLfloat light_position[] = { 10.0f, 10.0f, 10.0f, 1.0f }; // Positional light
//
//    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
//    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
//    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
//
//    glShadeModel(GL_SMOOTH); // Enable smooth shading
//}
//
//// --- Drawing Functions ---
//
//// Helper function to draw a simple cylinder (since glutSolidCylinder is often problematic)
//void drawCylinder(float radius, float height, int slices) {
//    GLUquadric* quad = gluNewQuadric();
//    gluQuadricNormals(quad, GLU_SMOOTH); // Generate smooth normals
//
//    // Draw the main body of the cylinder
//    gluCylinder(quad, radius, radius, height, slices, 1);
//
//    // Draw the top cap
//    glPushMatrix();
//    glTranslatef(0.0f, 0.0f, height);
//    gluDisk(quad, 0.0, radius, slices, 1); // Inner radius 0, outer radius 'radius'
//    glPopMatrix();
//
//    // Draw the bottom cap (facing downwards)
//    glPushMatrix();
//    glRotatef(180.0f, 1.0f, 0.0f, 0.0f); // Rotate to face down
//    gluDisk(quad, 0.0, radius, slices, 1);
//    glPopMatrix();
//
//    gluDeleteQuadric(quad); // Free the quadric object
//}
//
//
//// Function to draw the switch lever
//void drawSwitch(float angle) {
//    // Define material properties for non-bulb parts (using glColorMaterial style for simplicity here)
//    glEnable(GL_COLOR_MATERIAL);
//    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
//    // Set default specular and shininess for non-bulb parts
//    GLfloat default_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
//    GLfloat default_shininess = 10.0f;
//    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, default_specular);
//    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, default_shininess);
//
//
//    glPushMatrix(); // Save the current matrix state
//
//    // --- Base ---
//    glPushMatrix();
//    glColor3f(0.4f, 0.4f, 0.5f); // Greyish color for the base
//    glScalef(3.0f, 0.5f, 2.0f); // Scale a cube to form a flat base (dimensions: 3 wide, 0.5 high, 2 deep)
//    glutSolidCube(1.0);
//    glPopMatrix(); // Restore matrix state before base scaling
//
//    // --- Pivot Support ---
//    glPushMatrix();
//    glColor3f(0.6f, 0.6f, 0.6f); // Lighter grey for the support
//    glTranslatef(0.0f, 0.5f, 0.0f); // Position it on top of the base (Y = base_height/2 + support_height/2 = 0.25 + 0.25 = 0.5)
//    glScalef(0.6f, 1.0f, 0.6f); // Make it taller than wide/deep
//    glutSolidCube(1.0);
//    glPopMatrix(); // Restore matrix state before support scaling
//
//    // --- Lever Arm ---
//    glPushMatrix();
//    // Move to the pivot point (top center of the support)
//    glTranslatef(0.0f, 1.0f, 0.0f); // Pivot point Y = base_height + support_height = 0.5 + 0.5 = 1.0
//    // Apply the rotation for the lever's angle
//    glRotatef(angle, 0.0f, 0.0f, 1.0f); // Rotate around Z-axis
//
//    // Draw the main arm
//    glPushMatrix();
//    glColor3f(0.8f, 0.2f, 0.2f); // Reddish color for the lever arm
//    // Position the arm so it pivots correctly (e.g., center its base at the origin)
//    glTranslatef(0.0f, 1.5f, 0.0f); // Move up half its length (arm length = 3.0)
//    glScalef(0.3f, 3.0f, 0.3f); // Scale a cube into a long arm
//    glutSolidCube(1.0);
//    glPopMatrix(); // Restore matrix state before arm scaling
//
//    // --- Handle ---
//    glPushMatrix();
//    glColor3f(0.2f, 0.2f, 0.2f); // Dark color for the handle
//    // Position the handle at the end of the arm
//    glTranslatef(0.0f, 3.0f, 0.0f); // Position relative to the pivot point (length of the arm)
//    glutSolidSphere(0.4, 20, 20); // Draw a sphere as the handle
//    glPopMatrix(); // Restore matrix state before handle translation
//
//    glPopMatrix(); // Restore matrix state before lever rotation/translation
//
//
//    // --- Light Bulbs ---
//    // Disable color material tracking to use glMaterialfv for bulbs
//    glDisable(GL_COLOR_MATERIAL);
//
//    // Set common material properties for bulbs
//    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, bulb_ambient_diffuse);
//    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, bulb_specular);
//    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, bulb_shininess);
//
//    // Bulb 1 (State-dependent)
//    glPushMatrix();
//    // Position on the base (e.g., front left)
//    // Base center is at (0, 0, 0). Base dimensions are 3.0 x 0.5 x 2.0.
//    // Top surface of base is at Y = 0.25.
//    glTranslatef(-0.8f, 0.25f + 0.15f, 0.6f); // X=-0.8, Y=top_base+radius, Z=front
//
//    // Set emission based on lever state (ON if angle > 0)
//    if (angle > 0.0f) {
//        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, bulb_green_emission);
//    }
//    else {
//        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, bulb_off_emission);
//    }
//    glutSolidSphere(0.15, 15, 15); // Draw small sphere bulb
//    // Reset emission to off for safety (so it doesn't affect subsequent objects if logic changes)
//    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, bulb_off_emission);
//    glPopMatrix();
//
//    // Bulb 2 (Always ON Green)
//    glPushMatrix();
//    // Position on the base (e.g., front right)
//    glTranslatef(0.8f, 0.25f + 0.15f, 0.6f); // X=0.8, Y=top_base+radius, Z=front
//
//    // Set emission to green
//    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, bulb_green_emission);
//    glutSolidSphere(0.15, 15, 15); // Draw small sphere bulb
//    // Reset emission to off for safety
//    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, bulb_off_emission);
//    glPopMatrix();
//
//    // Re-enable color material for any subsequent drawing that might rely on it (though none here)
//    // glEnable(GL_COLOR_MATERIAL);
//
//
//    glPopMatrix(); // Restore the initial matrix state saved at the beginning of drawSwitch
//}
//
//// --- Display Callback ---
//void display() {
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
//    glLoadIdentity(); // Reset the model-view matrix
//
//    // --- Camera Transformation ---
//    // Translate back by cameraDistance
//    glTranslatef(0.0f, 0.0f, -cameraDistance);
//    // Rotate the scene based on mouse drag
//    glRotatef(cameraAngleX, 1.0f, 0.0f, 0.0f); // Rotate around X axis
//    glRotatef(cameraAngleY, 0.0f, 1.0f, 0.0f); // Rotate around Y axis
//
//    // --- Draw the Switch ---
//    drawSwitch(leverAngle);
//
//    glutSwapBuffers(); // Swap the front and back buffers (for double buffering)
//}
//
//// --- Reshape Callback ---
//void reshape(int w, int h) {
//    if (h == 0) h = 1; // Prevent division by zero
//    float ratio = (float)w / h;
//
//    glViewport(0, 0, w, h); // Set the viewport to the entire window
//
//    glMatrixMode(GL_PROJECTION); // Switch to Projection matrix
//    glLoadIdentity(); // Reset the projection matrix
//    gluPerspective(45.0f, ratio, 1.0f, 100.0f); // Set perspective projection (FOV, aspect ratio, near clip, far clip)
//
//    glMatrixMode(GL_MODELVIEW); // Switch back to ModelView matrix
//}
//
//// --- Keyboard Callback ---
//void keyboard(unsigned char key, int x, int y) {
//    switch (key) {
//    case 27: // ESC key
//        exit(0); // Exit the application
//        break;
//    case 's': // Toggle switch state
//    case 'S':
//        if (!isAnimating) { // Only start animation if not already animating
//            if (targetAngle == -30.0f) {
//                targetAngle = 30.0f; // Target ON state
//            }
//            else {
//                targetAngle = -30.0f; // Target OFF state
//            }
//            isAnimating = true;
//        }
//        break;
//    case '+': // Zoom in
//        cameraDistance -= 0.5f;
//        if (cameraDistance < 2.0f) cameraDistance = 2.0f; // Prevent getting too close
//        glutPostRedisplay();
//        break;
//    case '-': // Zoom out
//        cameraDistance += 0.5f;
//        if (cameraDistance > 50.0f) cameraDistance = 50.0f; // Prevent getting too far
//        glutPostRedisplay();
//        break;
//    }
//}
//
//// --- Mouse Button Callback ---
//void mouse(int button, int state, int x, int y) {
//    if (button == GLUT_LEFT_BUTTON) {
//        if (state == GLUT_DOWN) {
//            isDragging = true;
//            lastMouseX = x;
//            lastMouseY = y;
//        }
//        else { // state == GLUT_UP
//            isDragging = false;
//        }
//    }
//    else if (button == 3) { // Mouse wheel up (zoom in) - may vary on systems
//        cameraDistance -= 0.5f;
//        if (cameraDistance < 2.0f) cameraDistance = 2.0f;
//        glutPostRedisplay();
//    }
//    else if (button == 4) { // Mouse wheel down (zoom out) - may vary on systems
//        cameraDistance += 0.5f;
//        if (cameraDistance > 50.0f) cameraDistance = 50.0f;
//        glutPostRedisplay();
//    }
//}
//
//// --- Mouse Motion Callback ---
//void motion(int x, int y) {
//    if (isDragging) {
//        // Calculate change in mouse position
//        int deltaX = x - lastMouseX;
//        int deltaY = y - lastMouseY;
//
//        // Update camera angles (adjust sensitivity as needed)
//        cameraAngleY += deltaX * 0.5f;
//        cameraAngleX += deltaY * 0.5f;
//
//        // Clamp cameraAngleX to avoid flipping
//        if (cameraAngleX > 89.0f) cameraAngleX = 89.0f;
//        if (cameraAngleX < -89.0f) cameraAngleX = -89.0f;
//
//        // Store last mouse position
//        lastMouseX = x;
//        lastMouseY = y;
//
//        glutPostRedisplay(); // Request redraw
//    }
//}
//
//
//// --- Idle Callback (for animation) ---
//void idle() {
//    if (isAnimating) {
//        // Move current angle towards target angle
//        if (leverAngle < targetAngle) {
//            leverAngle += ANIMATION_SPEED;
//            if (leverAngle >= targetAngle) {
//                leverAngle = targetAngle; // Snap to target
//                isAnimating = false;      // Stop animation
//            }
//        }
//        else if (leverAngle > targetAngle) {
//            leverAngle -= ANIMATION_SPEED;
//            if (leverAngle <= targetAngle) {
//                leverAngle = targetAngle; // Snap to target
//                isAnimating = false;      // Stop animation
//            }
//        }
//        glutPostRedisplay(); // Request redraw if animating
//    }
//}