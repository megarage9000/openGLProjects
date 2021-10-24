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


namespace LinearAlgebra {

    void matrix4_multi(float a[], float b[], int a_len, int b_len, int result_len, float result_arr[]) {
        if(a_len == b_len && a_len == result_len && a_len == 16){
            matrix4_multi(a, b, result_arr);
        }
        else {
            for(int i = 0;  i < 16; i++) {
                result_arr[i] = IDENTITY_4[i];
            }
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
            for(int i = 0;  i < 16; i++) {
                result_arr[i] = IDENTITY_4[i];
            }
        }
    }
    void matrix4_inv(float a[], float result_arr[]){
        // Cofactors

    }

    

    // --- Matrix 3 --- //
    void matrix3_multi(float a[], float b[], int a_len, int b_len, int result_len ,float result_arr[]){
        if(a_len == b_len && a_len == result_len && a_len == 9) {
            matrix3_multi(a, b, result_arr);
        }
        else {
            for(int i = 0; i < 9; i++) {
                result_arr[i] = IDENTITY_3[i];
            }
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

    /*
        0 1 2
        3 4 5
        6 7 8
    */
    float determinant_mat_3(float a[]) {
        return a[0] * (a[4] * a[8] - a[5] * a[7]) -
            a[1] * (a[3] * a[8] - a[5] * a[6]) +
            a[2] * (a[3] * a[7] - a[4] * a[6]);
    }
}