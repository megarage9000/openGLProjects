#pragma once
#ifndef LINEAR_TRANSFORMATIONS
#define LINEAR_TRANSFORMATIONS
#include "LinearAlgebra.h"
#define PI 3.14159265

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
};

#endif