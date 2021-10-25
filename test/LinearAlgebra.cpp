#include "LinearAlgebra.h"

float IDENTITY_4[16] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};

float IDENTITY_3[9] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

void swap(float &a, float &b) {
    float temp = a;
    a = b;
    b=  temp;
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

    void matrix4_multi(float a[], float b[], int a_len, int b_len, int result_len, float result_arr[]) {
        if(a_len == b_len && a_len == result_len && a_len == 16){
            matrix4_multi(a, b, result_arr);
        }
        else {
            copy_from_matrix4(IDENTITY_4, result_arr, 16, result_len);
        }
    }
    void matrix4_multi(float a[], float b[], float result_arr[]){
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

    void matrix4_inv(float a[], float result_arr[], int a_len, int result_len) {
        if(a_len == result_len && a_len == 16){
            matrix4_inv(a, result_arr);
        }
        else{
            copy_from_matrix4(IDENTITY_4, result_arr, 16, result_len);
        }
    }
    void matrix4_inv(float a[], float result_arr[]){
        float det_a = a[0] * matrix4_cofact_val(a, 0, 0);
        float det_b = a[1] * matrix4_cofact_val(a, 0, 1);
        float det_c = a[2] * matrix4_cofact_val(a, 0, 2);
        float det_d = a[3] * matrix4_cofact_val(a, 0, 3);
        float det = det_a - det_b + det_c - det_d;
        if(det != 0.0) {
            float inv_det = 1/det;

            // Get Cofactors
            result_arr[0] = (det_a / a[0]) * inv_det;
            result_arr[1] = -(det_b / a[1]) * inv_det;
            result_arr[2] = (det_c / a[2]) * inv_det;
            result_arr[3] = -(det_d / a[3]) * inv_det;

            bool isEven = false;
            for(int row = 1; row < 4; row++) {
                for(int col = 0; col < 4; col++){
                    int pos = row * 4 + col;
                    result_arr[pos] = matrix4_cofact_val(a, row, col) * inv_det;
                    // Apply Cofactor signage
                    int logical_col = col + 1;
                    if((logical_col % 2 == 0 && isEven) || (logical_col % 2 != 0 && !isEven)){
                        result_arr[pos] = -result_arr[pos];
                    }
                }
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

    float matrix4_cofact_val(float a[], int row, int col) {
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
        if(result == 0){
            return 0;
        }
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
    float determinant_matrix3(float a[]) {
        return a[0] * (a[4] * a[8] - a[5] * a[7]) -
            a[1] * (a[3] * a[8] - a[5] * a[6]) +
            a[2] * (a[3] * a[7] - a[4] * a[6]);
    }

    void matrix3_multi(float a[], float b[], int a_len, int b_len, int result_len ,float result_arr[]){
        if(a_len == b_len && a_len == result_len && a_len == 9) {
            matrix3_multi(a, b, result_arr);
        }
        else {
            copy_from_matrix3(IDENTITY_3, result_arr, 16, result_len);
        }
    }
    void matrix3_multi(float a[], float b[], float result_arr[]) {
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