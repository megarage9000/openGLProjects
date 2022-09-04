
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
        result_matrix[0] = (2.0f * near) / (range * aspect + range * aspect);
        result_matrix[5] = near / range;
        result_matrix[10] = -(far + near) / (far - near);
        result_matrix[11] = -(2.0 * far * near) / (far - near); 
        result_matrix[14] = -1;
        result_matrix[15] = 0;
    }

    // --- Quanternions --- //
    void slerp(float versor_a[], float versor_b[], float result_versor[], float t) {
        copy_from_vec4(versor_a, result_versor, 4, 4);
        float dot = dot_product(versor_a, versor_b, 4, 4);

        // Check if the dot is less than 0 to avoid flick. If so, negate a vector
        if (dot < 0.0f) {
            for (int i = 0; i < 4; i++) {
                versor_a[i] *= -1.0f;
            }
        }
        
        // If dot product is ~= 1, return 1st versor
        if (fabs(dot) >= 1.0f) {
            return;
        }

        // If sin omega = 0, do linear interpolation (means versor angle is 180)
        float sin_omega = sqrt(1.0f - dot * dot);
        if (fabs(sin_omega) < 0.0001f) {
            for (int i = 0; i < 4; i++) {
                result_versor[i] = (1.0f - t) * versor_a[i] + t * versor_b[i];
            }
            return;
        }
        // Else, we do standard spherical interpolation
        else {
            float omega = acos(dot);
            float a = sin((1.0f - t) * omega) / sin_omega;
            float b = sin(t * omega) / sin_omega;
            for (int i = 0; i < 4; i++) {
                result_versor[i] = a * versor_a[i] + b * versor_b[i];
            }
            return;
        }
    }

    void versor(float result_versor[], float angle, float x, float y, float z) {
        copy_from_vec4(MAG_1_VEC4, result_versor, 4, 4);

        float rads = (angle / 2) * DEG_TO_RAD;
        float cos_comp = cos(rads);
        float sin_comp = sin(rads);

        result_versor[0] = cos_comp;
        float w = result_versor[0];
        result_versor[1] = sin_comp * x;
        float x_v = result_versor[1];
        result_versor[2] = sin_comp * y;
        float y_v = result_versor[2];
        result_versor[3] = sin_comp * z;
        float z_v = result_versor[3];

        // Normailzing if needed
        float mag = sqrt(w * w + x_v * x_v + y_v * y_v + z_v * z_v);
        if (mag >= 1) {
            normalize_vector(result_versor, 4);
        }
    }

    void versor_multiplication(float a[], float b[], float result_versor[]) {
        copy_from_vec4(MAG_1_VEC4, result_versor, 4, 4);
        /*
            t = q x r
            t = 
            [
                r0q0 - r1q1 - r2q2 - r3q3
                r0q1 + r1q0 - r2q3 + r3q2
                r0q2 + r1q3 + r2q0 - r3q1
                r0q3 - r1q2 + r2q1 + r3q0
            ]
        */
        result_versor[0] = a[0] * b[0] - a[1] * b[1] - a[2] * b[2] - a[3] * b[3];
        result_versor[1] = a[0] * b[1] + a[1] * b[0] - a[2] * b[3] + a[3] * b[2];
        result_versor[2] = a[0] * b[2] + a[1] * b[3] + a[2] * b[0] - a[3] * b[1];
        result_versor[3] = a[0] * b[3] - a[1] * b[2] + a[2] * b[1] + a[3] * b[0];
    }

    void to_quanternion(float versor[], float result_quaternion[]) {
        /*
            q = [w, (x, y, z)]

            m = [
                1 - 2y^2 - 2z^2, 2xy - 2wz,     2xz + 2wy,       0
                2xy + 2wz,       1 - 2x^2 - 2z, 2yz - 2wx,       0
                2xz - 2wy,       2yz + 2wx      1 - 2x^2 - 2y^2, 0
                0                0              0                1
            ]
        */
        copy_from_matrix4(IDENTITY_4, result_quaternion, 16, 16);

        float w = versor[0];
        float x = versor[1];
        float y = versor[2];
        float z = versor[3];

        // 1st row
        result_quaternion[0] = 1 - 2 * y * y - 2 * z * z;
        result_quaternion[1] = 2 * x * y - 2 * w * z;
        result_quaternion[2] = 2 * x * y + 2 * w * y;

        // 2nd row
        result_quaternion[4] = 2 * x * y + 2 * w * z;
        result_quaternion[5] = 1 - 2 * x * x - 2 * z * z;
        result_quaternion[6] = 2 * y * z - 2 * w * x;

        // 3rd row
        result_quaternion[8] = 2 * x * z - 2 * w * y;
        result_quaternion[9] = 2 * y * z + 2 * w * x;
        result_quaternion[10] = 1 - 2 * x * x - 2 * y * y;
    }
}