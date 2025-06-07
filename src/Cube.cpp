#include "Cube.h"
#include <vector>


// Define 8 unique vertices for the cube, with positions and colors
const float cubeVertices[] = {
    // Positions          // Colors
    // Front face (Red)
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, // Bottom-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, // Bottom-right
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, // Top-right
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, // Top-left
    // Back face (Blue)
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, // Bottom-left
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, // Bottom-right
     0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, // Top-right
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, // Top-left
    // Top face (Green)
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, // Front-top-left (from front view)
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, // Front-top-right
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, // Back-top-right
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, // Back-top-left
    // Bottom face (Yellow)
    -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f, // Front-bottom-left
     0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f, // Front-bottom-right
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, // Back-bottom-right
    -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, // Back-bottom-left
    // Right face (Cyan)
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // Front-bottom-right
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // Front-top-right
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f, // Back-top-right
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, // Back-bottom-right
    // Left face (Magenta)
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, // Front-bottom-left
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, // Front-top-left
    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f, // Back-top-left
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f  // Back-bottom-left
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


Cube::Cube() : VAO(0), VBO(0), EBO_faces(0), EBO_edges(0) {
}

Cube::~Cube() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO_faces);
    glDeleteBuffers(1, &EBO_edges);
}


void Cube::setupMesh(){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO_faces);
    glGenBuffers(1, &EBO_edges);

    glBindVertexArray(VAO);

    // VBO for vertex data (positions and colors)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    // EBO for face indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_faces);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // EBO for edge indices
    // Note: VAO remembers the EBO binding for glDrawElements.
    // We will bind the correct EBO before each draw call.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_edges);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(edgeIndices), edgeIndices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Cube::draw(Shader& shader){
    shader.use();
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_faces); // Bind EBO for faces
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Cube::drawEdges(Shader& shader){
    shader.use();
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_edges); // Bind EBO for edges
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0); // 12 edges * 2 vertices per edge = 24 indices
    glBindVertexArray(0);
}