#pragma once

#include "Shader.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <array>
#include <string>



class CubePiece
{
private:
    // Static members for shared OpenGL resources
    static GLuint s_VAO, s_VBO, s_EBO_faces, s_EBO_edges;
    static bool s_resourcesInitialized;
    std::array<bool, 6> faceVisibility;

public:
    glm::quat orientation; // Stores the piece's current orientation
    CubePiece();
    ~CubePiece(); 
    
    // Static methods to manage shared resources
    static void initSharedResources();
    static void cleanupSharedResources();
    
    void setupMesh();
    void draw(Shader& shader);
    void drawEdges(Shader& shader);
    
    glm::mat4 get_model();
    void update_orientation(glm::quat rotation_quat);
    void set_face_visibilities(int i, int j, int k);

};
