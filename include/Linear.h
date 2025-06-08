#pragma once

#include <vector>
#include <string>
#include <functional>



enum activations{
    sigmoid,
    relu
};

class Linear
{
private:
int n_input;
int n_output;
int n_hidden_layer;
int n_hidden_layer_node;

// Node values
std::vector<std::vector<float>> input;
std::vector<std::vector<std::vector<float>>> hidden_layer;
std::vector<std::vector<float>> output;

std::vector<std::vector<float>> desired_output;

// Pre-activation values (z = Wx + b)
std::vector<std::vector<std::vector<float>>> hidden_layer_z; // For hidden layers
std::vector<std::vector<float>> output_z;                   // For output layer


// Errors
std::vector<std::vector<std::vector<float>>> hidden_layer_error;
std::vector<std::vector<float>> output_error;



// Weight and biases
std::vector<std::vector<float>> input_weights;
std::vector<std::vector<std::vector<float>>> hidden_layer_weights;
std::vector<std::vector<float>> output_weights;

std::vector<std::vector<std::vector<float>>> hidden_layer_bias;
std::vector<std::vector<float>> output_bias;


// Activation function

std::function<float(float)> activation_function;
std::function<float(float)> derivative_activation_function;

public:
Linear(int n_input, int n_output, int n_hidden_layer, int n_hidden_layer_node, bool randomize, activations activation_function);
~Linear();
void forward();
void backward();
void update_weights(float learning_rate);
void update_biases(float learning_rate);



void set_input(std::vector<std::vector<float>> input);
void set_desired_output(std::vector<std::vector<float>> desired_output);

std::vector<std::vector<float>> get_input();
std::vector<std::vector<float>> get_output();

    
void print_node_values();
void print_error_values();
void print_weights();
void print_biases();

float get_final_error();
};
