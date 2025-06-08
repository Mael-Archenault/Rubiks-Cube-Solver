#include "Linear.h"
#include <iostream>
#include <vector>    // Though already in Linear.h, good for clarity if specific vector ops were used here
#include <string>    // For std::to_string and std::string operations
#include <algorithm> // For std::max
#include <iomanip>   // For std::setw, std::fixed, std::setprecision
#include "Matrix.h"
#include <random>



Linear::Linear(int n_input, int n_output, int n_hidden_layer, int n_hidden_layer_node, bool randomize, activations activation_function){

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1.0, 1.0);

    // Initialize sizes
    this->n_input = n_input;
    this->n_output = n_output;
    this->n_hidden_layer = n_hidden_layer;
    this->n_hidden_layer_node = n_hidden_layer_node;


    // Initialize node values

    this->input.resize(n_input);
    for(int i = 0; i < n_input; i++){
        this->input[i].resize(1, 0.0f);
    }
    
    this->hidden_layer.resize(n_hidden_layer);
    for(int i = 0; i < n_hidden_layer; i++){
        this->hidden_layer[i].resize(n_hidden_layer_node);
        
        for(int j = 0; j < n_hidden_layer_node; j++){
            this->hidden_layer[i][j].resize(1, 0.0f);
        }
    }
    
    this->output.resize(n_output);
    for(int i = 0; i < n_output; i++){
        this->output[i].resize(1, 0.0f);
    }
    this->desired_output.resize(n_output);
    for(int i = 0; i < n_output; i++){
        this->desired_output[i].resize(1, 0.0f);
    }

    // Initialize pre-activation (z) storage
    this->hidden_layer_z.resize(n_hidden_layer);
    for(int i = 0; i < n_hidden_layer; i++){
        this->hidden_layer_z[i].resize(n_hidden_layer_node);
        for(int j = 0; j < n_hidden_layer_node; j++){
            this->hidden_layer_z[i][j].resize(1, 0.0f);
        }
    }

    this->output_z.resize(n_output);
    for(int i = 0; i < n_output; i++){
        this->output_z[i].resize(1, 0.0f);
    }

    // Initialize errors

    this->hidden_layer_error.resize(n_hidden_layer);
    for(int i = 0; i < n_hidden_layer; i++){
        this->hidden_layer_error[i].resize(n_hidden_layer_node);
        
        for(int j = 0; j < n_hidden_layer_node; j++){
            this->hidden_layer_error[i][j].resize(1, 0.0f);
        }
    }
    
    this->output_error.resize(n_output);
    for(int i = 0; i < n_output; i++){
        this->output_error[i].resize(1, 0.0f);
    }

    // Initialize weights

    this->input_weights.resize(n_hidden_layer_node);
    for(int i = 0; i < n_hidden_layer_node; i++){
        this->input_weights[i].resize(n_input);
        for(int j = 0; j < n_input; j++){
            this->input_weights[i][j] = randomize? dis(gen) : 0.0f;
        }
    }

    this->hidden_layer_weights.resize(n_hidden_layer - 1);
    for(int i = 0; i < n_hidden_layer - 1; i++){
        this->hidden_layer_weights[i].resize(n_hidden_layer_node);
        for(int j = 0; j < n_hidden_layer_node; j++){
            this->hidden_layer_weights[i][j].resize(n_hidden_layer_node);
            for(int k = 0; k < n_hidden_layer_node; k++){
                this->hidden_layer_weights[i][j][k] = randomize? dis(gen) : 0.0f;
            }
        }
    }

    this->output_weights.resize(n_output);
    for(int i = 0; i < n_output; i++){
        this->output_weights[i].resize(n_hidden_layer_node);
        for(int j = 0; j < n_hidden_layer_node; j++){
            this->output_weights[i][j] = randomize? dis(gen) : 0.0f;
        }
    }

    // Initialize biases

    this->hidden_layer_bias.resize(n_hidden_layer);
    for(int i = 0; i < n_hidden_layer; i++){
        this->hidden_layer_bias[i].resize(n_hidden_layer_node);
        for(int j = 0; j < n_hidden_layer_node; j++){

            this->hidden_layer_bias[i][j].resize(1,  randomize? dis(gen) : 0.0f);
        }
    }

    this->output_bias.resize(n_output);
    for(int i = 0; i < n_output; i++){
        this->output_bias[i].resize(1,  randomize? dis(gen) : 0.0f);
    }

    // Initialize activation function

    switch(activation_function){
        case sigmoid:
            this->activation_function = [](float x) {
                return 1.0f / (1.0f + std::exp(-x));
            };
            this->derivative_activation_function = [](float x) {
                float sigmoid_x = 1.0f / (1.0f + std::exp(-x));
                return sigmoid_x * (1.0f - sigmoid_x);
            };
            break;
        case relu:
            this->activation_function = [](float x) {
                return std::max(0.0f, x);
            };
            this->derivative_activation_function = [](float x) {
                return (x > 0.0f) ? 1.0f : 0.0f;
            };
            break;
        // default: // Consider adding a default or error for unhandled enum values
    }
}

Linear::~Linear(){

}




void Linear::print_node_values(){
    std::cout << "||||||Linear Node Values||||||" << std::endl << std::endl;

    // Determine the maximum number of nodes in any single layer part
    // (input layer, any single hidden layer, or output layer)
    int max_nodes_per_layer = n_input;
    max_nodes_per_layer = std::max(max_nodes_per_layer, n_output);
    if (n_hidden_layer > 0) {
        max_nodes_per_layer = std::max(max_nodes_per_layer, n_hidden_layer_node);
    }

    if (max_nodes_per_layer == 0) {
        std::cout << "Network has no nodes to display in this format." << std::endl;
        return;
    }

    const int col_width = 10; // Width for data columns
    const int idx_col_width = 5;  // Width for Node index column

    // Print header
    std::cout << std::left; // Align text to the left for headers
    std::cout << std::setw(idx_col_width) << "Node" << " | ";
    std::cout << std::setw(col_width) << "Input" << " | ";
    for (int i = 0; i < n_hidden_layer; ++i) {
        std::cout << std::setw(col_width) << ("H" + std::to_string(i)) << " | ";
    }
    std::cout << std::setw(col_width) << "Output" << std::endl;
    std::cout << std::right; // Reset to default (right alignment for numbers)

    // Print separator line
    std::cout << std::string(idx_col_width, '-') << "-+-";
    std::cout << std::string(col_width, '-') << "-+-";
    for (int i = 0; i < n_hidden_layer; ++i) {
        std::cout << std::string(col_width, '-') << "-+-";
    }
    std::cout << std::string(col_width, '-') << std::endl;

    // Print node values row by row
    for (int node_idx = 0; node_idx < max_nodes_per_layer; ++node_idx) {
        std::cout << std::setw(idx_col_width) << node_idx << " | ";

        // Input layer
        if (node_idx < n_input) {
            std::cout << std::setw(col_width) << std::fixed << std::setprecision(4) << input[node_idx][0] << " | ";
        } else {
            std::cout << std::setw(col_width) << "---" << " | ";
        }

        // Hidden layers
        for (int h_layer_idx = 0; h_layer_idx < n_hidden_layer; ++h_layer_idx) {
            if (node_idx < n_hidden_layer_node && h_layer_idx < hidden_layer.size() && node_idx < hidden_layer[h_layer_idx].size()) {
                std::cout << std::setw(col_width) << std::fixed << std::setprecision(4) << hidden_layer[h_layer_idx][node_idx][0] << " | ";
            } else {
                std::cout << std::setw(col_width) << "---" << " | ";
            }
        }

        // Output layer
        if (node_idx < n_output) {
            std::cout << std::setw(col_width) << std::fixed << std::setprecision(4) << output[node_idx][0] << " | ";
        } else {
            std::cout << std::setw(col_width) << "---" << " | ";
        }
        std::cout << std::endl;
    }
    std::cout<<std::endl;

}


void Linear::print_error_values(){
    std::cout << "||||||Linear Error Values||||||" << std::endl << std::endl;

    // Determine the maximum number of nodes in any single layer part
    // (input layer, any single hidden layer, or output layer)
    int max_nodes_per_layer = n_input;
    max_nodes_per_layer = std::max(max_nodes_per_layer, n_output);
    if (n_hidden_layer > 0) {
        max_nodes_per_layer = std::max(max_nodes_per_layer, n_hidden_layer_node);
    }

    if (max_nodes_per_layer == 0) {
        std::cout << "Network has no nodes to display in this format." << std::endl;
        return;
    }

    const int col_width = 10; // Width for data columns
    const int idx_col_width = 5;  // Width for Node index column

    // Print header
    std::cout << std::left; // Align text to the left for headers
    std::cout << std::setw(idx_col_width) << "Node" << " | ";
    std::cout << std::setw(col_width) << "Input" << " | ";
    for (int i = 0; i < n_hidden_layer; ++i) {
        std::cout << std::setw(col_width) << ("H" + std::to_string(i)) << " | ";
    }
    std::cout << std::setw(col_width) << "Output" << std::endl;
    std::cout << std::right; // Reset to default (right alignment for numbers)

    // Print separator line
    std::cout << std::string(idx_col_width, '-') << "-+-";
    std::cout << std::string(col_width, '-') << "-+-";
    for (int i = 0; i < n_hidden_layer; ++i) {
        std::cout << std::string(col_width, '-') << "-+-";
    }
    std::cout << std::string(col_width, '-') << std::endl;

    // Print node values row by row
    for (int node_idx = 0; node_idx < max_nodes_per_layer; ++node_idx) {
        std::cout << std::setw(idx_col_width) << node_idx << " | ";

        // Input layer
        if (node_idx < n_input) {
            std::cout << std::setw(col_width) << std::fixed << std::setprecision(4) << input[node_idx][0] << " | ";
        } else {
            std::cout << std::setw(col_width) << "---" << " | ";
        }

        // Hidden layers
        for (int h_layer_idx = 0; h_layer_idx < n_hidden_layer; ++h_layer_idx) {
            if (node_idx < n_hidden_layer_node && h_layer_idx < hidden_layer_error.size() && node_idx < hidden_layer_error[h_layer_idx].size()) {
                std::cout << std::setw(col_width) << std::fixed << std::setprecision(4) << hidden_layer_error[h_layer_idx][node_idx][0] << " | ";
            } else {
                std::cout << std::setw(col_width) << "---" << " | ";
            }
        }

        // Output layer
        if (node_idx < n_output) {
            std::cout << std::setw(col_width) << std::fixed << std::setprecision(4) << output_error[node_idx][0] << " | ";
        } else {
            std::cout << std::setw(col_width) << "---" << " | ";
        }
        std::cout << std::endl;
    }
    std::cout<<std::endl;

}

// Helper function to print a weight matrix section
void print_single_weight_matrix(
    const std::string& title,
    const std::vector<std::vector<float>>& weights_matrix,
    int num_to_nodes, const std::string& to_prefix,
    int num_from_nodes, const std::string& from_prefix,
    int display_col_width) {

    std::cout << title << std::endl;

    if (num_to_nodes == 0 || num_from_nodes == 0) {
        std::cout << "  (No connections to display for these layers - zero nodes)" << std::endl << std::endl;
        return;
    }

    if (weights_matrix.empty() || 
        (weights_matrix.size() != static_cast<size_t>(num_to_nodes)) ||
        (!weights_matrix.empty() && weights_matrix[0].size() != static_cast<size_t>(num_from_nodes))) {
        std::cout << "  (Weight data is not available or has unexpected dimensions)" << std::endl;
        std::cout << "  Expected To x From: " << num_to_nodes << "x" << num_from_nodes << std::endl;
        std::cout << "  Actual matrix size: " << weights_matrix.size();
        if (!weights_matrix.empty()) std::cout << "x" << weights_matrix[0].size();
        std::cout << std::endl << std::endl;
        // We can still try to print headers if dimensions are expected
    }

    // Header for "From Node"
    // Adjust first column width to fit "To_X |"
    int first_col_label_width = std::max((int)to_prefix.length() + 3, display_col_width); 
    std::cout << std::setw(first_col_label_width) << std::left << "To\\From" << " | ";
    std::cout << std::right; // Reset alignment for numbers

    for (int from_idx = 0; from_idx < num_from_nodes; ++from_idx) {
        std::cout << std::setw(display_col_width) << (from_prefix + std::to_string(from_idx)) << " | ";
    }
    std::cout << std::endl;

    // Separator line
    std::cout << std::string(first_col_label_width, '-') << "-+-";
    for (int from_idx = 0; from_idx < num_from_nodes; ++from_idx) {
        std::cout << std::string(display_col_width, '-') << "-+-";
    }
    std::cout << std::endl;

    for (int to_idx = 0; to_idx < num_to_nodes; ++to_idx) {
        std::cout << std::setw(first_col_label_width) << std::left << (to_prefix + std::to_string(to_idx)) << " | ";
        std::cout << std::right; // Reset alignment for numbers
        for (int from_idx = 0; from_idx < num_from_nodes; ++from_idx) {
            if (static_cast<size_t>(to_idx) < weights_matrix.size() && static_cast<size_t>(from_idx) < weights_matrix[to_idx].size()) {
                std::cout << std::setw(display_col_width) << weights_matrix[to_idx][from_idx] << " | ";
            } else {
                std::cout << std::setw(display_col_width) << "N/A" << " | ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Linear::print_weights(){
    std::cout << "||||||Linear Weights||||||" << std::endl << std::endl;
    const int display_col_width = 8; // Column width for weight values
    std::cout << std::fixed << std::setprecision(4);

    // Case 1: No hidden layers (Input -> Output)
    if (n_hidden_layer == 0) {
        // output_weights dimensions: [n_output][n_input]
        print_single_weight_matrix("Weights: Input Layer -> Output Layer",
                                   output_weights,
                                   n_output, "O",
                                   n_input, "I",
                                   display_col_width);
        return;
    }

    // Case 2: With hidden layers
    // 2a. Input -> First Hidden Layer (H0)
    // input_weights dimensions: [n_hidden_layer_node][n_input]
    print_single_weight_matrix("Weights: Input Layer -> Hidden Layer 0",
                               input_weights,
                               n_hidden_layer_node, "H0",
                               n_input, "I",
                               display_col_width);

    // 2b. Hidden Layer h -> Hidden Layer h+1
    // hidden_layer_weights[h] dimensions: [n_hidden_layer_node][n_hidden_layer_node]
    for (int h_idx = 0; h_idx < n_hidden_layer - 1; ++h_idx) {
        if (static_cast<size_t>(h_idx) < hidden_layer_weights.size()) {
            print_single_weight_matrix("Weights: Hidden Layer " + std::to_string(h_idx) + " -> Hidden Layer " + std::to_string(h_idx + 1),
                                       hidden_layer_weights[h_idx],
                                       n_hidden_layer_node, "H" + std::to_string(h_idx + 1),
                                       n_hidden_layer_node, "H" + std::to_string(h_idx),
                                       display_col_width);
        } else {
            std::cout << "Weights: Hidden Layer " << h_idx << " -> Hidden Layer " << (h_idx + 1) << ": Data not available (hidden_layer_weights not fully sized for this layer)." << std::endl << std::endl;
        }
    }

    // 2c. Last Hidden Layer (H_{n_hidden_layer-1}) -> Output Layer
    // output_weights dimensions: [n_output][n_hidden_layer_node]
    print_single_weight_matrix("Weights: Hidden Layer " + std::to_string(n_hidden_layer - 1) + " -> Output Layer",
                               output_weights,
                               n_output, "O",
                               n_hidden_layer_node, "H" + std::to_string(n_hidden_layer - 1),
                               display_col_width);
}

void Linear::print_biases(){
    std::cout << "||||||Linear Biases||||||" << std::endl << std::endl;
    std::cout << std::fixed << std::setprecision(4);

    // Determine the maximum number of nodes in any single layer part
    int max_nodes_per_layer = n_input;
    max_nodes_per_layer = std::max(max_nodes_per_layer, n_output);
    if (n_hidden_layer > 0) {
        max_nodes_per_layer = std::max(max_nodes_per_layer, n_hidden_layer_node);
    }

    if (max_nodes_per_layer == 0 && n_hidden_layer == 0) { // Check if any nodes exist at all
        std::cout << "Network has no nodes/biases to display in this format." << std::endl;
        return;
    }

    const int col_width = 12; // Width for bias data columns (e.g., "Input Bias")
    const int idx_col_width = 5;  // Width for Node index column

    // Print header
    std::cout << std::left; // Align text to the left for headers
    std::cout << std::setw(idx_col_width) << "Node" << " | ";
    for (int i = 0; i < n_hidden_layer; ++i) {
        std::cout << std::setw(col_width) << ("H" + std::to_string(i) + " Bias") << " | ";
    }
    if (n_output > 0) {
        std::cout << std::setw(col_width) << "Output Bias" << std::endl;
    } else if (n_hidden_layer > 0) { // Ensure newline if any previous column was printed
        std::cout << std::endl;
    }
    std::cout << std::right; // Reset to default (right alignment for numbers)

    // Print separator line
    std::cout << std::string(idx_col_width, '-') << "-+-";
    for (int i = 0; i < n_hidden_layer; ++i) {
        std::cout << std::string(col_width, '-') << "-+-";
    }
    if (n_output > 0) {
        std::cout << std::string(col_width, '-') << std::endl;
    } else if (n_hidden_layer > 0) {
         std::cout << std::endl;
    }

    // Print bias values row by row
    for (int node_idx = 0; node_idx < max_nodes_per_layer; ++node_idx) {
        std::cout << std::setw(idx_col_width) << node_idx << " | ";

        // Hidden layer biases
        for (int h_layer_idx = 0; h_layer_idx < n_hidden_layer; ++h_layer_idx) {
            if (node_idx < n_hidden_layer_node && static_cast<size_t>(h_layer_idx) < hidden_layer_bias.size() && static_cast<size_t>(node_idx) < hidden_layer_bias[h_layer_idx].size() && !hidden_layer_bias[h_layer_idx][node_idx].empty()) {
                std::cout << std::setw(col_width) << hidden_layer_bias[h_layer_idx][node_idx][0] << " | ";
            } else {
                std::cout << std::setw(col_width) << (node_idx < n_hidden_layer_node ? "---" : "---") << " | ";
            }
        }

        // Output layer biases
        if (n_output > 0) {
            if (node_idx < n_output && static_cast<size_t>(node_idx) < output_bias.size() && !output_bias[node_idx].empty()) {
                std::cout << std::setw(col_width) << output_bias[node_idx][0] << " | ";
            } else {
                std::cout << std::setw(col_width) << (node_idx < n_output ? "N/A" : "---") << " | ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


void Linear::forward(){
    std::vector<std::vector<float>> current_activated_input = this->input; // Start with raw input

    if (n_hidden_layer > 0) {
        // Input to first hidden layer (H0)
        // hidden_layer_z[0] stores pre-activation (z)
        // hidden_layer[0] stores post-activation (a)
        this->hidden_layer_z[0] = matrix_multiply(this->input_weights, current_activated_input);
        this->hidden_layer_z[0] = matrix_add(this->hidden_layer_z[0], this->hidden_layer_bias[0]);
        this->hidden_layer[0] = matrix_apply(this->hidden_layer_z[0], this->activation_function);
        current_activated_input = this->hidden_layer[0]; // Update for the next layer

        // Through subsequent hidden layers (H1 to H_last)
        for(int i = 1; i < n_hidden_layer; i++){
            // hidden_layer_z[i] stores pre-activation
            // hidden_layer[i] stores post-activation
            this->hidden_layer_z[i] = matrix_multiply(this->hidden_layer_weights[i-1], current_activated_input);
            this->hidden_layer_z[i] = matrix_add(this->hidden_layer_z[i], this->hidden_layer_bias[i]);
            this->hidden_layer[i] = matrix_apply(this->hidden_layer_z[i], this->activation_function);
            current_activated_input = this->hidden_layer[i]; // Update for the next layer
        }
    }

    // To Output layer
    this->output_z = matrix_multiply(this->output_weights, current_activated_input);
    this->output_z = matrix_add(this->output_z, this->output_bias);
    this->output = matrix_apply(this->output_z, this->activation_function);
}

void Linear::backward(){
    std::vector<std::vector<float>> negative_desired_output = matrix_apply(desired_output, [](float x){return -x;});
    output_error = matrix_add(output, negative_desired_output);
    std::vector<std::vector<float>> derivative_terms = matrix_apply(output_z, this->derivative_activation_function);
    for(int i = 0; i < n_output; i++){
        output_error[i][0] = derivative_terms[i][0]*output_error[i][0];
    }


    for (int i = n_hidden_layer - 1; i >= 0; i--) {
        if (i == n_hidden_layer - 1) {
            std::vector<std::vector<float>> transposed_output_weights = matrix_transpose(this->output_weights);
            hidden_layer_error[i] = matrix_multiply(transposed_output_weights, output_error);
        } else {
            std::vector<std::vector<float>> transposed_hidden_weights = matrix_transpose(this->hidden_layer_weights[i]);
            hidden_layer_error[i] = matrix_multiply(transposed_hidden_weights, hidden_layer_error[i + 1]);
        }
        std::vector<std::vector<float>> derivative_terms = matrix_apply(hidden_layer_z[i], this->derivative_activation_function);
        for(int j = 0; j < n_hidden_layer_node; j++){

            hidden_layer_error[i][j][0] = derivative_terms[j][0]*hidden_layer_error[i][j][0];
        }
    }

}

void Linear::update_weights(float learning_rate){
    //Input weights:

    for (int i = 0; i < n_hidden_layer_node; i++) {
        for (int j = 0; j < n_input; j++) {
            input_weights[i][j] -= learning_rate * hidden_layer_error[0][i][0] * input[j][0];
        }
    }

    // Hidden layers weights
    for (int i = 0; i < n_hidden_layer - 1; i++) {
        for (int j = 0; j < n_hidden_layer_node; j++) {
            for (int k = 0; k < n_hidden_layer_node; k++) {
                hidden_layer_weights[i][j][k] -= learning_rate * hidden_layer_error[i + 1][j][0] * hidden_layer[i][k][0];
            }
        }
    }

    // Output weights

    for (int i = 0; i < n_output; i++) {
        for (int j = 0; j < n_hidden_layer_node; j++) {
            output_weights[i][j] -= learning_rate * hidden_layer_error[n_hidden_layer-1][j][0] * output_error[i][0];
        }
    }



}

void Linear::update_biases(float learning_rate){
    // Hidden layer biases
    // hidden_layer_bias[i][j][0] is the bias for node j in hidden layer i
    // The gradient is hidden_layer_error[i][j][0]
    for (int i = 0; i < n_hidden_layer; i++) {
        for (int j = 0; j < n_hidden_layer_node; j++) {
            hidden_layer_bias[i][j][0] -= learning_rate * hidden_layer_error[i][j][0];
        }
    }

    // Output layer biases
    // output_bias[i][0] is the bias for output node i
    // The gradient is output_error[i][0]
    for (int i = 0; i < n_output; i++) {
        output_bias[i][0] -= learning_rate * output_error[i][0];
    }
}


void Linear::set_input(std::vector<std::vector<float>> input){
    this->input = input;
}
void Linear::set_desired_output(std::vector<std::vector<float>> desired_output){
    this->desired_output = desired_output;
}


std::vector<std::vector<float>> Linear::get_input(){
    return this->input;
}

std::vector<std::vector<float>> Linear::get_output(){
    return this->output;
}


float Linear::get_final_error(){
    float error = 0.0f;
    for(int i = 0; i < n_output; i++){
        error += output_error[i][0] * output_error[i][0]*100;
    }
    return error;
}