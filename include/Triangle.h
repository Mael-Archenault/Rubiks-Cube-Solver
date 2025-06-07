#pragma once
#include "Vertex.h"
#include <vector>


class Triangle
{
private:
    /* data */
public:
    std::vector<Vertex> vertices;
    Triangle(Vertex v1, Vertex v2, Vertex v3){
        vertices.push_back(v1);
        vertices.push_back(v2);
        vertices.push_back(v3);
    };
    ~Triangle(){
        vertices.clear();
    };
    void print() const {
        std::cout << "Triangle : "<< std::endl;
        for(int i = 0; i < vertices.size(); i++){
            std::cout << "\t";
            vertices[i].print();

        }
    }
};