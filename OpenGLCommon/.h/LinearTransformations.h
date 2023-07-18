#pragma once
#ifndef LINEAR_TRANSFORMATIONS
#define LINEAR_TRANSFORMATIONS
#include "LinearAlgebra.h"
#define PI 3.14159265
#define DEG_TO_RAD PI / 180.0f

namespace LinearAlgebra {
    Mat4 translate(float x, float y, float z);
    Mat4 translate(Vec3 translation_vector);
    Mat4 scale(float x, float y, float z);
    Mat4 scale(Vec4 translation_vector);
    Mat4 rotateX(float rotationX);
    Mat4 rotateY(float rotationY);
    Mat4 rotateZ(float rotationZ);
    Mat4 view_matrix(Vec3 up_vector, Vec3 right_vector, Vec3 forward_vector, Vec4 cam_world_position);
    Mat4 view_matrix(Vec3 up_vector, Vec4 focus_position, Vec4 cam_world_position);
    Mat4 view_matrix(Vec4 focus_position, Vec4 cam_world_position, Vec3& up_vector, Vec3& forward_vector, Vec3& right_vector);
    Mat4 projection_matrix(float near, float far, float fov, float aspect);


    // To check implementation, need to see 
    // https://github.com/capnramses/antons_opengl_tutorials_book/blob/master/06_vcam_with_quaternion/main.cpp


    class Versor {
            std::array<float, 4> values;
            void intialize_values(float x, float y, float z, float angle);
            Versor(std::array<float, 4> new_values) : values(new_values) {}
        public: 
            Versor();
            Versor(float x, float y, float z, float angle);
            Versor(float* orientation, int size, float angle);
            Versor(std::array<float, 3> orientation, float angle);
            Versor(Vec3 axis, float angle);
            float& operator [] (int index);
            Versor operator * (Versor& other_versor);
            Mat4 to_matrix();
            Versor normalize();
            Versor conjugate();
            float dot(Versor& other_versor);
            Versor slerp(Versor& other_versor, float t);
            void print();
    };
};

#endif