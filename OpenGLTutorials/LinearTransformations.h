#pragma once
#ifndef LINEAR_TRANSFORMATIONS
#define LINEAR_TRANSFORMATIONS
#include "LinearAlgebra.h"
#define PI 3.14159265
# define DEG_TO_RAD (2.0 * PI) / 360

namespace LinearTransformations {

    // Translations
    void translate(float transform_matrix[], float translation_vector[]);

    // Scales
    void scale(float transform_matrix[], float scale_vector[]);

    // Rotations
    void rotateX(float transform_matrix[], float rotationX);
    void rotateY(float transform_matrix[], float rotationY);
    void rotateZ(float transform_matrix[], float rotationZ);

    void cross_product_vec4(float a[], float b[], float result[]);

    // View Matrix
    void view_matrix(float view_matrix[], float up_vector[], float focus_position[], float cam_world_position[]);

    // Projection Matrix
    void projection_matrix(float resultMatrix[], float near, float far, float fov, float range, float aspect);

    // --- Quaternions --- 
    void slerp(float versor_a[], float versor_b[], float result_versor[], float t);
    
    // Assumes angle in degrees
    void versor(float result_versor[], float angle, float x, float y, float z);

    void versor_multiplication(float a[], float b[], float result_versor[]);

    void to_quanternion(float versor[], float result_quaternion[]);
};

#endif