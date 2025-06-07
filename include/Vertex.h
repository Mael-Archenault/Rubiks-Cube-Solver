#pragma once
#include <iostream>

class Vertex {
public:
    float x, y, z;

    // Constructor with member initializer list
    Vertex(float x_val, float y_val, float z_val) : x(x_val), y(y_val), z(z_val) {
        // std::cout << "Vertex created: (" << x << ", " << y << ", " << z << ")" << std::endl;
    }

    // Destructor
    ~Vertex() {
        // std::cout << "Vertex destroyed: (" << x << ", " << y << ", " << z << ")" << std::endl;
    }

    // Print method (const-correct)
    void print() const {
        std::cout << "Vertex: (" << x << ", " << y << ", " << z << ")" << std::endl;
    }
};
