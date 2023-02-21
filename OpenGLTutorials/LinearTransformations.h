#pragma once
#ifndef LINEAR_TRANSFORMATIONS
#define LINEAR_TRANSFORMATIONS
#include "LinearAlgebra.h"
#define PI 3.14159265
# define DEG_TO_RAD PI / 180

namespace LinearAlgebra {
    Mat4 translate(float x, float y, float z);
    Mat4 translate(Vec4 translation_vector);
    Mat4 scale(float x, float y, float z);
    Mat4 scale(Vec4 translation_vector);
    Mat4 rotateX(float rotationX);
    Mat4 rotateY(float rotationY);
    Mat4 rotateZ(float rotationZ);
    Mat4 view_matrix(Vec4 up_vector, Vec4 focus_position, Vec4 cam_world_position);
    Mat4 projection_matrix(float near, float far, float fov, float aspect);

    class Versor {
            std::array<float, 4> values;
            void intialize_values(float x, float y, float z, float angle);
            Versor(std::array<float, 4>);
        public: 
            Versor(float x, float y, float z, float angle);
            Versor(float* orientation, int size, float angle);
            Versor(std::array<float, 3> orientation, float angle);
            float& operator [] (int index);
            Mat4 to_matrix();
            Versor normalize();
    };
};

#endif