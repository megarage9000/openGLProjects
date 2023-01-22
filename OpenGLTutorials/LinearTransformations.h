#pragma once
#ifndef LINEAR_TRANSFORMATIONS
#define LINEAR_TRANSFORMATIONS
#include "LinearAlgebra.h"
#define PI 3.14159265
# define DEG_TO_RAD (2.0 * PI) / 360

namespace LinearAlgebra {
    Mat4 translate(Vec4 translation_vector);
    Mat4 scale(Vec4 translation_vector);
    Mat4 rotateX(float rotationX);
    Mat4 rotateY(float rotationY);
    Mat4 rotateZ(float rotationZ);
    Mat4 view_matrix(Vec4 up_vector, Vec4 focus_position, Vec4 cam_world_position);
    Mat4 projection_matrix(float near, float far, float fov, float aspect);
};

#endif