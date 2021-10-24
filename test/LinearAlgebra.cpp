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
    void matrix4_multi(float a[], float b[], int a_len, int b_len, float result_arr[]) {
        if(a_len == b_len && a_len == 16){
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
}