#pragma once
#include <GL/glut.h>

class Lever {
private:
    // Animation state
    float leverAngle;       // Current angle (-30.0f is OFF, 30.0f is ON)
    float targetAngle;    
    bool isAnimating;     
    const float ANIMATION_SPEED = 2.0f;

    float posX, posY, posZ;
    float scale;
    bool isWestWall;

    bool activated;          // Has the lever been activated (Bulb 1)
    bool won;                // Has the user won (Bulb 2 permanent green)
    bool showingError;       // Is currently showing error (Bulb 2 temporary red)
    float errorTimer;        // Timer for error display
    
    bool waitingBeforeReset;
    float resetDelayTimer;  

    // Material properties for bulbs
    GLfloat bulb_off_emission[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; // No emission (off)
    GLfloat bulb_green_emission[4] = { 0.1f, 0.8f, 0.1f, 1.0f }; // Green emission (on)
    GLfloat bulb_red_emission[4] = { 0.8f, 0.1f, 0.1f, 1.0f }; // Red emission (error)
    GLfloat bulb_ambient_diffuse[4] = { 0.8f, 0.8f, 0.8f, 1.0f }; // Base color of the bulb (whitish grey)
    GLfloat bulb_specular[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat bulb_shininess = 32.0f;

    void drawCylinder(float radius, float height, int slices) {
        GLUquadric* quad = gluNewQuadric();
        gluQuadricNormals(quad, GLU_SMOOTH); // Generate smooth normals

        // Draw the main body of the cylinder
        gluCylinder(quad, radius, radius, height, slices, 1);

        // Draw the top cap
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, height);
        gluDisk(quad, 0.0, radius, slices, 1); // Inner radius 0, outer radius 'radius'
        glPopMatrix();

        // Draw the bottom cap (facing downwards)
        glPushMatrix();
        glRotatef(180.0f, 1.0f, 0.0f, 0.0f); // Rotate to face down
        gluDisk(quad, 0.0, radius, slices, 1);
        glPopMatrix();

        gluDeleteQuadric(quad); // Free the quadric object
    }

public:
    Lever(float x = 0.0f, float y = 0.0f, float z = 0.0f, float leverScale = 1.0f)
        : leverAngle(-30.0f), targetAngle(-30.0f), isAnimating(false),
          posX(x), posY(y), posZ(z), scale(leverScale),
          activated(false), won(false), showingError(false), errorTimer(0.0f),
          waitingBeforeReset(false), resetDelayTimer(0.0f) {
        // Determine if this is a west wall lever based on position
        isWestWall = (x < 0);
    }

    void reset() {
        leverAngle = -30.0f;
        targetAngle = -30.0f;
        isAnimating = false;
        activated = false;
        won = false;
        showingError = false;
        errorTimer = 0.0f;
        waitingBeforeReset = false;
        resetDelayTimer = 0.0f;
    }

    void activate(bool winConditionMet) {
        if (!won) {
            // Set lever to ON position
            if (targetAngle != 30.0f) {
                targetAngle = 30.0f;
                isAnimating = true;
            }
            
            activated = true;
            
            if (winConditionMet) {
                won = true;
                showingError = false;
                errorTimer = 0.0f;
                waitingBeforeReset = false;
                resetDelayTimer = 0.0f;
            } else {
                showingError = true;
                errorTimer = 0.0f;
                
                waitingBeforeReset = true;
                resetDelayTimer = 0.0f;
            }
        }
    }

    void update(float deltaTime) {
        // Update lever animation
        if (isAnimating) {
            // Move current angle towards target angle
            if (leverAngle < targetAngle) {
                leverAngle += ANIMATION_SPEED;
                if (leverAngle >= targetAngle) {
                    leverAngle = targetAngle; // Snap to target
                    isAnimating = false;      // Stop animation
                }
            }
            else if (leverAngle > targetAngle) {
                leverAngle -= ANIMATION_SPEED;
                if (leverAngle <= targetAngle) {
                    leverAngle = targetAngle; // Snap to target
                    isAnimating = false;      // Stop animation
                }
            }
        }
        
        if (showingError) {
            errorTimer += deltaTime;
            // Error display lasts for 2 seconds
            if (errorTimer >= 2.0f) {
                showingError = false;
                errorTimer = 0.0f;
            }
        }
        
       
        if (waitingBeforeReset) {
            resetDelayTimer += deltaTime;
            // Wait 0.5 seconds before returning lever to OFF state
            if (resetDelayTimer >= 0.5f) {
                // Return the lever to OFF state after wrong answer
                targetAngle = -30.0f;
                isAnimating = true;
                waitingBeforeReset = false;
                resetDelayTimer = 0.0f;
            }
        }
    }

    void render() {
        // Define material properties for non-bulb parts
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        
        GLfloat default_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
        GLfloat default_shininess = 10.0f;
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, default_specular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, default_shininess);

        glPushMatrix();
        
        glTranslatef(posX, posY, posZ);
        
        if (isWestWall) {
            glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
        }
        
        glScalef(scale, scale, scale);

        // --- Base ---
        glPushMatrix();
        glColor3f(0.4f, 0.4f, 0.5f); 
        glScalef(3.0f, 0.5f, 2.0f);
        glutSolidCube(1.0);
        glPopMatrix();

        // --- Pivot Support ---
        glPushMatrix();
        glColor3f(0.6f, 0.6f, 0.6f);
        glTranslatef(0.0f, 0.5f, 0.0f);
        glScalef(0.6f, 1.0f, 0.6f); 
        glutSolidCube(1.0);
        glPopMatrix();

        // --- Lever Arm ---
        glPushMatrix();
        glTranslatef(0.0f, 1.0f, 0.0f);
        glRotatef(leverAngle, 0.0f, 0.0f, 1.0f);

        // Draw the main arm
        glPushMatrix();
        glColor3f(0.8f, 0.2f, 0.2f); 
        glTranslatef(0.0f, 1.5f, 0.0f); // Move up half its length
        glScalef(0.3f, 3.0f, 0.3f);
        glutSolidCube(1.0);
        glPopMatrix();

        // --- Handle ---
        glPushMatrix();
        glColor3f(0.2f, 0.2f, 0.2f); 
        glTranslatef(0.0f, 3.0f, 0.0f);
        glutSolidSphere(0.4, 20, 20);
        glPopMatrix();

        glPopMatrix();

        // --- Light Bulbs ---
        glDisable(GL_COLOR_MATERIAL);

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, bulb_ambient_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, bulb_specular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, bulb_shininess);

        glPushMatrix();
        glTranslatef(-0.8f, 0.25f + 0.15f, 0.6f); // Position on the base

        if (activated) {
            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, bulb_green_emission);
        } else {
            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, bulb_off_emission);
        }
        glutSolidSphere(0.15, 15, 15); // Draw small sphere bulb
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, bulb_off_emission); // Reset emission
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.8f, 0.25f + 0.15f, 0.6f); // Position on the base

        if (won) {
            // Player has won - show green permanently
            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, bulb_green_emission);
        } else if (showingError) {
            // Showing error - temporary red
            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, bulb_red_emission);
        } else {
            // Default state - no emission
            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, bulb_off_emission);
        }
        
        glutSolidSphere(0.15, 15, 15); // Draw small sphere bulb
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, bulb_off_emission); // Reset emission
        glPopMatrix();

        glPopMatrix(); // Restore the initial matrix state
    }

    bool isOn() const {
        return leverAngle > 0.0f;
    }

    bool hasWon() const {
        return won;
    }

    bool isCurrentlyAnimating() const {
        return isAnimating;
    }
};
