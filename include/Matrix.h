#pragma once

#include <vector>
#include <functional>

std::vector<std::vector<float>> matrix_multiply(
    const std::vector<std::vector<float>>& A,
    const std::vector<std::vector<float>>& B);

std::vector<std::vector<float>> matrix_add(
    const std::vector<std::vector<float>>& A,
    const std::vector<std::vector<float>>& B);

std::vector<std::vector<float>> matrix_apply(
    const std::vector<std::vector<float>>& A,
    const std::function<float(float)>& func);

std::vector<std::vector<float>> matrix_transpose(const std::vector<std::vector<float>>& matrix);


void print_matrix(const std::vector<std::vector<float>>& matrix);