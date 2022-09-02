
#include "LinearTransformations.h"

namespace LinearTransformations {

    using namespace LinearAlgebra;
    
    void translate(float transform_matrix[], float translation_vector[]) {
        copy_from_matrix4(IDENTITY_4, transform_matrix, 16, 16);
        transform_matrix[3] = translation_vector[0];
        transform_matrix[7] = translation_vector[1];
        transform_matrix[11] = translation_vector[2];
    }

    void scale(float transform_matrix[], float scale_vector[]) {
        copy_from_matrix4(IDENTITY_4, transform_matrix, 16, 16);
        transform_matrix[0] = scale_vector[0];
        transform_matrix[5] = scale_vector[1];
        transform_matrix[10] = scale_vector[2];
    }

    void rotateX(float transform_matrix[], float rotationX) {
        copy_from_matrix4(IDENTITY_4, transform_matrix, 16, 16);
        transform_matrix[5] = cos(rotationX * PI / 180);
        transform_matrix[6] = -sin(rotationX * PI / 180);
        transform_matrix[9] = -transform_matrix[6];
        transform_matrix[10] = transform_matrix[5];
    }

    void rotateY(float transform_matrix[], float rotationY) {
        copy_from_matrix4(IDENTITY_4, transform_matrix, 16, 16);
        transform_matrix[0] = cos(rotationY * PI / 180);
        transform_matrix[2] = sin(rotationY * PI / 180);
        transform_matrix[8] = -transform_matrix[2];
        transform_matrix[10] = transform_matrix[0];
    }

    void rotateZ(float transform_matrix[], float rotationZ) {
        copy_from_matrix4(IDENTITY_4, transform_matrix, 16, 16);
        transform_matrix[0] = cos(rotationZ * PI / 180);
        transform_matrix[1] = -sin(rotationZ * PI / 180);
        transform_matrix[4] = -transform_matrix[1];
        transform_matrix[5] = transform_matrix[0];
    }

    void cross_product_vec4(float a[], float b[], float result[]) {
        float a_3[3] = {a[0], a[1], a[2]};
        float b_3[3] = {b[0], b[1], b[2]};
        float result_3[3] = {result[0], result[1], result[2]};
        cross_product_vec3(a_3, b_3, 3, 3, result_3);
        result[0] = result_3[0];
        result[1] = result_3[1];
        result[2] = result_3[2];
    }

    void view_matrix(float view_matrix[], float up_vector[], float focus_position[], float cam_world_position[]) {
        copy_from_matrix4(IDENTITY_4, view_matrix, 16, 16);

        float forward_vector[4];
        subtract_vectors(focus_position, cam_world_position, forward_vector, 4);
        normalize_vector(forward_vector, 4);

        float new_up_vector[4];
        cross_product_vec4(up_vector, forward_vector, new_up_vector);
        normalize_vector(new_up_vector, 4);

        float new_right_vector[4];
        cross_product_vec4(forward_vector, new_up_vector, new_right_vector);
        normalize_vector(new_right_vector, 4);

        view_matrix[0] =  new_right_vector[0];
        view_matrix[1] = new_right_vector[1];
        view_matrix[2] = new_right_vector[2];
        view_matrix[3] = -cam_world_position[0];

        view_matrix[4] = new_up_vector[0];
        view_matrix[5] = new_up_vector[1];
        view_matrix[6] = new_up_vector[2];
        view_matrix[7] = -cam_world_position[1];

        view_matrix[8] = -forward_vector[0];
        view_matrix[9] = -forward_vector[1];
        view_matrix[10] = -forward_vector[2];
        view_matrix[11] = -cam_world_position[2];
    }

    void projection_matrix(float result_matrix[], float near, float far, float fov, float range, float aspect){
        copy_from_matrix4(IDENTITY_4, result_matrix, 16, 16);
        float temp = tan(fov/2);
        result_matrix[0] = 1 / (aspect * temp);
        result_matrix[5] = 1 / temp;
        result_matrix[10] = -(far + near) / (far - near);
        result_matrix[11] = -(2 * far * near) / (far - near); 
        result_matrix[14] = -1;
        result_matrix[15] = 0;
    }

}