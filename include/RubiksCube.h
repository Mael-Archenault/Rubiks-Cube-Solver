#pragma once

#include <vector>
#include <string>
#include "Shader.h"
#include "CubePiece.h"



class RubiksCube
{
private:

    // "sides" will contain the colors of each face
    // Each element of "sides" is a matrix 3x3 containing ids corresponding to colors.
    // 0: Blue
    // 1: Green
    // 2: Red
    // 3: Orange
    // 4: Blue
    // 5: White

    std::vector<std::vector<int>> sides;
    std::vector<std::vector<std::vector<CubePiece>>> cubePieces;

    // Animation state
    bool isAnimating;
    float currentAnimationAngleRad; // Current angle in radians
    float frameAddedAngle;
    float targetAnimationAngleRad;  // Target angle in radians (pi/2)
    float animationSpeedRad;        // Radians per second
    glm::vec3 animationWorldAxis;   // Axis of rotation in world space
    int animatingLayerIndex;        // 0, 1, or 2
    char animatingFacePlane;        // 'X', 'Y', or 'Z' (plane normal to rotation axis)
    bool animationIsClockwise;      // Direction of logical turn

public:
    RubiksCube();
    ~RubiksCube();
    void setupMesh();
    void draw(Shader& shader);
    void turn(std::string moveName, bool clockwise); // Changed int to bool
    void update(float deltaTime); // For animation
};
