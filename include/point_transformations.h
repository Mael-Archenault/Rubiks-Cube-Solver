#pragma once

#include "Vertex.h"
#include "math.h"




Vertex translate(const Vertex& point, float dx, float dy, float dz);

Vertex scale(const Vertex& point, float sx, float sy, float sz);

Vertex rotate(const Vertex& point, float angle_x, float angle_y, float angle_z);
