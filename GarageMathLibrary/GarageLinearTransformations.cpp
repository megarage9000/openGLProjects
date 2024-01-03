
#include "GarageLinearTransformations.h"

namespace GarageLinearAlgebra {
    Mat4 translate(float x, float y, float z) {
        Mat4 transform_matrix = Mat4(IDENTITY_4, 16);
        transform_matrix[0][3] = x;
        transform_matrix[1][3] = y;
        transform_matrix[2][3] = z;
        return transform_matrix;
    }
    Mat4 translate(Vec3 translation_vector) {
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
        Vec4 new_right_vector = (forward_vector.cross_vec4(up_vector)).normalize();
        Vec4 new_up_vector = (forward_vector.cross_vec4(new_right_vector)).normalize();

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

    Mat4 view_matrix(Vec3 up_vector, Vec3 right_vector, Vec3 forward_vector, Vec4 cam_world_position) {
        Mat4 view;

        view[0][0] = right_vector[0];
        view[0][1] = right_vector[1];
        view[0][2] = right_vector[2];
        view[0][3] = -cam_world_position[0];

        view[1][0] = up_vector[0];
        view[1][1] = up_vector[1];
        view[1][2] = up_vector[2];
        view[1][3] = -cam_world_position[1];

        view[2][0] = forward_vector[0];
        view[2][1] = forward_vector[1];
        view[2][2] = forward_vector[2];
        view[2][3] = -cam_world_position[2];

        return view;
    }

    Mat4 view_matrix(Vec3 up_vector, Vec4 focus_position, Vec4 cam_world_position) {
        Vec3 forward_vector;
        Vec3 right_vector;
        return view_matrix(focus_position, cam_world_position, up_vector, forward_vector, right_vector);
    }

    Mat4 view_matrix(Vec4 focus_position, Vec4 cam_world_position, Vec3& up_vector, Vec3& forward_vector, Vec3& right_vector) {

        Mat4 view_matrix = Mat4(IDENTITY_4, 16);

        Vec3 direction = (cam_world_position - focus_position);
        forward_vector = direction.normalize();
        Vec3 new_right_vector = (Vec3{ 0.0f, 1.0f, 0.0 }.cross(forward_vector)).normalize();
        Vec3 new_up_vector = (forward_vector.cross(new_right_vector)).normalize();

        up_vector = new_up_vector;
        right_vector = new_right_vector;

        /*Mat4 translation_matrix = translate(cam_world_position * -1);

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
        view_matrix[2][3] = -cam_world_position[2];*/


        //Mat4 translation_matrix = Mat4(IDENTITY_4, 16);
        //translation_matrix[0][3] = -cam_world_position[0];
        //translation_matrix[1][3] = -cam_world_position[1];
        //translation_matrix[2][3] = -cam_world_position[2];

        //translation_matrix.print();


        //right_vector.print();
        //up_vector.print();
        //forward_vector.print();

        //Mat4 rotation_matrix = Mat4(IDENTITY_4, 16);
        //rotation_matrix[0][0] = right_vector[0];
        //rotation_matrix[0][1] = right_vector[1];
        //rotation_matrix[0][2] = right_vector[2];

        //rotation_matrix[1][0] = up_vector[0];
        //rotation_matrix[1][1] = up_vector[1];
        //rotation_matrix[1][2] = up_vector[2];

        //rotation_matrix[2][0] = forward_vector[0];
        //rotation_matrix[2][1] = forward_vector[1];
        //rotation_matrix[2][2] = forward_vector[2];
        //
        //rotation_matrix.print();
        //view_matrix = rotation_matrix * translation_matrix;

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
        float radians = angle * DEG_TO_RAD;
        values[0] = cosf(radians / 2.0f);
        values[1] = sinf(radians / 2.0f) * x;
        values[2] = sinf(radians / 2.0f) * y;
        values[3] = sinf(radians / 2.0f) * z;
    }

    Versor::Versor() {
        intialize_values(0.0f, 1.0f, 0.0f, 0.0f);
    }

    Versor::Versor(float x, float y, float z, float angle) {
        intialize_values(x, y, z, angle);
    }

    Versor::Versor(float* orientation, int size, float angle) {
        if (size == 3) {
            intialize_values(orientation[0], orientation[1], orientation[2], angle);
        }
    }

    Versor::Versor(std::array<float, 3> orientation, float angle) {
        intialize_values(orientation[0], orientation[1], orientation[2], angle);
    }

    Versor::Versor(Vec3 axis, float angle) {
        intialize_values(axis[0], axis[1], axis[2], angle);
    }

    float& Versor::operator[] (int index) {
        return values[index];
    }

    Versor Versor::operator * (Versor& r) {
        std::array<float, 4> new_values;

        new_values[0] = r[0] * values[0] - r[1] * values[1] - r[2] * values[2] - r[3] * values[3];
        new_values[1] = r[0] * values[1] + r[1] * values[0] - r[2] * values[3] + r[3] * values[2];
        new_values[2] = r[0] * values[2] + r[1] * values[3] + r[2] * values[0] - r[3] * values[1];
        new_values[3] = r[0] * values[3] - r[1] * values[2] + r[2] * values[1] + r[3] * values[0];

        return Versor(new_values).normalize();

    }

    Mat4 Versor::to_matrix() {
        Mat4 matrix = Mat4();

        float w = values[0];
        float x = values[1];
        float y = values[2];
        float z = values[3];

        matrix[0][0] = 1.0f - 2.0f * (y * y) - 2.0f * (z * z);
        matrix[0][1] = 2.0f * (x * y) - 2.0f * (w * z);
        matrix[0][2] = 2.0f * (x * z) + 2.0f * (w * y);
        matrix[0][3] = 0.0f;
        matrix[1][0] = 2.0f * (x * y) + 2.0f * (w * z);
        matrix[1][1] = 1.0f - 2.0f * (x * x) - 2.0f * (z * z);
        matrix[1][2] = 2.0f * (y * z) - 2.0f * (w * x);
        matrix[1][3] = 0.0;
        matrix[2][0] = 2.0f * (x * z) - 2.0f * (w * y);
        matrix[2][1] = 2.0f * (y * z) + 2.0f * (w * x);
        matrix[2][2] = 1.0f - 2.0f * (x * x) - 2.0f * (y * y);
        matrix[2][3] = 0.0f;
        matrix[3][0] = 0.0f;
        matrix[3][1] = 0.0f;
        matrix[3][2] = 0.0f;
        matrix[3][3] = 1.0f;
        return matrix;
    }

    Versor Versor::normalize() {
        std::array<float, 4> new_values = values;
        float sum = new_values[0] * new_values[0] +
            new_values[1] * new_values[1] +
            new_values[2] * new_values[2] +
            new_values[3] * new_values[3];

        if (float_equals(sum, 1.0f) == false) {
            float mag = sqrt(sum);
            new_values[0] = new_values[0] / mag;
            new_values[1] = new_values[1] / mag;
            new_values[2] = new_values[2] / mag;
            new_values[3] = new_values[3] / mag;
        }
        return Versor(new_values);
    }

    Versor Versor::conjugate() {
        std::array<float, 4> new_values;
        new_values[0] = values[0];
        new_values[1] = -values[1];
        new_values[2] = -values[2];
        new_values[3] = -values[3];
        return Versor(new_values);
    }

    float Versor::dot(Versor& other_versor) {
        return
            values[0] * other_versor[0] +
            values[1] * other_versor[1] +
            values[2] * other_versor[2] +
            values[3] * other_versor[3];
    }

    Versor Versor::slerp(Versor& other_versor, float t) {
        float dp = dot(other_versor);
        if (float_equals(dp, 1.0)) {
            return Versor(values);
        }

        float sin_omega = sqrt(1.0f - dp * dp);
        std::array<float, 4> versor_values;

        if (fabs(sin_omega) < 0.001f) {
            for (int i = 0; i < 4; i++) {
                versor_values[i] = (1.0f - t) * values[i] + t * other_versor[i];
            }
            return Versor(versor_values);
        }

        float omega = acos(dp);
        float a = sin((1.0f - t) * omega) / sin_omega;
        float b = sin(t * omega) / omega;

        for (int i = 0; i < 4; i++) {
            versor_values[i] = values[i] * a + other_versor[i] * b;
        }
        return Versor(versor_values);
    }

    void Versor::print() {
        std::cout << "\n--------------------------------\n";
        for (int i = 0; i < 4; i++) {
            std::cout << std::setw(16) << std::setprecision(5) << (*this)[i];
        }
        std::cout << "\n--------------------------------\n";
    }
};


