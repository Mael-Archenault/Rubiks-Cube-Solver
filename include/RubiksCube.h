#pragma once

#include <vector>
#include <string>
#include <array> // Required for std::array
#include "Shader.h"
#include "CubePiece.h"



// Define face indices for clarity, matching EBO and shader logic
enum class CubeFace {
    FRONT_RED = 0,     // Corresponds to faceVisibility[0]
    BACK_ORANGE = 1,   // Corresponds to faceVisibility[1]
    RIGHT_BLUE = 2,    // Corresponds to faceVisibility[2]
    LEFT_GREEN = 3,    // Corresponds to faceVisibility[3]
    TOP_WHITE = 4,     // Corresponds to faceVisibility[4]
    BOTTOM_YELLOW = 5  // Corresponds to faceVisibility[5]
};
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
    std::array<bool, 6> faceVisibility; // Stores visibility for Front, Back, Right, Left, Top, Bottom

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
    void init();
    void draw(Shader& shader);
    void turn(std::string moveName, bool clockwise); // Changed int to bool
    void update(float deltaTime); // For animation
    bool is_solved();
    

};
