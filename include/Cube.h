#pragma once

#include "Shader.h"
#include <GL/glew.h>



class Cube
{
private:
    GLuint VAO, VBO, EBO_faces, EBO_edges;
public:
    Cube();
    ~Cube();


    void setupMesh();
    void draw(Shader& shader);
    void drawEdges(Shader& shader);
};
