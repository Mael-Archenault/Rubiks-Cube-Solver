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
            // Each element cubePieces[i][j][k] is now a default-constructed CubePiece
        }
    }

    isAnimating = false;
    currentAnimationAngleRad = 0.0f;
    frameAddedAngle = 0.0f;
    targetAnimationAngleRad = glm::radians(90.0f);
    animationSpeedRad = glm::radians(180.0f); // 180 degrees per second
    animationWorldAxis = glm::vec3(0.0f);
    animatingLayerIndex = -1;
    animatingFacePlane = ' ';
    animationIsClockwise = true;
}

RubiksCube::~RubiksCube() {
    // Vectors will handle their own memory deallocation
}

void RubiksCube::setupMesh() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                cubePieces[i][j][k].setupMesh(); // Call if CubePiece has individual setup
                std::ostringstream oss;
                oss << i << "_" << j << "_" << k;
                cubePieces[i][j][k].name = oss.str();
            }
        }
    }

}

void RubiksCube::draw(Shader& shader) {
    // Iterate and draw each piece, applying appropriate model transformations
    float pieceSize = 1.0f; // Assuming each CubePiece is a 1x1x1 unit cube
    float spacing = 0.01f;   // A small gap between pieces, adjust as needed
    float totalSize = pieceSize + spacing;

    // The Rubik's Cube is 3 pieces wide/high/deep.
    // The center of the Rubik's Cube will be at (0,0,0) in its local space.
    // So, pieces range from -totalSize to +totalSize from the center.
    // (e.g., for i=0, offset is -totalSize; for i=1, offset is 0; for i=2, offset is +totalSize)

    for (int i = 0; i < 3; ++i) { // X-axis
        for (int j = 0; j < 3; ++j) { // Y-axis
            for (int k = 0; k < 3; ++k) { // Z-axis
                // Calculate the position of the current piece
                float xPos = (i - 1) * totalSize; // (0-1)*ts = -ts, (1-1)*ts = 0, (2-1)*ts = ts
                float yPos = (j - 1) * totalSize;
                float zPos = (k - 1) * totalSize;
                glm::mat4 pieceModel = cubePieces[i][j][k].get_model();

                
                
                // Base model: piece's internal orientation + its position in the grid
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
                        //finalModel = glm::translate(glm::mat4(1.0f), glm::vec3(xPos, yPos, zPos)) *cubePieces[i][j][k].get_model();
                        //cubePieces[i][j][k].update_rotation(frameAddedAngle *animationWorldAxis);
                    }
                }

                // Example: Make the piece at (1,1,2) darker. Adjust indices as needed.
                // This assumes (1,1,2) is a valid piece you want to target.
                if (cubePieces[i][j][k].name == "2_2_2") { // Target the piece at specific indices
                    shader.setBool("u_isDarkPiece", true);
                    shader.setFloat("u_darkeningFactor", 0.5f); // Make it 50% darker
                } else {
                    shader.setBool("u_isDarkPiece", false);
                }
                shader.setMat4("model", finalModel);
                cubePieces[i][j][k].draw(shader);
            }
        }
    }
}


// This function should only shuffle pieces. Orientation update is separate.
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
    // targetAnimationAngleRad is already glm::radians(90.0f)
    animationIsClockwise = clockwise;

    // Determine animation axis and layer based on moveName
    // Standard Rubik's Cube Notation:
    // R: Right face (X=2 slice), rotate around positive X-axis
    // L: Left face (X=0 slice), rotate around negative X-axis
    // U: Up face (Y=2 slice), rotate around positive Y-axis
    // D: Down face (Y=0 slice), rotate around negative Y-axis
    // F: Front face (Z=2 slice), rotate around positive Z-axis
    // B: Back face (Z=0 slice), rotate around negative Z-axis
    // (Assuming cube indices [i][j][k] are [x][y][z] from 0 to 2,
    // and +X is right, +Y is up, +Z is front)

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
                    // This is where the piece's internal orientation (its 'rotation' member)
                    // needs to be updated by composing its current orientation with the 90-degree turn.
                    // This is complex with Euler angles. Using quaternions in CubePiece is highly recommended.
                    // For now, this step is simplified/omitted for brevity as it's a separate math problem.
                    // Example if CubePiece::rotation was a quaternion:
                    glm::quat turnQuat = glm::angleAxis(logicalAngleRad, animationWorldAxis);
                    piece_ptr->orientation = turnQuat * piece_ptr->orientation; // This was one way to do it directly
                    if (piece_ptr->name == "test") std::cout<<"Test : "<<piece_ptr->orientation.x<<" "<<piece_ptr->orientation.y<<" "<<piece_ptr->orientation.z<<std::endl;
                    //piece_ptr->update_orientation(turnQuat); // Use the new method
                }
            }
        }

        // 2. Reorder (shuffle) CubePiece objects in the cubePieces array
        std::vector<std::vector<CubePiece>> slice(3, std::vector<CubePiece>(3));
        if (animatingFacePlane == 'X') {
            bool direction = (animatingLayerIndex==0)?animationIsClockwise:!animationIsClockwise;
            for (int r = 0; r < 3; ++r) for (int c = 0; c < 3; ++c) slice[r][c] = cubePieces[animatingLayerIndex][r][c];
            rotate3x3Matrix(slice, direction);
            // The update_orientation is now handled above for each piece in the animating layer
            for (int r = 0; r < 3; ++r) for (int c = 0; c < 3; ++c) cubePieces[animatingLayerIndex][r][c] = slice[r][c];
        } else if (animatingFacePlane == 'Y') {
            bool direction = (animatingLayerIndex==0)?!animationIsClockwise:animationIsClockwise;
            for (int r = 0; r < 3; ++r) for (int c = 0; c < 3; ++c) slice[r][c] = cubePieces[r][animatingLayerIndex][c]; // Correct indexing for Y-slice
            rotate3x3Matrix(slice, direction);
            // The update_orientation is now handled above
            for (int r = 0; r < 3; ++r) for (int c = 0; c < 3; ++c) cubePieces[r][animatingLayerIndex][c] = slice[r][c];
        } else if (animatingFacePlane == 'Z') {
            bool direction = (animatingLayerIndex==0)?animationIsClockwise:!animationIsClockwise;
            for (int r = 0; r < 3; ++r) for (int c = 0; c < 3; ++c) slice[r][c] = cubePieces[r][c][animatingLayerIndex]; // Correct indexing for Z-slice
            rotate3x3Matrix(slice, direction);
            // The update_orientation is now handled above
            for (int r = 0; r < 3; ++r) for (int c = 0; c < 3; ++c) cubePieces[r][c][animatingLayerIndex] = slice[r][c];
        }


        std::cout<<"/////////////////////"<<std::endl;
        for(int i = 0; i < 3; ++i){
            for(int j = 0; j < 3; ++j){
                std::cout<<"("<<cubePieces[i][j][0].name<<", "<<cubePieces[i][j][1].name<<", "<<cubePieces[i][j][2].name<<") "<<std::endl;
            }
            std::cout<<std::endl;
        } 
        
    } 
}