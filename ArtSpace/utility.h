#pragma once
#include <GL/glut.h>
#include <string>
#include <vector>
#include <functional>
#include <fstream>

// Forward declarations
class Config;

// TextAlignment enumeration for text rendering
enum class TextAlignment {
    MidLeft,  // Align text to the left while vertically centered
    MidMid,   // Center text both horizontally and vertically
    MidRight  // Align text to the right while vertically centered
};

// Logger class for centralized logging
class Logger {
public:
    enum LogLevel {
        INFO,
        WARNING,
        DEBUG,
        ERROR_
    };

private:
    static Logger instance;
    bool consoleLogging;
    bool fileLogging;
    std::string logFilePath;
    std::ofstream logFile;
    LogLevel minLevel;

    // Private constructor (singleton)
    Logger();

public:
    // Delete copy constructor and assignment operator
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    // Destructor
    ~Logger();

    // Get singleton instance
    static Logger& getInstance();
    
    // Configuration
    void enableConsoleLogging(bool enable);
    void enableFileLogging(bool enable, const std::string& filePath = "artspace.log");
    void setMinimumLogLevel(LogLevel level);
    
    // Logging methods
    void logInfo(const std::string& message);
    void logWarning(const std::string& message);
    void logError(const std::string& message);
    void logDebug(const std::string& message);
    
    // Log with custom level
    void log(LogLevel level, const std::string& message);
};

// Forward text-related function declarations
void calculateTextPosition(float boxX, float boxY, float boxWidth, float boxHeight, 
                         const std::string& text, TextAlignment alignment,
                         float& outX, float& outY);
void renderText(float x, float y, const std::string& text, float* color = nullptr);
void renderText(float boxX, float boxY, float boxWidth, float boxHeight, 
               const std::string& text, TextAlignment alignment, float* color = nullptr);

// Base UI component class
class UIComponent {
protected:
    float position[2];  // x, y in screen coordinates
    float size[2];      // width, height
    bool isVisible;
    float alpha;

public:
    UIComponent();
    virtual ~UIComponent() = default;
    
    virtual void render() = 0;
    virtual void update(float deltaTime) {}
    
    void setPosition(float x, float y);
    void setSize(float width, float height);
    void setVisible(bool visible);
    void setAlpha(float a);
    
    bool isPointInside(float x, float y) const;
    const float* getPosition() const { return position; }
    const float* getSize() const { return size; }
    bool getVisible() const { return isVisible; }
};

// Button component
class Button : public UIComponent {
private:
    std::string text;
    float textColor[4];     // RGBA
    float backgroundColor[4]; // RGBA
    float hoverColor[4];    // RGBA
    bool isHovered;
    bool isPressed;
    std::function<void()> onClick;

public:
    Button(const std::string& buttonText);
    ~Button() override = default;
    
    void render() override;
    void update(float deltaTime) override;
    
    void setText(const std::string& buttonText);
    void setOnClick(const std::function<void()>& callback);
    void setTextColor(float r, float g, float b, float a = 1.0f);
    void setBackgroundColor(float r, float g, float b, float a = 1.0f);
    void setHoverColor(float r, float g, float b, float a = 1.0f);
    
    bool handleMouseEvent(int button, int state, int x, int y);
    bool handleMouseMove(int x, int y);
};

// TextBox component
class TextBox : public UIComponent {
private:
    std::string text;
    std::string placeholder;
    float textColor[4];         // RGBA
    float backgroundColor[4];   // RGBA
    float borderColor[4];       // RGBA
    bool isEditable;
    bool hasFocus;
    int cursorPosition;
    float cursorBlinkTime;
    bool showCursor;

public:
    TextBox(const std::string& initialText = "", const std::string& placeholderText = "");
    ~TextBox() override = default;
    
    void render() override;
    void update(float deltaTime) override;
    
    void setText(const std::string& newText);
    const std::string& getText() const;
    void setPlaceholder(const std::string& newPlaceholder);
    void setEditable(bool editable);
    void setTextColor(float r, float g, float b, float a = 1.0f);
    void setBackgroundColor(float r, float g, float b, float a = 1.0f);
    void setBorderColor(float r, float g, float b, float a = 1.0f);
    
    bool handleKeyEvent(unsigned char key);
    bool handleMouseEvent(int button, int state, int x, int y);
};

// Timer display component
class Timer : public UIComponent {
private:
    std::string timeText;
    float currentTime;
    bool isRunning;
    float textColor[4];  // RGBA

public:
    Timer();
    ~Timer() override = default;
    
    void start();
    void stop();
    void reset();
    void update(float deltaTime) override;
    void render() override;
    float getTime() const;
    void setTextColor(float r, float g, float b, float a = 1.0f);
};

// Screen navigator
enum class Screen {
    StartScreen,
    Level1Screen,
    GameStateScreen,
    PauseScreen,
    EndScreen
};

class Navigator {
private:
    Screen currentScreen;
    std::vector<UIComponent*> screenComponents;

public:
    Navigator();
    ~Navigator();
    
    void navigateTo(Screen screen);
    Screen getCurrentScreen() const;
    void addComponent(UIComponent* component);
    void removeComponent(UIComponent* component);
    void renderCurrentScreen();
    void updateCurrentScreen(float deltaTime);
    
    bool handleMouseEvent(int button, int state, int x, int y);
    bool handleKeyEvent(unsigned char key);
    bool handleMouseMove(int x, int y);
};

/*
 * Usage Example:
 * 
 * Here's how to integrate these UI components in your main.cpp file:
 * 
 * // Global variables
 * Navigator* navigator = nullptr;
 * Button* startButton = nullptr;
 * Button* quitButton = nullptr;
 * TextBox* playerNameBox = nullptr;
 * Timer* gameTimer = nullptr;
 * float lastFrameTime = 0.0f;
 * 
 * // Setup UI components and navigation
 * void setupUI() {
 *     // Create the navigator
 *     navigator = new Navigator();
 * 
 *     // Create start screen components
 *     startButton = new Button("Start Game");
 *     startButton->setPosition(glutGet(GLUT_WINDOW_WIDTH) / 2 - 100, glutGet(GLUT_WINDOW_HEIGHT) / 2);
 *     startButton->setSize(200, 40);
 *     startButton->setBackgroundColor(0.1f, 0.4f, 0.8f);
 *     startButton->setHoverColor(0.2f, 0.5f, 0.9f);
 *     startButton->setOnClick([]() {
 *         // Switch to game screen
 *         navigator->navigateTo(Screen::Level1Screen);
 *         // Start the timer
 *         gameTimer->reset();
 *         gameTimer->start();
 *     });
 * 
 *     quitButton = new Button("Quit");
 *     quitButton->setPosition(glutGet(GLUT_WINDOW_WIDTH) / 2 - 100, glutGet(GLUT_WINDOW_HEIGHT) / 2 + 60);
 *     quitButton->setSize(200, 40);
 *     quitButton->setBackgroundColor(0.8f, 0.1f, 0.1f);
 *     quitButton->setHoverColor(0.9f, 0.2f, 0.2f);
 *     quitButton->setOnClick([]() {
 *         exit(0);
 *     });
 * 
 *     playerNameBox = new TextBox("", "Enter your name");
 *     playerNameBox->setPosition(glutGet(GLUT_WINDOW_WIDTH) / 2 - 150, glutGet(GLUT_WINDOW_HEIGHT) / 2 - 60);
 *     playerNameBox->setSize(300, 40);
 * 
 *     // Create game screen components
 *     gameTimer = new Timer();
 *     gameTimer->setPosition(glutGet(GLUT_WINDOW_WIDTH) - 100, 20);
 *     gameTimer->setTextColor(1.0f, 1.0f, 0.0f);  // Yellow
 *     
 *     // Add components to navigator
 *     navigator->addComponent(startButton);
 *     navigator->addComponent(quitButton);
 *     navigator->addComponent(playerNameBox);
 *     navigator->addComponent(gameTimer);
 * 
 *     // Set initial screen
 *     navigator->navigateTo(Screen::StartScreen);
 * }
 * 
 * // GLUT display callback
 * void display() {
 *     // Clear the screen
 *     glClear(GL_COLOR_BUFFER_BIT);
 *     
 *     // Set up orthographic projection for UI
 *     glMatrixMode(GL_PROJECTION);
 *     glLoadIdentity();
 *     glOrtho(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT), -1, 1);
 *     
 *     glMatrixMode(GL_MODELVIEW);
 *     glLoadIdentity();
 *     
 *     // Calculate delta time for animations
 *     float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
 *     float deltaTime = currentTime - lastFrameTime;
 *     lastFrameTime = currentTime;
 *     
 *     // Show/hide components based on current screen
 *     Screen currentScreen = navigator->getCurrentScreen();
 *     startButton->setVisible(currentScreen == Screen::StartScreen);
 *     quitButton->setVisible(currentScreen == Screen::StartScreen);
 *     playerNameBox->setVisible(currentScreen == Screen::StartScreen);
 *     gameTimer->setVisible(currentScreen == Screen::Level1Screen);
 *     
 *     // Update and render UI components
 *     navigator->updateCurrentScreen(deltaTime);
 *     navigator->renderCurrentScreen();
 *     
 *     // Swap buffers
 *     glutSwapBuffers();
 * }
 * 
 * // GLUT mouse callback
 * void mouse(int button, int state, int x, int y) {
 *     navigator->handleMouseEvent(button, state, x, y);
 * }
 * 
 * // GLUT keyboard callback
 * void keyboard(unsigned char key, int x, int y) {
 *     // Handle ESC key globally
 *     if (key == 27) {
 *         exit(0);
 *     }
 *     
 *     // Let navigator handle other keys
 *     navigator->handleKeyEvent(key);
 * }
 * 
 * // GLUT mouse motion callback
 * void motion(int x, int y) {
 *     navigator->handleMouseMove(x, y);
 * }
 * 
 * // Main function
 * int main(int argc, char** argv) {
 *     // Initialize GLUT
 *     glutInit(&argc, argv);
 *     glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
 *     glutInitWindowSize(800, 600);
 *     glutCreateWindow("ArtSpace");
 *     
 *     // Set up OpenGL state
 *     glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
 *     glEnable(GL_BLEND);
 *     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 *     
 *     // Set up UI
 *     setupUI();
 *     
 *     // Register GLUT callbacks
 *     glutDisplayFunc(display);
 *     glutIdleFunc(display);  // Call display continuously for animations
 *     glutMouseFunc(mouse);
 *     glutKeyboardFunc(keyboard);
 *     glutPassiveMotionFunc(motion);
 *     glutMotionFunc(motion);
 *     
 *     // Clean up on exit
 *     atexit([]() {
 *         delete navigator;
 *         // Note: navigator doesn't own components, so we need to delete them separately
 *         delete startButton;
 *         delete quitButton;
 *         delete playerNameBox;
 *         delete gameTimer;
 *     });
 *     
 *     // Start the main loop
 *     glutMainLoop();
 *     return 0;
 * }
 */
