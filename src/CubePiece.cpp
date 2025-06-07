#include "CubePiece.h"
#include <vector>
#include <glm/glm.hpp>


// Define 8 unique vertices for the cube, with positions and colors
const float cubeVertices[] = {
    // Positions          // Colors
    // Front face (Red)
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, // Bottom-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, // Bottom-right
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, // Top-right
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, // Top-left
    // Back face (Black, was Orange)
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, // Bottom-left
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, // Bottom-right
     0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, // Top-right
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, // Top-left
    // Top face (White)
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, // Front-top-left (from front view)
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, // Front-top-right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, // Back-top-right
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, // Back-top-left
    // Bottom face (Black, was Yellow)
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, // Front-bottom-left
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, // Front-bottom-right
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, // Back-bottom-right
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, // Back-bottom-left
    // Right face (Blue)
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, // Front-bottom-right
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, // Front-top-right
     0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, // Back-top-right
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, // Back-bottom-right
    // Left face (Black, was Green)
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, // Front-bottom-left
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, // Front-top-left
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, // Back-top-left
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f  // Back-bottom-left
};


// Define indices for the 12 triangles (faces) of the cube
const unsigned int indices[] = {
    // Front face
    0, 1, 2,
    2, 3, 0,
    // Back face
    4, 6, 5, 
    6, 4, 7, 
    // Right face
    16, 18, 17,
    18, 16, 19,
    // Left face
    20, 21, 22,
    22, 23, 20,
    // Top face
    8, 9, 10,
    10, 11, 8,
    // Bottom face
    12, 14, 13,
    14, 12, 15
};

// Define indices for the 12 edges of the cube
const unsigned int edgeIndices[] = {
    0, 1, 1, 2, 2, 3, 3, 0, // Front face edges
    4, 5, 5, 6, 6, 7, 7, 4, // Back face edges
    0, 4, 1, 5, 2, 6, 3, 7  // Connecting edges
};

// Initialize static members
GLuint CubePiece::s_VAO = 0;
GLuint CubePiece::s_VBO = 0;
GLuint CubePiece::s_EBO_faces = 0;
GLuint CubePiece::s_EBO_edges = 0;
bool CubePiece::s_resourcesInitialized = false;

// CubePiece::CubePiece() : rotation(glm::vec3(0.0f)){
CubePiece::CubePiece() : orientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)) { // Initialize with identity quaternion

};

CubePiece::~CubePiece() {
    // Destructor should not delete shared static resources.
    // Cleanup is handled by cleanupSharedResources().
}

void CubePiece::initSharedResources() {
    if (s_resourcesInitialized) {
        return;
    }

    glGenVertexArrays(1, &s_VAO);
    glGenBuffers(1, &s_VBO);
    glGenBuffers(1, &s_EBO_faces);
    glGenBuffers(1, &s_EBO_edges);

    glBindVertexArray(s_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, s_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_EBO_faces);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_EBO_edges);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(edgeIndices), edgeIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    s_resourcesInitialized = true;
}

void CubePiece::cleanupSharedResources() {
    if (!s_resourcesInitialized) return;
    glDeleteVertexArrays(1, &s_VAO);
    glDeleteBuffers(1, &s_VBO);
    glDeleteBuffers(1, &s_EBO_faces);
    glDeleteBuffers(1, &s_EBO_edges);
    s_VAO = 0; s_VBO = 0; s_EBO_faces = 0; s_EBO_edges = 0;
    s_resourcesInitialized = false;
}

void CubePiece::setupMesh(){
    // Ensure shared resources are initialized
    initSharedResources();
    // The individual VAO, VBO members are no longer used for drawing.
}

void CubePiece::draw(Shader& shader){
    if (!s_resourcesInitialized) return;
    shader.use();
    glBindVertexArray(s_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_EBO_faces); // Bind EBO for faces
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void CubePiece::drawEdges(Shader& shader){
    if (!s_resourcesInitialized) return;
    shader.use();
    glBindVertexArray(s_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_EBO_edges); // Bind EBO for edges
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0); // 12 edges * 2 vertices per edge = 24 indices
    glBindVertexArray(0);
}


glm::mat4 CubePiece::get_model(){
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::mat4_cast(orientation) * model; // Apply orientation
    return model;
}

// void CubePiece::update_rotation(glm::vec3 new_angle){
void CubePiece::update_orientation(glm::quat rotation_quat){
    // Multiply by the rotation quaternion to combine rotations.
    // The order matters: new_orientation = rotation_to_apply * current_orientation
    orientation = rotation_quat * orientation;
    orientation = glm::normalize(orientation); // Keep the quaternion normalized
}