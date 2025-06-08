#include <iostream>
#include <vector>
#include <stdexcept> // For std::invalid_argument
#include <functional> // For std::function


// Function to perform matrix multiplication
std::vector<std::vector<float>> matrix_multiply(
    const std::vector<std::vector<float>>& A,
    const std::vector<std::vector<float>>& B) {

    if (A.empty() || B.empty() || A[0].empty() || B[0].empty()) {
        throw std::invalid_argument("Input matrices cannot be empty.");
    }

    size_t rowsA = A.size();
    size_t colsA = A[0].size();
    size_t rowsB = B.size();
    size_t colsB = B[0].size();

    if (colsA != rowsB) {
        throw std::invalid_argument("Number of columns in matrix A must equal number of rows in matrix B.");
    }

    // Initialize the result matrix C with dimensions rowsA x colsB, filled with 0.0
    std::vector<std::vector<float>> C(rowsA, std::vector<float>(colsB, 0.0f));

    for (size_t i = 0; i < rowsA; ++i) {        // Iterate over rows of A
        for (size_t j = 0; j < colsB; ++j) {    // Iterate over columns of B
            for (size_t k = 0; k < colsA; ++k) { // Or k < rowsB, since colsA == rowsB
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

// Function to perform matrix addition

std::vector<std::vector<float>> matrix_add(
    const std::vector<std::vector<float>>& A,
    const std::vector<std::vector<float>>& B) {

    if (A.empty() || B.empty() || A[0].empty() || B[0].empty()) {
        throw std::invalid_argument("Input matrices cannot be empty.");
    }

    size_t rowsA = A.size();
    size_t colsA = A[0].size();
    size_t rowsB = B.size();
    size_t colsB = B[0].size();

    if (colsA != colsB || rowsA != rowsB) {
        throw std::invalid_argument("Matrix should be the same sizes");
    }

    // Initialize the result matrix C with dimensions rowsA x colsB, filled with 0.0
    std::vector<std::vector<float>> C(rowsA, std::vector<float>(colsA, 0.0f));

    for (size_t i = 0; i < rowsA; ++i) {        // Iterate over rows of A
        for (size_t j = 0; j < colsA; ++j) {    // Iterate over columns of B
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    return C;
}


std::vector<std::vector<float>> matrix_apply(
    const std::vector<std::vector<float>>& A,
    const std::function<float(float)>& func){

        if (A.empty() || A[0].empty()) {
        throw std::invalid_argument("Input matrice cannot be empty.");
        }

        size_t rowsA = A.size();
        size_t colsA = A[0].size();

        std::vector<std::vector<float>> B(rowsA, std::vector<float>(colsA, 0.0f));
        for(int i = 0; i < A.size(); i++){
            for(int j = 0; j < A[0].size(); j++){
                B[i][j] = func(A[i][j]);
            }
        }
        return B;
    }



// Helper function to print a matrix
void print_matrix(const std::vector<std::vector<float>>& matrix) {
    if (matrix.empty()) {
        std::cout << "Matrix is empty." << std::endl;
        return;
    }
    for (const auto& row : matrix) {
        for (float val : row) {
            std::cout << val << "\t";
        }
        std::cout << std::endl;
    }
}


// Example: Add to Matrix.cpp (and declare in Matrix.h)
std::vector<std::vector<float>> matrix_transpose(const std::vector<std::vector<float>>& matrix) {
    if (matrix.empty() || matrix[0].empty()) {
        return {}; // Or throw an error
    }
    size_t rows = matrix.size();
    size_t cols = matrix[0].size();

    std::vector<std::vector<float>> transposed_matrix(cols, std::vector<float>(rows));

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            transposed_matrix[j][i] = matrix[i][j];
        }
    }
    return transposed_matrix;
}

