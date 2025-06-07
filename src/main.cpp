#include <iostream>
#include "Shader.h"
#include "CubePiece.h"
#include "RubiksCube.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Window dimensions
const GLint WIDTH = 800, HEIGHT = 800;

// Mouse control variables
static bool leftMouseButtonPressed = false;
static double lastMouseX = static_cast<double>(WIDTH) / 2.0;
static double lastMouseY = static_cast<double>(HEIGHT) / 2.0;
const float MOUSE_SENSITIVITY = 0.2f;

static float cameraYaw = 45.0f; // Initial yaw, looking towards -Z
static float cameraPitch = 45.0f;   // Initial pitch
static float cameraDistance = 8.0f; // Initial distance from origin

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            leftMouseButtonPressed = true;
            glfwGetCursorPos(window, &lastMouseX, &lastMouseY); // Update last position when button is pressed
        } else if (action == GLFW_RELEASE) {
            leftMouseButtonPressed = false;
        }
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (leftMouseButtonPressed) {
        float deltaX = static_cast<float>(xpos - lastMouseX);
        float deltaY = static_cast<float>(lastMouseY - ypos); // Reversed since y-coordinates go from top to bottom

        lastMouseX = xpos;
        lastMouseY = ypos;

        cameraYaw += deltaX * MOUSE_SENSITIVITY;
        cameraPitch -= deltaY * MOUSE_SENSITIVITY;

        // Constrain pitch to avoid flipping
        if (cameraPitch > 89.0f) cameraPitch = 89.0f;
        if (cameraPitch < -89.0f) cameraPitch = -89.0f;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    cameraDistance -= (float)yoffset * 0.5f; // Adjust sensitivity as needed
    if (cameraDistance < 1.0f)
        cameraDistance = 1.0f;
    if (cameraDistance > 10.0f)
        cameraDistance = 10.0f;
}

// Forward declaration for RubiksCube instance if needed by callback
RubiksCube* globalRubiksCubePtr = nullptr;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    bool clockwise = 0;
    if (action == GLFW_PRESS) { // We only care about initial key presses for now
        clockwise = mods & GLFW_MOD_SHIFT; // True if Shift is pressed
        if (globalRubiksCubePtr) { // Ensure the Rubik's Cube exists
            if (key == GLFW_KEY_R) {
                globalRubiksCubePtr->turn("R", clockwise);
            } else if (key == GLFW_KEY_L) {
                globalRubiksCubePtr->turn("L", clockwise);
            } else if (key == GLFW_KEY_U) {
                globalRubiksCubePtr->turn("U", clockwise);
            } else if (key == GLFW_KEY_D) {
                globalRubiksCubePtr->turn("D", clockwise);
            } else if (key == GLFW_KEY_F) {
                globalRubiksCubePtr->turn("F", clockwise);
            } else if (key == GLFW_KEY_B) {
                globalRubiksCubePtr->turn("B", clockwise);
            }
        }
    }
}




int main() {
    // 1. Initialize GLFW
    if (!glfwInit()) {
        std::cout << "GLFW Initialization failed!" << std::endl;
        glfwTerminate();
        return 1;
    }

    // 2. Set OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required for macOS, good practice

    // 3. Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Rubiks Cube Solver", nullptr, nullptr);
    if (!window) {
        std::cout << "GLFW Window creation failed!" << std::endl;
        glfwTerminate();
        return 1;
    }

    // 4. Make the window's context current
    glfwMakeContextCurrent(window);

    // 5. Initialize GLEW
    glewExperimental = GL_TRUE; // Required for core profile
    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW Initialization failed!" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    // 6. Set the viewport
    glViewport(0, 0, WIDTH, HEIGHT);
   
    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Register mouse callbacks
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback); // Register the key callback



    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); // Typically cull back faces for solid objects
    glFrontFace(GL_CCW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    Shader shader("../shaders/shader.vert", "../shaders/shader.frag");

    CubePiece::initSharedResources(); // Initialize shared resources once

    RubiksCube rubiksCube;
    rubiksCube.setupMesh(); // This will call setupMesh for each CubePiece
    globalRubiksCubePtr = &rubiksCube; // Assign address to global pointer
    
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

    // Removed the initial automatic turn, it will now be triggered by key presses.
    float lastFrameTime = 0.0f;

    // 7. Main loop
    while (!glfwWindowShouldClose(window)) {
        // Poll for and process events
        glfwPollEvents();

        float currentFrameTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;
        // Clear the color buffer
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Darker background
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear depth buffer as well

        shader.use(); // Activate the shader

        // Calculate camera position based on yaw, pitch, and distance (orbiting around origin)
        glm::vec3 cameraPos;
        cameraPos.x = cameraDistance * cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
        cameraPos.y = cameraDistance * sin(glm::radians(cameraPitch));
        cameraPos.z = cameraDistance * sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));

        // Update view matrix
        glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));


        glm::mat4 model = glm::mat4(1.0f); // Start with identity matrix

        // Send matrices to the shader
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setMat4("model", model); // This model matrix applies to the whole Rubik's Cube

        rubiksCube.update(deltaTime); // Update animation state

        rubiksCube.draw(shader);

        // Swap front and back buffers
        glfwSwapBuffers(window);
    }

    CubePiece::cleanupSharedResources(); // Cleanup shared resources

    // 8. Terminate GLFW
    glfwTerminate();
    return 0;
}