
#include "LinearTransformations.h"

//namespace LinearTransformations {
//
//    using namespace LinearAlgebra;
//    
//    void translate(float transform_matrix[], float translation_vector[]) {
//        copy_from_matrix4(IDENTITY_4, transform_matrix, 16, 16);
//        transform_matrix[3] = translation_vector[0];
//        transform_matrix[7] = translation_vector[1];
//        transform_matrix[11] = translation_vector[2];
//    }
//
//    void scale(float transform_matrix[], float scale_vector[]) {
//        copy_from_matrix4(IDENTITY_4, transform_matrix, 16, 16);
//        transform_matrix[0] = scale_vector[0];
//        transform_matrix[5] = scale_vector[1];
//        transform_matrix[10] = scale_vector[2];
//    }
//
//    void rotateX(float transform_matrix[], float rotationX) {
//        copy_from_matrix4(IDENTITY_4, transform_matrix, 16, 16);
//        transform_matrix[5] = cos(rotationX * PI / 180);
//        transform_matrix[6] = -sin(rotationX * PI / 180);
//        transform_matrix[9] = -transform_matrix[6];
//        transform_matrix[10] = transform_matrix[5];
//    }
//
//    void rotateY(float transform_matrix[], float rotationY) {
//        copy_from_matrix4(IDENTITY_4, transform_matrix, 16, 16);
//        transform_matrix[0] = cos(rotationY * PI / 180);
//        transform_matrix[2] = sin(rotationY * PI / 180);
//        transform_matrix[8] = -transform_matrix[2];
//        transform_matrix[10] = transform_matrix[0];
//    }
//
//    void rotateZ(float transform_matrix[], float rotationZ) {
//        copy_from_matrix4(IDENTITY_4, transform_matrix, 16, 16);
//        transform_matrix[0] = cos(rotationZ * PI / 180);
//        transform_matrix[1] = -sin(rotationZ * PI / 180);
//        transform_matrix[4] = -transform_matrix[1];
//        transform_matrix[5] = transform_matrix[0];
//    }
//
//    void cross_product_vec4(float a[], float b[], float result[]) {
//        float a_3[3] = {a[0], a[1], a[2]};
//        float b_3[3] = {b[0], b[1], b[2]};
//        float result_3[3] = {result[0], result[1], result[2]};
//        cross_product_vec3(a_3, b_3, 3, 3, result_3);
//        result[0] = result_3[0];
//        result[1] = result_3[1];
//        result[2] = result_3[2];
//    }
//
//    void view_matrix(float view_matrix[], float up_vector[], float focus_position[], float cam_world_position[]) {
//        copy_from_matrix4(IDENTITY_4, view_matrix, 16, 16);
//
//        float forward_vector[4];
//        subtract_vectors(focus_position, cam_world_position, forward_vector, 4);
//        normalize_vector(forward_vector, 4);
//
//        float new_up_vector[4];
//        cross_product_vec4(up_vector, forward_vector, new_up_vector);
//        normalize_vector(new_up_vector, 4);
//
//        float new_right_vector[4];
//        cross_product_vec4(forward_vector, new_up_vector, new_right_vector);
//        normalize_vector(new_right_vector, 4);
//
//        view_matrix[0] =  new_right_vector[0];
//        view_matrix[1] = new_right_vector[1];
//        view_matrix[2] = new_right_vector[2];
//        view_matrix[3] = -cam_world_position[0];
//
//        view_matrix[4] = new_up_vector[0];
//        view_matrix[5] = new_up_vector[1];
//        view_matrix[6] = new_up_vector[2];
//        view_matrix[7] = -cam_world_position[1];
//
//        view_matrix[8] = -forward_vector[0];
//        view_matrix[9] = -forward_vector[1];
//        view_matrix[10] = -forward_vector[2];
//        view_matrix[11] = -cam_world_position[2];
//    }
//
//    void projection_matrix(float result_matrix[], float near, float far, float fov, float range, float aspect){
//        copy_from_matrix4(IDENTITY_4, result_matrix, 16, 16);
//        result_matrix[0] = (2.0f * near) / (range * aspect + range * aspect);
//        result_matrix[5] = near / range;
//        result_matrix[10] = -(far + near) / (far - near);
//        result_matrix[11] = -(2.0 * far * near) / (far - near); 
//        result_matrix[14] = -1;
//        result_matrix[15] = 0;
//    }
//
//}
//    void projection_matrix(float result_matrix[], float near, float far, float fov, float range, float aspect){
//        copy_from_matrix4(IDENTITY_4, result_matrix, 16, 16);
//        result_matrix[0] = (2.0f * near) / (range * aspect + range * aspect);
//        result_matrix[5] = near / range;
//        result_matrix[10] = -(far + near) / (far - near);
//        result_matrix[11] = -(2.0 * far * near) / (far - near); 
//        result_matrix[14] = -1;
//        result_matrix[15] = 0;
//    }

namespace LinearAlgebra {
    Mat4 translate(float x, float y, float z) {
        Mat4 transform_matrix = Mat4(IDENTITY_4, 16);
        transform_matrix[0][3] = x;
        transform_matrix[1][3] = y;
        transform_matrix[2][3] = z;
        return transform_matrix;
    }
    Mat4 translate(Vec4 translation_vector) {
        Mat4 transform_matrix = Mat4(IDENTITY_4, 16);
        transform_matrix[0][3] = translation_vector[0];
        transform_matrix[1][3] = translation_vector[1];
        transform_matrix[2][3] = translation_vector[2];
        return transform_matrix;
    }

    Mat4 scale(float x, float y, float z) {
        Mat4 transform_matrix = Mat4(IDENTITY_4, 16);
        transform_matrix[0][0] = x;
        transform_matrix[1][1] = y;
        transform_matrix[2][2] = z;
        return transform_matrix;
    }

    Mat4 scale(Vec4 translation_vector) {
        Mat4 transform_matrix = Mat4(IDENTITY_4, 16);
        transform_matrix[0][0] = translation_vector[0];
        transform_matrix[1][1] = translation_vector[1];
        transform_matrix[2][2] = translation_vector[2];
        return transform_matrix;
    }
    Mat4 rotateX(float rotationX) {
        Mat4 transform_matrix = Mat4(IDENTITY_4, 16);
        transform_matrix[1][1] = cos(rotationX * DEG_TO_RAD);
        transform_matrix[1][2] = -sin(rotationX * DEG_TO_RAD);
        transform_matrix[2][1] = -transform_matrix[1][2];
        transform_matrix[2][2] = transform_matrix[1][1];
        return transform_matrix;
    }
    Mat4 rotateY(float rotationY) {
        Mat4 transform_matrix = Mat4(IDENTITY_4, 16);
        transform_matrix[0][0] = cos(rotationY * DEG_TO_RAD);
        transform_matrix[0][2] = sin(rotationY * DEG_TO_RAD);
        transform_matrix[2][0] = -transform_matrix[0][2];
        transform_matrix[2][2] = transform_matrix[0][0];
        return transform_matrix;
    }
    Mat4 rotateZ(float rotationZ) {
        Mat4 transform_matrix = Mat4(IDENTITY_4, 16);
        transform_matrix[0][0] = cos(rotationZ * DEG_TO_RAD);
        transform_matrix[0][1] = -sin(rotationZ * DEG_TO_RAD);
        transform_matrix[1][0] = -transform_matrix[0][1];
        transform_matrix[1][1] = transform_matrix[0][0];
        return transform_matrix;
    }
    Mat4 view_matrix(Vec4 up_vector, Vec4 focus_position, Vec4 cam_world_position) {
        Mat4 view_matrix = Mat4(IDENTITY_4, 16);

        Vec4 forward_vector = (focus_position - cam_world_position).normalize();
        Vec4 new_up_vector = (up_vector.cross_vec4(forward_vector)).normalize();
        Vec4 new_right_vector = (new_up_vector.cross_vec4(forward_vector)).normalize();

        view_matrix[0][0] = new_right_vector[0];
        view_matrix[0][1] = new_right_vector[1];
        view_matrix[0][2] = new_right_vector[2];
        view_matrix[0][3] = -cam_world_position[0];

        view_matrix[1][0] = new_up_vector[0];
        view_matrix[1][1] = new_up_vector[1];
        view_matrix[1][2] = new_up_vector[2];
        view_matrix[1][3] = -cam_world_position[1];

        view_matrix[2][0] = -forward_vector[0];
        view_matrix[2][1] = -forward_vector[1];
        view_matrix[2][2] = -forward_vector[2];
        view_matrix[2][3] = -cam_world_position[2];

        return view_matrix;
    }
    Mat4 projection_matrix(float near, float far, float fov, float aspect) {
        Mat4 projection_matrix = Mat4(IDENTITY_4, 16);
        float range = tan(fov / 2) * near;
        projection_matrix[0][0] = (2 * near) / (range * aspect + range * aspect);
        projection_matrix[1][1] = near / range;
        projection_matrix[2][2] = -(far + near) / (far - near);
        projection_matrix[2][3] = -(2 * far * near) / (far - near);
        projection_matrix[3][2] = -1;
        projection_matrix[3][3] = 0;
        return projection_matrix;
    }

    void Versor::intialize_values(float x, float y, float z, float angle) {
        values[0] = cos(angle * DEG_TO_RAD / 2);
        values[1] = sin(angle * DEG_TO_RAD / 2) * x;
        values[2] = sin(angle * DEG_TO_RAD / 2) * y;
        values[3] = sin(angle * DEG_TO_RAD / 2) * z;


    }

    Versor::Versor(std::array<float, 4> values_from_versor) : values(values_from_versor) {}

    Versor::Versor(float x, float y, float z, float angle) {
        intialize_values(x, y, z, angle);
    }

    Versor::Versor(float* orientation, int size, float angle) {
        if (size == 4) {
            intialize_values(orientation[0], orientation[1], orientation[2], angle);
        }
    }
    Versor::Versor(std::array<float, 3> orientation, float angle) {
        intialize_values(orientation[0], orientation[1], orientation[2], angle);
    }

    float& Versor::operator[] (int index) {
        return values[index];
    }

    Versor Versor::operator * (Versor& other_versor) {
        std::array<float, 4> new_values;
        
        new_values[0] =
            values[0] * other_versor[0] - values[1] * other_versor[1] - values[2] * other_versor[2] - values[3] * other_versor[3];
        new_values[1] =
            values[1] * other_versor[0] + values[0] * other_versor[1] - values[3] * other_versor[2] + values[2] * other_versor[3];
        new_values[2] =
            values[2] * other_versor[0] + values[3] * other_versor[1] + values[0] * other_versor[2] - values[1] * other_versor[3];
        new_values[0] =
            values[3] * other_versor[0] - values[2] * other_versor[1] - values[1] * other_versor[2] - values[0] * other_versor[3];

        return Versor(new_values);

    }

    Mat4 Versor::to_matrix() {
        Mat4 matrix = Mat4();
        float w = values[0];
        float x = values[1];
        float y = values[2];
        float z = values[3];

        matrix[0][0] = 1 - 2 * (y * y) - 2 * (z * z);
        matrix[0][1] = 2 * (x * y) - 2 * (w * z);
        matrix[0][3] = 2 * (x * z) + 2 * (w * y);
        matrix[1][0] = 2 * (x * y) + 2 * (w * z);
        matrix[1][1] = 1 - 2 * (x * x) - 2 * (z * z);
        matrix[1][2] = 2 * (y * z) - 2 * (w * x);
        matrix[2][0] = 2 * (x * z) - 2 * (w * y);
        matrix[2][1] = 2 * (y * z) + 2 * (w * x);
        matrix[2][2] = 1 - 2 * (x * x) - 2 * (y * y);
        
        return matrix;
    }

    Versor Versor::normalize() {
        std::array<float, 4> new_values = values;
        float mag = sqrt(
            new_values[0] * new_values[0] +
            new_values[1] * new_values[1] +
            new_values[2] * new_values[2] +
            new_values[3] * new_values[3]);
        new_values[0] = new_values[0] / mag;
        new_values[1] = new_values[1] / mag;
        new_values[2] = new_values[2] / mag;
        new_values[3] = new_values[3] / mag;
        return Versor(new_values);
    }
};


