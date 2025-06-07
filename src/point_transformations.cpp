#include "Vertex.h"
#include <cmath>


const float PI_F = std::acos(-1.0f);




Vertex translate(const Vertex& point, float dx, float dy, float dz) {
    return Vertex(point.x + dx, point.y + dy, point.z + dz);
}

Vertex scale(const Vertex& point, float sx, float sy, float sz) {
    return Vertex(point.x * sx, point.y * sy, point.z * sz);
}

Vertex rotateX(const Vertex& point, float angle_deg){
    float angle_rad = angle_deg *PI_F/180.0f;
    float cos_a = std::cos(angle_rad);
    float sin_a = std::sin(angle_rad);
    return Vertex(point.x, point.y * cos_a - point.z * sin_a, point.y * sin_a + point.z * cos_a);
}

Vertex rotateY(const Vertex& point, float angle_deg){
    float angle_rad = angle_deg *PI_F/180.0f;
    float cos_a = std::cos(angle_rad);
    float sin_a = std::sin(angle_rad);
    return Vertex(point.x * cos_a + point.z * sin_a, point.y, -point.x * sin_a + point.z * cos_a);
}

Vertex rotateZ(const Vertex& point, float angle_deg){
    float angle_rad = angle_deg *PI_F/180.0f;
    float cos_a = std::cos(angle_rad);
    float sin_a = std::sin(angle_rad);
    return Vertex(point.x * cos_a - point.y * sin_a, point.x * sin_a + point.y * cos_a, point.z);
}

Vertex rotate(const Vertex& point, float angle_x, float angle_y, float angle_z){
    return rotateZ(rotateY(rotateX(point, angle_x), angle_y), angle_z);
}