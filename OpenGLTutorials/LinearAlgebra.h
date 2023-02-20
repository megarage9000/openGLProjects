#pragma once
#ifndef LINEAR_ALGEBRA
#define LINEAR_ALGEBRA

#define EPISLON 0.000001
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <array>
#include <assert.h>

void swap(float &a, float &b);
bool float_equals(float a, float b);

namespace LinearAlgebra {


    // Identity Matrix 4
    static float IDENTITY_4[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    // Identity Matrix 3
    static float IDENTITY_3[9] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    // Vector 4 of magnitude 1
    static float MAG_1_VEC4[4] = {
        1.0f, 1.0f, 1.0f, 1.0f
    };

    // Vector 3 of magnitude 1
    static float MAG_1_VEC3[3] = {
        1.0f, 1.0f, 1.0f
    };

    // --- Matrix 4 --- //
    // Copies contents from one matrix 4 to another
    void copy_from_matrix4(float src[], float dest[], int src_len, int dest_len);

    // Copies contents from one vector 4 to another
    void copy_from_vec4(float src[], float dest[], int src_len, int dest_len);

    // Performs matrix 4 x matrix 4
    // - For matrix 4 x vector 4, set a = matrix, b = vector and pass in is_b_vec to true
    void matrix4_multi(float a[], float b[], int a_len, int b_len, int result_len ,float result_arr[]);

    // Assumes array lengths are correct
    // Performs matrix 4 x matrix 4 / matrix 4 x vector 4 calculations
    // - For matrix 4 x vector 4, set a = matrix, b = vector and pass in is_b_vec to true
    // TODO: investigate test-cases for row and column vectors, see if they yield differences
    void matrix4_multi(float a[], float b[], float result_arr[], bool is_b_vec);

    // --- Overloaded functions Matrix 4 --- //
    // Performs a matrix 4(a_mat4) x vector 4(b_vec4) multiplication
    void matrix4_vec4_multi(float a_mat4[], float b_vec4[], float result_arr[], int a_len, int b_len, int result_len);

    // Assumes matrix 4 len = 16 and b_vec4, result_arr len = 4, performs
    // a matrix 4(a_mat4) x vector 4(b_vec4) multiplication
    void matrix4_vec4_multi(float a_mat4[], float b_vec4[], float result_arr[]);  

    // Assumes a, b, and result_arr lens = 16
    // Performs a matrix 4 x matrix 4 calculation
    void matrix4_multi(float a[], float b[], float result_arr[]);
    
    // Calculates inverse of a matrix
    void matrix4_inv(float a[], float result_arr[], int a_len, int result_len);

    // Assumes length are correct. Calculates inverse of a matrix 4
    // - Uses the Minors + Cofactors + Adjugate method
    // - Link: https://www.mathsisfun.com/algebra/matrix-inverse-minors-cofactors-adjugate.html
    void matrix4_inv(float a[], float result_arr[]);

    // Gets minors values from a subset 3x3 matrix with a given row, column
    float matrix4_minors_val(float a[], int row, int col);

    // Performs an in-place transpose on the matrix 4
    void transpose_matrix4(float a[]);

    // --- Matrix 3 --- //
    // Copies contents from matrix 3 to another
    void copy_from_matrix3(float src[], float dest[], int src_len, int dest_len);

    // Copies contents from vector 3 to another
    void copy_from_vec3(float src[], float dest[], int src_len, int dest_len);
    
    // Performs matrix 3 x matrix 3
    // - For matrix 3 x vector calculation, set b to a vector 3, and pass true to is_b_vec
    void matrix3_multi(float a[], float b[], int a_len, int b_len, int result_len ,float result_arr[]);
    
    // Assumes array lengths are correct
    // Performs matrix 3 x matrix 3 / matrix 3 x vector 3 calculation
    // - For matrix 3 x vector calculation, set b to a vector 3, and pass true to is_b_vec
    void matrix3_multi(float a[], float b[], float result_arr[], bool is_b_vec);

    // --- Overloaded functions Matrix 3 --- //
    // Performs matrix 3(a_mat3) x vector 3(b_vec3) multiplication
    void matrix3_vec3_multi(float a_mat3[], float b_vec3[], float result_arr[], int a_len, int b_len, int result_len);

    // Performs matrix 3(a_mat3) x vector 3(b_vec3) multiplication
    // - Assumes a_mat3 len = 9, b_vec3 and result_arr len = 3
    void matrix3_vec3_multi(float a_mat3[], float b_vec3[], float result_arr[]);

    // Performs matrix 3 x matrix 3 multiplication
    // - Assumes a, b, result_arr len = 9
    void matrix3_multi(float a[], float b[], float result_arr[]);

    // Assumes length are correct. Calculates inverse of a matrix 3
    // - Uses the Minors + Cofactors + Adjugate method
    // - Link: https://www.mathsisfun.com/algebra/matrix-inverse-minors-cofactors-adjugate.html
    void matrix3_inv(float a[], float result_arr[]);

    // Returns determinant of a given matrix3
    float determinant_matrix3(float a[]);

    // Gets minors values from a subset 2x2 matrix with a given row, column
    float matrix3_minors_val(float a[], int row, int col);

    // Performs an in-place transpose on the matrix 3
    void transpose_matrix3(float a[]);


    // --- Vectors --- 
    
    // Multiple row and column vector
    void multiply_vectors(float row_vector[], float col_vector[], float res[], int length, int result_length);
    
    // Add vectors
    void add_vectors(float a[], float b[], float res[], int length);

    // Subtract vectors
    void subtract_vectors(float a[], float b[], float res[], int length);

    // Normalizes vector a to length 1
    void normalize_vector(float a[], int a_length);

    // Calculates the vector magnitude
    float vector_magnitude(float a[], int a_length);
    
    // Calculates the dot product between vectors a and b, returns angle in radians
    // - a and b must be both of same length
    float dot_product(float a[], float b[], int a_length, int b_length);

    // Calculates the cross product between vectors a and b (supports vector 3s only)
    // - a and b must be both of same length
    void cross_product_vec3(float a[], float b[], int a_length, int b_length, float result[]);

    // --- Printers 
    void print_mat4(float a[], int a_length);
    void print_mat3(float a[], int a_length);
    void print_vector(float a[], int a_length);


    // TODO: Create a function to return raw pointer data, instead of std::array
    class LinStruct {
        LinStruct();
    public:
        // virtual ~LinStruct() = 0;
        virtual void print() = 0;
        int const dimension;
        int const size;
    protected:
        LinStruct(int dim, int sz) : dimension(dim), size(sz) {};
    };

    class DoubleDimension {
        int row;
        float* arr;
        DoubleDimension();
    public:
        DoubleDimension(int _row, float _arr[]) : row(_row), arr(_arr) {};
        float& operator[] (int);
    };

    // TODO
    // 3. Implement normalize(), subtract() operator, magnitude, cross(), dot() for Vec3, Vec4
    // 4. Implement ostreams for all LinStructs
    // 5. Test

    class Mat4 : public LinStruct {
        std::array<float, 16> values;
    public:
        Mat4();
        Mat4(float[], int);
        Mat4(std::array<float, 16>);
        Mat4 inverse();
        Mat4 transpose();
        DoubleDimension operator[] (int);
        Mat4& operator = (const Mat4&);
        Mat4 operator * (const Mat4&);
        Mat4 operator + (const Mat4&);
        bool operator == (const Mat4&);
        operator float * const () { return values.data(); }
        std::array<float, 16> data() const { return values; };
        void print();
    };

    class Mat3 : public LinStruct {
        std::array<float, 9> values;
    public:
        Mat3();
        Mat3(float[], int);
        Mat3(std::array<float, 9>);
        Mat3 inverse();
        Mat3 transpose();
        DoubleDimension operator[] (int);
        Mat3& operator = (const Mat3&);
        Mat3 operator * (const Mat3&);
        Mat3 operator + (const Mat3&);
        bool operator == (const Mat3&);
        operator float* const () { return values.data(); }
        std::array<float, 9> data() const { return values; }
        void print();
    };

    class Vec3 : public LinStruct {
        std::array<float, 3> values;
    public:
        Vec3();
        Vec3(float[], int);
        Vec3(std::array<float, 3>);
        Vec3 normalize();
        Vec3 cross(const Vec3&);
        float dot(const Vec3&);
        float magnitude();
        float& operator[] (int);
        Vec3& operator = (const Vec3&);
        Mat3 operator * (const Vec3&);
        Vec3 operator + (const Vec3&);
        Vec3 operator - (const Vec3&);
        bool operator == (const Vec3&);
        // Returns a vector 4 version, defaults z to 1.0f
        operator float* const () { return values.data(); }
        std::array<float, 3> data() const { return values; }
        void print();
    };

    class Vec4 : public LinStruct {
        std::array<float, 4> values;
    public:
        Vec4();
        Vec4(float[], int);
        Vec4(std::array<float, 4>);
        Vec4 normalize();
        Vec4 cross_vec4(const Vec4&);
        float dot(const Vec4&);
        float magnitude();
        float& operator[] (int);
        Vec4& operator = (const Vec4&);
        Mat4 operator * (const Vec4&);
        Vec4 operator + (const Vec4&);
        Vec4 operator - (const Vec4&);
        bool operator == (const Vec4&);
        operator float* const () { return values.data(); }
        std::array<float, 4> data() const { return values; }
        void print();
    };

    // Vector - Matrix Multiplications
    Vec4 operator * (const Mat4& left_matrix, const Vec4& col_vector);
    Vec4 operator * (const Vec4& row_vector, const Mat4& right_matrix);

    Vec3 operator * (const Mat3& left_matrix, const Vec3& right_vector);
    Vec3 operator * (const Vec3& left_vector, const Mat3& right_matrix);
};

#endif 