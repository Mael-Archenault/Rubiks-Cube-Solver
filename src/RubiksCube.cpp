#include "RubiksCube.h"
#include "CubePiece.h"
#include <string>
#include <glm/gtc/matrix_transform.hpp>

RubiksCube::RubiksCube() {
    // Initialize cubePieces as a 3x3x3 grid
    cubePieces.resize(3); // X dimension
    for (int i = 0; i < 3; ++i) {
        cubePieces[i].resize(3); // Y dimension
        for (int j = 0; j < 3; ++j) {
            cubePieces[i][j].resize(3); // Z dimension
        }
    }

    isAnimating = false;
    currentAnimationAngleRad = 0.0f;
    frameAddedAngle = 0.0f;
    targetAnimationAngleRad = glm::radians(90.0f);
    animationSpeedRad = glm::radians(180.0f); 
    animationWorldAxis = glm::vec3(0.0f);
    animatingLayerIndex = -1;
    animatingFacePlane = ' ';
    animationIsClockwise = true;
}

RubiksCube::~RubiksCube() {
}

void RubiksCube::setupMesh() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                cubePieces[i][j][k].setupMesh(); 
            }
        }
    }

}

void RubiksCube::draw(Shader& shader) {
    float pieceSize = 1.0f; 
    float spacing = 0.01f;
    float totalSize = pieceSize + spacing;


    for (int i = 0; i < 3; ++i) { // X-axis
        for (int j = 0; j < 3; ++j) { // Y-axis
            for (int k = 0; k < 3; ++k) { // Z-axis
                // Calculate the position of the current piece
                float xPos = (i - 1) * totalSize;
                float yPos = (j - 1) * totalSize;
                float zPos = (k - 1) * totalSize;

                glm::mat4 pieceModel = cubePieces[i][j][k].get_model();
                glm::mat4 basePieceModel = glm::translate(glm::mat4(1.0f), glm::vec3(xPos, yPos, zPos)) * pieceModel;
                glm::mat4 finalModel = basePieceModel;

                if (isAnimating) {
                    bool pieceIsInAnimatingLayer = false;
                    if (animatingFacePlane == 'X' && i == animatingLayerIndex) pieceIsInAnimatingLayer = true;
                    else if (animatingFacePlane == 'Y' && j == animatingLayerIndex) pieceIsInAnimatingLayer = true;
                    else if (animatingFacePlane == 'Z' && k == animatingLayerIndex) pieceIsInAnimatingLayer = true;

                    if (pieceIsInAnimatingLayer) {
                        float visualAngle = animationIsClockwise ? currentAnimationAngleRad : -currentAnimationAngleRad;

                        glm::mat4 animationRotation = glm::rotate(glm::mat4(1.0f), visualAngle, animationWorldAxis);
                        finalModel = animationRotation * basePieceModel;
                    }
                }

                shader.setMat4("model", finalModel);
                cubePieces[i][j][k].draw(shader);
            }
        }
    }
}


void rotate3x3Matrix(std::vector<std::vector<CubePiece>>& matrix, bool clockwise) {

    std::vector<std::vector<CubePiece>> original = matrix; // Create a copy of the original matrix
    
    if (clockwise) {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                matrix[i][j] = original[2 - j][i];
            }
        }
    } else {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                matrix[i][j] = original[j][2 - i];
            }
        }
    }
}



void RubiksCube::turn(std::string moveName, bool clockwise){
    if (isAnimating) return; // Don't start a new turn if one is in progress

    isAnimating = true;
    currentAnimationAngleRad = 0.0f;
    animationIsClockwise = clockwise;

    if (moveName == "R") {
        animatingFacePlane = 'X'; animatingLayerIndex = 2; animationWorldAxis = glm::vec3(1.0f, 0.0f, 0.0f);
    } else if (moveName == "L") {
        animatingFacePlane = 'X'; animatingLayerIndex = 0; animationWorldAxis = glm::vec3(-1.0f, 0.0f, 0.0f);
    } else if (moveName == "U") {
        animatingFacePlane = 'Y'; animatingLayerIndex = 2; animationWorldAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    } else if (moveName == "D") {
        animatingFacePlane = 'Y'; animatingLayerIndex = 0; animationWorldAxis = glm::vec3(0.0f, -1.0f, 0.0f);
    } else if (moveName == "F") {
        animatingFacePlane = 'Z'; animatingLayerIndex = 2; animationWorldAxis = glm::vec3(0.0f, 0.0f, 1.0f);
    } else if (moveName == "B") {
        animatingFacePlane = 'Z'; animatingLayerIndex = 0; animationWorldAxis = glm::vec3(0.0f, 0.0f, -1.0f);
    } else {
        isAnimating = false; // Invalid move name
        return;
    }
}

void RubiksCube::update(float deltaTime) {
    if (!isAnimating) return;

    currentAnimationAngleRad += animationSpeedRad * deltaTime;
    frameAddedAngle = animationSpeedRad * deltaTime;


    if (currentAnimationAngleRad >= targetAnimationAngleRad) {
        currentAnimationAngleRad = 0.0f; // Reset for next animation
        isAnimating = false;

        // --- Perform logical state update ---
        // 1. Update internal orientation of each piece in the turned layer
        float logicalAngleRad = animationIsClockwise ? targetAnimationAngleRad : -targetAnimationAngleRad;
        
        for (int p1 = 0; p1 < 3; ++p1) {
            for (int p2 = 0; p2 < 3; ++p2) {
                CubePiece* piece_ptr = nullptr;
                if (animatingFacePlane == 'X') piece_ptr = &cubePieces[animatingLayerIndex][p1][p2];
                else if (animatingFacePlane == 'Y') piece_ptr = &cubePieces[p1][animatingLayerIndex][p2];
                else if (animatingFacePlane == 'Z') piece_ptr = &cubePieces[p1][p2][animatingLayerIndex];

                if (piece_ptr) {
                    glm::quat turnQuat = glm::angleAxis(logicalAngleRad, animationWorldAxis);
                    piece_ptr->update_orientation(turnQuat);
                }
            }
        }

        std::vector<std::vector<CubePiece>> slice(3, std::vector<CubePiece>(3));
        if (animatingFacePlane == 'X') {
            bool direction = (animatingLayerIndex==0)?animationIsClockwise:!animationIsClockwise;
            for (int r = 0; r < 3; ++r) for (int c = 0; c < 3; ++c) slice[r][c] = cubePieces[animatingLayerIndex][r][c];
            rotate3x3Matrix(slice, direction);
            for (int r = 0; r < 3; ++r) for (int c = 0; c < 3; ++c) cubePieces[animatingLayerIndex][r][c] = slice[r][c];
        } else if (animatingFacePlane == 'Y') {
            bool direction = (animatingLayerIndex==0)?!animationIsClockwise:animationIsClockwise;
            for (int r = 0; r < 3; ++r) for (int c = 0; c < 3; ++c) slice[r][c] = cubePieces[r][animatingLayerIndex][c]; // Correct indexing for Y-slice
            rotate3x3Matrix(slice, direction);
            for (int r = 0; r < 3; ++r) for (int c = 0; c < 3; ++c) cubePieces[r][animatingLayerIndex][c] = slice[r][c];
        } else if (animatingFacePlane == 'Z') {
            bool direction = (animatingLayerIndex==0)?animationIsClockwise:!animationIsClockwise;
            for (int r = 0; r < 3; ++r) for (int c = 0; c < 3; ++c) slice[r][c] = cubePieces[r][c][animatingLayerIndex]; // Correct indexing for Z-slice
            rotate3x3Matrix(slice, direction);
            for (int r = 0; r < 3; ++r) for (int c = 0; c < 3; ++c) cubePieces[r][c][animatingLayerIndex] = slice[r][c];
        }
        
    } 
}