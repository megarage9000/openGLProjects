#include "LinearAlgebra.h"

// Identity Matrix 4
float IDENTITY_4[16] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};

// Identity Matrix 3
float IDENTITY_3[9] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

// Vector 4 of magnitude 1
float MAG_1_VEC4[4] = {
    1.0f, 1.0f, 1.0f, 1.0f
};

// Vector 3 of magnitude 1
float MAG_1_VEC3[3] = {
    1.0f, 1.0f, 1.0f
};

// Private helper function
void swap(float &a, float &b) {
    float temp = a;
    a = b;
    b=  temp;
}

bool float_equals(float a, float b){
    return fabs(a - b) < EPISLON;
}

namespace LinearAlgebra 
{
    void copy_from_matrix4(float src[], float dest[], int src_len, int dest_len) {
        if(src_len == dest_len && src_len == 16) {
            for(int i = 0; i < 16; i++) {
                dest[i] = src[i];
            }
        }
    }

    void copy_from_vec4(float src[], float dest[], int src_len, int dest_len) {
        if(src_len == dest_len && src_len == 4) {
            for(int i = 0; i < 4; i++) {
                dest[i] = src[i];
            }
        }
    }

    void matrix4_multi(float a[], float b[], int a_len, int b_len, int result_len, float result_arr[]) {
        if(a_len == b_len  && b_len == result_len) {
            matrix4_multi(a, b, result_arr, false);
        }
        else {
            copy_from_matrix4(IDENTITY_4, result_arr, 16, result_len);
        }
    }
    void matrix4_multi(float a[], float b[], float result_arr[], bool is_b_vec){
        // Vector multiplication
        if(is_b_vec) {
            for(int i = 0; i < 4; i++) {
                result_arr[i] = a[i * 4] * b[0] +
                        a[i * 4 + 1] * b[1] +
                        a[i * 4 + 2] * b[2] +
                        a[i * 4 + 3] * b[3];
            }
        }
        // Matrix multiplication
        else {
            for(int row = 0; row < 4; row++){
                for(int col = 0; col < 4; col++) {
                    int pos = (row * 4) + col;
                    int start_a = row * 4;
                    int start_b = col;
                    result_arr[pos] = a[start_a] * b[start_b] + 
                        a[start_a + 1] * b[start_b + 4] +
                        a[start_a + 2] * b[start_b + 8] +
                        a[start_a + 3] * b[start_b + 12];
                }
            }
        }
    }

    // Overloaded functions
    void matrix4_vec4_multi(float a_mat4[], float b_vec4[], float result_arr[], int a_len, int b_len, int result_len) {
        if(a_len == 16 && b_len == result_len && b_len == 4) {
            matrix4_multi(a_mat4, b_vec4, result_arr, true);
        }
        else {
            copy_from_vec4(MAG_1_VEC4, result_arr, 4, result_len);
        }
    }

    void matrix4_vec4_multi(float a_mat4[], float b_vec4[], float result_arr[]){
        matrix4_multi(a_mat4, b_vec4, result_arr, true);
    }

    void matrix4_multi(float a[], float b[], float result_arr[]) {
        matrix4_multi(a, b, result_arr, false);
    }

    void matrix4_inv(float a[], float result_arr[], int a_len, int result_len) {
        if(a_len == result_len && a_len == 16){
            matrix4_inv(a, result_arr);
        }
        else{
            copy_from_matrix4(IDENTITY_4, result_arr, 16, result_len);
        }
    }
    void matrix4_inv(float a[], float result_arr[]){

        // Finding determinant using Laplace method
        // - |M| = aM1 - bM2 + cM3 - dM4
        // - Link: https://www.statlect.com/matrix-algebra/Laplace-expansion-minors-cofactors-adjoints
        float det_a = a[0] * matrix4_minors_val(a, 0, 0);
        float det_b = a[1] * matrix4_minors_val(a, 0, 1);
        float det_c = a[2] * matrix4_minors_val(a, 0, 2);
        float det_d = a[3] * matrix4_minors_val(a, 0, 3);
        float det = det_a - det_b + det_c - det_d;

        // Determines if the following method is a matrix via determinant
        if(float_equals(det, 0.0) == false) {

            // 3. Apply the 1 / determinant (see later in loop)
            float inv_det = 1/det;

            // Reuse the top row determinant values instead of including them
            result_arr[0] = (det_a / a[0]) * inv_det;
            result_arr[1] = -(det_b / a[1]) * inv_det;
            result_arr[2] = (det_c / a[2]) * inv_det;
            result_arr[3] = -(det_d / a[3]) * inv_det;

            // For the cofactor step
            bool isEven = false;
            for(int row = 1; row < 4; row++) {
                for(int col = 0; col < 4; col++){
                    
                    // 1. Get the Minors value per position
                    int pos = row * 4 + col;
                    result_arr[pos] = matrix4_minors_val(a, row, col) * inv_det;

                    // 2. Apply Cofactor signage
                    // - since we are using 0 index, add 1
                    int logical_col = col + 1;
                    if((logical_col % 2 == 0 && isEven) || (logical_col % 2 != 0 && !isEven)){
                        result_arr[pos] = -result_arr[pos];
                    }
                }
                // Alternate for C
                isEven = !isEven;
            }
            // Get Adjugate(Transpose result)
            transpose_matrix4(result_arr);
        }
        else {
            copy_from_matrix4(IDENTITY_4, result_arr, 16, 16);
        }   
    }

    /*
        0  1  2  3 
        4  5  6  7
        8  9  10 11
        12 13 14 15
    */
    void transpose_matrix4(float a[]) {
        // positions 0, 5, 10, 15 are not needed to move
        // swap 4 and 1
        swap(a[4], a[1]);
        // swap 8 and 2
        swap(a[8], a[2]);
        // swap 9 and 6
        swap(a[9], a[6]);
        // swap 12 and 3
        swap(a[12], a[3]);   
        // swap 13 and 7
        swap(a[13], a[7]);
        // swap 14 and 11
        swap(a[14], a[11]);
    }

    float matrix4_minors_val(float a[], int row, int col) {
        float cofact[9];
        int cofact_ind = 0;
        // Find ways to improve this!
        for(int i = 0; i < 4; i++) {
            if(i == row) {
                continue;
            }
            for(int j = 0; j < 4; j++) {
                if(j == col) {
                    continue;
                }
                cofact[cofact_ind] = a[(i * 4) + j];
                cofact_ind++;
            }
        }
        float result = determinant_matrix3(cofact);

        // Handles -0's, kinda of annoying
        // if(result == 0){
        //     return 0;
        // }
        return result;
    }



    // --- Matrix 3 --- //
       /*
        0 1 2
        3 4 5
        6 7 8
    */

    void copy_from_matrix3(float src[], float dest[], int src_len, int dest_len) {
        if(src_len == dest_len && src_len == 9) {
            for(int i = 0; i < 9; i++) {
                dest[i] = src[i];
            }
        }
    }
    void copy_from_vec3(float src[], float dest[], int src_len, int dest_len) {
        if(src_len == dest_len && src_len == 3) {
            for(int i = 0; i < 3; i++) {
                src[i] = dest[i];
            }
        }
    }
    float determinant_matrix3(float a[]) {
        return a[0] * (a[4] * a[8] - a[5] * a[7]) -
            a[1] * (a[3] * a[8] - a[5] * a[6]) +
            a[2] * (a[3] * a[7] - a[4] * a[6]);
    }

    void matrix3_multi(float a[], float b[], int a_len, int b_len, int result_len ,float result_arr[]){
        if(a_len == b_len && b_len == result_len) {
            matrix3_multi(a, b, result_arr, false);
        }
        else {
            copy_from_matrix3(IDENTITY_3, result_arr, 16, result_len);
        }
    }
    void matrix3_multi(float a[], float b[], float result_arr[], bool is_b_vec) {
        if(is_b_vec) {
            for(int i = 0; i < 3; i++) {
                result_arr[i] = a[i * 3] * b[0] +
                        a[i * 3 + 1] * b[1] +
                        a[i * 3 + 2] * b[2];
            }
        }
        else {
            for(int row = 0; row < 3; row++){
                for(int col = 0; col < 3; col++) {
                    int pos = (row * 3) + col;
                    int start_a = row * 3;
                    int start_b = col;
                    result_arr[pos] = a[start_a] * b[start_b] + 
                        a[start_a + 1] * b[start_b + 3] +
                        a[start_a + 2] * b[start_b + 6];
                }
            }
        }
    }

    void matrix3_vec3_multi(float a_mat3[], float b_vec3[], float result_arr[], int a_len, int b_len, int result_len){
        if(a_len / 3 == b_len && b_len == result_len && result_len == 3) {
            matrix3_multi(a_mat3, b_vec3, result_arr, true);
        }
        else {
            for(int i = 0; i < 3; i++) {
                result_arr[i] = MAG_1_VEC3[i];
            }
        }
    }

    void matrix3_vec3_multi(float a_mat3[], float b_vec3[], float result_arr[]){
        matrix3_multi(a_mat3, b_vec3, result_arr, true);
    }

    void matrix3_multi(float a[], float b[], float result_arr[]) {
        matrix3_multi(a, b, result_arr, false);
    }

    void matrix3_inv(float a[], float result_arr[]) {
        
    }
}