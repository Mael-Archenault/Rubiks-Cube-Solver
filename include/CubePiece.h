#pragma once

#include "Shader.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>



class CubePiece
{
private:
    // Static members for shared OpenGL resources
    static GLuint s_VAO, s_VBO, s_EBO_faces, s_EBO_edges;
    static bool s_resourcesInitialized;

public:
    // glm::vec3 rotation; // Replaced by quaternion
    glm::quat orientation; // Stores the piece's current orientation
    CubePiece();
    ~CubePiece(); // Destructor will no longer delete shared resources
    
    // Static methods to manage shared resources
    static void initSharedResources();
    static void cleanupSharedResources();
    
    void setupMesh();
    void draw(Shader& shader);
    void drawEdges(Shader& shader);
    
    glm::mat4 get_model();
    // void update_rotation(glm::vec3 new_angle); // Replaced by update_orientation
    void update_orientation(glm::quat rotation_quat);
    std::string name;

};
