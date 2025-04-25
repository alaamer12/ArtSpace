#include <GL/glut.h>
#include <iostream>
#include "utility.h"
#include "config.h"

// Global variables
Image* realImage = nullptr;
Image* invalidImage = nullptr;
Image* noFallbackImage = nullptr;
bool preserveAspectRatio = true;
float tintR = 1.0f, tintG = 1.0f, tintB = 1.0f;

// Function prototypes
void display();
void reshape(int width, int height);
void update();
void keyboard(unsigned char key, int x, int y);
void cleanup();

// Display callback function
void display() {
    // Clear the color buffer
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Set up 2D orthographic projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT), -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Render our images
    if (realImage) {
        realImage->render();
    }

    if (invalidImage) {
        invalidImage->render();
    }

    if (noFallbackImage) {
        noFallbackImage->render();
    }

    // Swap buffers
    glutSwapBuffers();
}

// Reshape callback function
void reshape(int width, int height) {
    // Update viewport
    glViewport(0, 0, width, height);

    // Update image positions
    if (realImage && invalidImage && noFallbackImage) {
        float centerX = (width - realImage->getSize()[0]) / 2;

        // Position the images one above another with padding
        float padding = 20.0f;
        float totalHeight = realImage->getSize()[1] + invalidImage->getSize()[1] +
            noFallbackImage->getSize()[1] + (2 * padding);

        float currentY = (height - totalHeight) / 2 + totalHeight - realImage->getSize()[1];

        // Top image (valid)
        realImage->setPosition(centerX, currentY);

        // Middle image (invalid with fallback)
        currentY -= (invalidImage->getSize()[1] + padding);
        invalidImage->setPosition(centerX, currentY);

        // Bottom image (invalid without fallback)
        currentY -= (noFallbackImage->getSize()[1] + padding);
        noFallbackImage->setPosition(centerX, currentY);
    }
}

// Update function for animation
void update() {
    // Nothing to update for now
    glutPostRedisplay();
}

// Keyboard callback function
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 27: // ESC key
        cleanup();
        exit(0);
        break;

    case 'a':
    case 'A':
        // Toggle aspect ratio preservation
        preserveAspectRatio = !preserveAspectRatio;
        if (realImage) {
            realImage->setPreserveAspectRatio(preserveAspectRatio);
        }
        if (invalidImage) {
            invalidImage->setPreserveAspectRatio(preserveAspectRatio);
        }
        if (noFallbackImage) {
            noFallbackImage->setPreserveAspectRatio(preserveAspectRatio);
        }
        break;

    case 'r':
    case 'R':
        // Adjust red tint
        tintR = (tintR > 0.5f) ? 0.0f : 1.0f;
        if (realImage) {
            realImage->setTint(tintR, tintG, tintB);
        }
        break;

    case 'g':
    case 'G':
        // Adjust green tint
        tintG = (tintG > 0.5f) ? 0.0f : 1.0f;
        if (realImage) {
            realImage->setTint(tintR, tintG, tintB);
        }
        break;

    case 'b':
    case 'B':
        // Adjust blue tint
        tintB = (tintB > 0.5f) ? 0.0f : 1.0f;
        if (realImage) {
            realImage->setTint(tintR, tintG, tintB);
        }
        break;
    }

    glutPostRedisplay();
}

// Cleanup function
void cleanup() {
    if (realImage) {
        delete realImage;
        realImage = nullptr;
    }

    if (invalidImage) {
        delete invalidImage;
        invalidImage = nullptr;
    }

    if (noFallbackImage) {
        delete noFallbackImage;
        noFallbackImage = nullptr;
    }
}

int main(int argc, char** argv) {
    // Initialize logger
    Logger& logger = Logger::getInstance();
    logger.enableConsoleLogging(true);
    logger.logInfo("ArtSpace Image Demo starting...");

    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    // Get screen size from config
    Config& config = Config::getInstance();
    int width = config.getScreenWidth();
    int height = config.getScreenHeight();

    // Create the window
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("ArtSpace Image Demo");

    // Set up callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(update);

    // Load valid image - replace with a real path to a valid image on your system
    // If the image doesn't exist, it'll use the default fallback color (white)
    std::string validImagePath = "E:\\Projects\\Games\\ArtSpace\\ArtSpace\\assets\\pictures\\StarScream.jpg";
    realImage = new Image(validImagePath);
    if (!realImage) {
        logger.logError("Failed to create valid image");
        return 1;
    }

    // If the image doesn't exist, adjust size to have something visible
    if (!realImage->isImageLoaded()) {
        logger.logWarning("Using fallback for supposedly valid image");
    }

    // Configure valid image
    realImage->setSize(300, 200);
    realImage->setPreserveAspectRatio(preserveAspectRatio);
    realImage->setTint(tintR, tintG, tintB);

    // Create invalid image with red fallback
    invalidImage = new Image("nonexistent_image.png", "#ff0000");
    if (!invalidImage) {
        logger.logError("Failed to create invalid image with fallback");
        return 1;
    }

    // Configure invalid image with fallback
    invalidImage->setSize(300, 200);
    invalidImage->setPreserveAspectRatio(preserveAspectRatio);

    // Create invalid image with no fallback - should result in an error message
    // but we'll still create the object to demonstrate what happens
    try {
        noFallbackImage = new Image("another_nonexistent_image.png", "");
        if (noFallbackImage) {
            // Configure it just in case it was created despite the error
            noFallbackImage->setSize(300, 200);
            noFallbackImage->setPreserveAspectRatio(preserveAspectRatio);
            noFallbackImage->setVisible(false); // Hide it since it shouldn't render anything
        }
    }
    catch (...) {
        logger.logError("Exception creating image without fallback");
        noFallbackImage = nullptr;
    }

    reshape(width, height);

    logger.logInfo("Images created and configured");

    // Start the main loop
    glutMainLoop();

    cleanup();
    return 0;
}
