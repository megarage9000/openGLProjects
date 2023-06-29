#include "../.h/LinearAlgebra.h"


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
    /*
        Classes
    */
    // Double Dimension
    float& DoubleDimension::operator[] (int col) {
        return arr[row + col];
    }

    // ---- Matrix 4 -----
    Mat4::Mat4() : LinStruct(4, 16) { 
        copy_from_matrix4(IDENTITY_4, values.data(), 16, 16);
    }

    Mat4::Mat4(float _values[], int _size) : LinStruct(4, 16) {
        assert(_size == 16);
        for (int i = 0; i < size; i++) {
            values[i] = _values[i];
        }
    }

    Mat4::Mat4(std::array<float, 16> _values) : LinStruct(4, 16) {
        values = _values;
    }

    Mat4 Mat4::inverse() {
        std::array<float, 16> result_arr;
        matrix4_inv(values.data(), result_arr.data());
        return Mat4(result_arr);
    }

    Mat4 Mat4::transpose() {
        std::array<float, 16> result_arr = values;
        transpose_matrix4(result_arr.data());
        return Mat4(result_arr);
    }

    DoubleDimension Mat4::operator[] (int row) {
        assert(row >= 0 && row < dimension);
        return DoubleDimension(row * dimension, values.data());
    }

    // Matrix 4 multis
    Mat4& Mat4::operator = (const Mat4& other_matrix) {
        if (this != &other_matrix) {
            values = other_matrix.values;
        }
        return *this;
    }

    Mat4 Mat4::operator * (const Mat4& other_matrix) {
        std::array<float, 16> new_arr;
        std::array<float, 16> other_arr = other_matrix.data();
        matrix4_multi(values.data(), other_arr.data(), new_arr.data(), false);
        return Mat4(new_arr);
    }

    // Vector / Matrix 4 multis
    Vec4 operator * (const Mat4& left_matrix, const Vec4& row_vector) {
        std::array<float, 4> new_arr;
        std::array<float, 16> left_arr = left_matrix.data();
        std::array<float, 4> right_arr = row_vector.data();
        matrix4_multi(left_arr.data(), right_arr.data(), new_arr.data(), true);
        return Vec4(new_arr);
    }

    Vec4 operator * (const Vec4& row_vector, const Mat4& right_matrix) {
        std::array<float, 4> new_arr;
        std::array<float, 16> left_arr = right_matrix.data();
        std::array<float, 4> right_arr = row_vector.data();
        matrix4_multi(left_arr.data(), right_arr.data(), new_arr.data(), true);
        return Vec4(new_arr);
    }
    

    Mat4 Mat4::operator + (const Mat4& other_matrix) {
        std::array<float, 16> new_arr;
        for (int i = 0; i < size; i++) {
            new_arr[i] = values[i] + other_matrix.values[i];
        }
        return Mat4(new_arr);
    }

    bool Mat4::operator == (const Mat4& other_matrix) {
        return values == other_matrix.data();
    }

    void Mat4::print() {
        std::cout << "--------------------------------\n";
        std::cout << std::left;
        std::cout << std::setprecision(2);
        for (int y = 0; y < dimension; y++) {
            for (int x = 0; x < dimension; x++) {
                std::cout << std::setw(16) << std::setprecision(5) << (*this)[y][x];
            }
            std::cout << '\n';
        }
        std::cout << "--------------------------------\n";
    }

    // ---- Matrix 3 -----

    Mat3::Mat3() : LinStruct(3, 9) {
        copy_from_matrix3(IDENTITY_3, values.data(), 9, values.size());
    }

    Mat3::Mat3(float _values[], int _size) : LinStruct(3, 9) {
        assert(_size == 9);
        for (int i = 0; i < size; i++) {
            values[i] = _values[i];
        }
    }
   

    Mat3::Mat3(std::array<float, 9> _values) : LinStruct(3, 9) {
        values = _values;
    }

    Mat3 Mat3::inverse() {
        std::array<float, 9> result_arr;
        matrix3_inv(values.data(), result_arr.data());
        return Mat3(result_arr);
    }

    Mat3 Mat3::transpose() {
        std::array<float, 9> result_arr = values;
        transpose_matrix3(values.data());
        return Mat3(result_arr);
    }

    DoubleDimension Mat3::operator[](int row) {
        assert(row >= 0 && row < dimension);
        return DoubleDimension(row * dimension, values.data());
    }

    Mat3& Mat3::operator = (const Mat3& other_matrix) {
        if (this != &other_matrix) {
            values = other_matrix.data();
        }
        return *this;
    }

    Mat3 Mat3::operator * (const Mat3& other_matrix) {
        std::array<float, 9> new_arr;
        std::array<float, 9> other_arr = other_matrix.data();
        matrix3_multi(values.data(), other_arr.data(), new_arr.data());
        return Mat3(new_arr);
    }

    Vec3 operator * (const Mat3& left_matrix, const Vec3& right_vector) {
        std::array<float, 3> new_arr;
        std::array<float, 9> left_arr = left_matrix.data();
        std::array<float, 3> right_arr = right_vector.data();
        matrix3_multi(left_arr.data(), right_arr.data(), new_arr.data(), true);
        return Vec3(new_arr);
    }

    Vec3 operator * (const Vec3& left_vector, const Mat3& right_matrix) {
        std::array<float, 3> new_arr;
        std::array<float, 9> left_arr = right_matrix.data();
        std::array<float, 3> right_arr = left_vector.data();
        matrix3_multi(left_arr.data(), right_arr.data(), new_arr.data(), true);
        return Vec3(new_arr);
    }

    Mat3 Mat3::operator + (const Mat3& other_matrix) {
        std::array<float, 9> new_arr;
        for (int i = 0; i < size; i++) {
            new_arr[i] = values[i] + other_matrix.values[i];
        }
        return Mat3(new_arr);
    }

    bool Mat3::operator == (const Mat3& other_matrix) {
        return values == other_matrix.data();
    }

    void Mat3::print() {
        std::cout << "--------------------------------\n";
        std::cout << std::left;
        std::cout << std::setprecision(2);
        for (int y = 0; y < dimension; y++) {
            for (int x = 0; x < dimension; x++) {
                std::cout << std::setw(8) << (*this)[y][x];
            }
            std::cout << '\n';
        }
        std::cout << "--------------------------------\n";
    }
    // ---- Vector 4 ----
    
    Vec4::Vec4() : LinStruct(4, 4) {
        copy_from_vec4(MAG_1_VEC4, values.data(), 4, 4);
    }

    Vec4::Vec4(float _values[], int _size) : LinStruct(4, 4) {
        assert(_size == 4);
        std::copy(_values, _values + 4, values.data());
    }

    Vec4::Vec4(std::array<float, 4> _values) : LinStruct(4, 4) {
        values = _values;
    }

    Vec4::Vec4(float x, float y, float z, float q) : LinStruct(4, 4) {
        values[0] = x;
        values[1] = y;
        values[2] = z;
        values[3] = q;
    }

    Vec4::Vec4(Vec3 vec3, float q) : LinStruct(4, 4) {
        values[0] = vec3[0];
        values[1] = vec3[1];
        values[2] = vec3[2];
        values[3] = q;
    }

    Vec4 Vec4::normalize(bool true_normalize) {
        std::array<float, 4> result_arr = values;
        if (true_normalize) {
            // Normalize all components
            normalize_vector(result_arr.data(), 4);
        }
        else {
            // For linear algebra, keep the 1 at the end normal
            normalize_vector(result_arr.data(), 3);
        }
        return Vec4(result_arr);
    }

    Vec4 Vec4::cross_vec4(const Vec4& other) {
        Vec3 this_vec = Vec3();
        this_vec[0] = values[0];
        this_vec[1] = values[1];
        this_vec[2] = values[2];

        Vec3 other_vec = Vec3();
        other_vec[0] = other.data()[0];
        other_vec[1] = other.data()[1];
        other_vec[2] = other.data()[2];

        Vec3 cross_vec3 = this_vec.cross(other_vec);
        Vec4 cross_result = Vec4();
        cross_result[0] = cross_vec3[0];
        cross_result[1] = cross_vec3[1];
        cross_result[2] = cross_vec3[2];

        return cross_result;
    }

    float Vec4::dot(const Vec4& other) {
        return dot_product(values.data(), other.data().data(), 4, 4);
    }

    float Vec4::magnitude() {
        return vector_magnitude(values.data(), 4);
    }

    float& Vec4::operator[] (int index) {
        return values[index];
    }

    Vec4& Vec4::operator = (const Vec4& other_vector) {
        if (this != &other_vector) {
            values = other_vector.data();
        }
        return *this;
    }
   
    Mat4 Vec4::operator * (const Vec4& other_vector) {
        std::array<float, 16> new_arr;
        std::array<float, 4> other_arr = other_vector.data();
        multiply_vectors(values.data(), other_arr.data(), new_arr.data(), size, 16);
        return Mat4(new_arr);
    }

    Vec4 Vec4::operator * (const float& scalar) {
        std::array<float, 4> new_arr;
        for (int i = 0; i < 4; i++) {
            new_arr[i] = values[i] * scalar;
        }
        return Vec4(new_arr);
    }

    Vec4 Vec4::operator + (const Vec4& other_vector) {
        std::array<float, 4> new_arr;
        std::array<float, 4> other_arr = other_vector.data();
        add_vectors(values.data(), other_arr.data(), new_arr.data(), 4);
        return Vec4(new_arr);
    }
    
    Vec4 Vec4::operator - (const Vec4& other_vector) {
        std::array<float, 4> result_arr;
        std::array<float, 4> other_arr = other_vector.data();
        subtract_vectors(values.data(), other_arr.data(), result_arr.data(), 4);
        return Vec4(result_arr);
    }

    bool Vec4::operator == (const Vec4& other_vector) {
        return values == other_vector.data();
    }

    void Vec4::print() {
        std::cout << "\n--------------------------------\n";
        for (int i = 0; i < dimension; i++) {
            std::cout << std::setw(16) << std::setprecision(5) << (*this)[i];
        }
        std::cout << "\n--------------------------------\n";
    }

    // ---- Vec3 ----

    Vec3::Vec3() : LinStruct(3, 3) {
        copy_from_vec3(MAG_1_VEC3, values.data(), 3, 3);
    }

    Vec3::Vec3(float _values[], int _size) : LinStruct(3, 3) {
        assert(_size == 3);
        std::copy(_values, _values + 3, values.data());
    }

    Vec3::Vec3(std::array<float, 3> _values) : LinStruct(3, 3) {
        values = _values;
    }

    Vec3::Vec3(float x, float y, float z) : LinStruct(3, 3) {
        values[0] = x;
        values[1] = y;
        values[2] = z;
    }

    Vec3 Vec3::normalize() {
        std::array<float, 3> result_arr = values;
        normalize_vector(result_arr.data(), 3);
        return Vec3(result_arr);
    }

    Vec3 Vec3::cross(const Vec3& other) {
        std::array<float, 3> result_arr;
        cross_product_vec3(values.data(), other.data().data(), 3, 3, result_arr.data());
        return Vec3(result_arr);
    }

    float Vec3::dot(const Vec3& other) {
        return dot_product(values.data(), other.data().data(), 3, 3);
    }

    float Vec3::magnitude() {
        return vector_magnitude(values.data(), 3);
    }


    float& Vec3::operator [] (int index) {
        return values[index];
    }

    Vec3& Vec3::operator = (const Vec3& other_vector) {
        if (this != &other_vector) {
            values = other_vector.values;
        }
        return *this;
    }

    Mat3 Vec3::operator * (const Vec3& other_vector) {
        std::array<float, 9> new_arr;
        std::array<float, 3> other_arr = other_vector.data();
        multiply_vectors(values.data(), other_arr.data(), new_arr.data(), 3, 9);
        return Mat3(new_arr);
    }

    Vec3 Vec3::operator * (const float& scalar) {
        std::array<float, 3> new_arr;
        for (int i = 0; i < 3; i++) {
            new_arr[i] = values[i] * scalar;
        }
        return Vec3(new_arr);
    }

    Vec3 Vec3::operator + (const Vec3& other_vector) {
        std::array<float, 3> new_arr;
        std::array<float, 3> other_arr = other_vector.data();
        add_vectors(values.data(), other_arr.data(), new_arr.data(), 3);
        return Vec3(new_arr);
    }

    Vec3 Vec3::operator - (const Vec3& other_vector) {
        std::array<float, 3> result_arr;
        std::array<float, 3> other_arr = other_vector.data();
        subtract_vectors(values.data(), other_arr.data(), result_arr.data(), 3);
        return Vec3(result_arr);
    }

    bool Vec3::operator == (const Vec3& other_vector) {
        return values == other_vector.data();
    }
    
    void Vec3::print() {
        std::cout << "\n--------------------------------\n";
        for (int i = 0; i < dimension; i++) {
            std::cout << std::setw(8) << (*this)[i];
        }
        std::cout << "\n--------------------------------\n";
    }

    // ---- Functions -----

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
    void matrix4_multi(float a[], float b[], float result_arr[], bool is_b_vec = false){
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

    // NEED TO REWORK THIS!
    // https://www.codespeedy.com/matrix-inversion-in-cpp/
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
            for(int row = 0; row < 4; row++) {
                for(int col = 0; col < 4; col++){
                    int pos = row * 4 + col;
                    result_arr[pos] = matrix4_minors_val(a, row, col) * inv_det * pow(-1, (row + col + 2));
                } 
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
        float minors[9];
        int minors_ind = 0;
        // Find ways to improve this!
        for(int i = 0; i < 4; i++) {
            if(i == row) {
                continue;
            }
            for(int j = 0; j < 4; j++) {
                if(j == col) {
                    continue;
                }
                minors[minors_ind] = a[(i * 4) + j];
                minors_ind++;
            }
        }
        float result = determinant_matrix3(minors);

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
    void copy_from_vec3(float src[], float dest[], int src_len, int dest_len) {
        if(src_len == dest_len && src_len == 3) {
            for(int i = 0; i < 3; i++) {
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
        // See matrix4_inv for detailed walkthrough

        // Check determinant
        float determinant = determinant_matrix3(a);
        if(float_equals(determinant, 0.0) == false) {
            
            float inv_det = 1.0f / determinant;

            bool isEven = true;
            for(int row = 0; row < 3; row++){
                for(int col = 0; col < 3; col++){
                    
                    int pos = row * 3 + col;
                    result_arr[pos] = inv_det * matrix3_minors_val(a, row, col);

                    int logical_col = col + 1;
                    if((logical_col % 2 == 0 && isEven) || (logical_col % 2 != 0 && !isEven)){
                        result_arr[pos] = -result_arr[pos];
                    }
                }
                isEven = !isEven;
            }

            transpose_matrix3(result_arr);
        }
        else{
            copy_from_matrix3(IDENTITY_3, result_arr, 9, 9);
        }
    }

    float matrix3_minors_val(float a[], int row, int col) {
        float minors_val[4];
        int minors_ind = 0;

        for(int i = 0; i < 3; i++){
            if(i == row){
                continue;
            }

            for(int j = 0; j < 3; j++) {
                if(j == col){
                    continue;
                }
                minors_val[minors_ind] = a[(i * 3) + j];
                minors_ind++;
            }
        }
        return (minors_val[0] * minors_val[3]) - (minors_val[1] * minors_val[2]); 
    }

    /*
        0 1 2
        3 4 5
        6 7 8
    */
    void transpose_matrix3(float a[]) {
        // Don't need to swap 0, 4, and 8
        swap(a[1], a[3]);
        swap(a[2], a[6]);
        swap(a[7], a[5]);
    }

    // --- Vectors ---- 
    void multiply_vectors(float row_vector[], float col_vector[], float res[], int length, int result_length) {
        if (length * length != result_length) {
            for (int i = 0; i < result_length; i++) {
                res[i] = 0.0f;
            }
        }
        else {
            for (int y = 0; y < length; y++) {
                for (int x = 0; x < length; x++) {
                    res[y * length + x] = row_vector[y] * col_vector[x];
                }
            }
        }
    }

    void add_vectors(float a[], float b[], float res[], int length) {
        for(int i = 0; i < length; i++) {
            res[i] = a[i] + b[i];
        }
    }

   
    void subtract_vectors(float a[], float b[], float res[], int length) {
        for(int i = 0; i < length; i++) {
            res[i] = a[i] - b[i];
        }
    }

    void normalize_vector(float a[], int a_length) {
        if(a_length != 0){
            float magnitude = vector_magnitude(a, a_length);
            if (float_equals(magnitude, 0) == false) {
                for(int i = 0; i < a_length; i++){
                    a[i] = a[i] / magnitude;
                }
            }
        }       
    }

    float vector_magnitude(float a[], int a_length) {
        float sumSquares = 0;
        for(int i = 0; i < a_length; i++){
            sumSquares += a[i] * a[i];
        }
        return sqrt(sumSquares);
    }

    float dot_product(float a[], float b[], int a_length, int b_length) {
        if(a_length == b_length){
            float a_mag = vector_magnitude(a, a_length);
            float b_mag = vector_magnitude(b, b_length);

            float a_b_product = 0;

            for(int i = 0; i < a_length; i++) {
                a_b_product += a[i] * b[i];
            }

            // a_b_product = a_b_product / (a_mag * b_mag);
            // return acos(a_b_product);
            return a_b_product;
        }
        return 0;
    }

    void cross_product_vec3(float a[], float b[], int a_length, int b_length, float result[]) {

        if(a_length != 3 || b_length != 3) {
            copy_from_vec3(MAG_1_VEC3, result, 3, 3);
            return;
        }
        result[0] = (a[1] * b[2]) - (a[2] * b[1]);
        result[1] = (a[2] * b[0]) - (a[0] * b[2]);
        result[2] = (a[0] * b[1]) - (a[1] * b[0]);
    }

    void print_mat4(float a[], int a_length) {

        if(a_length != 16) {
            print_mat4(IDENTITY_4, 16);
        }

        std::cout << "--------------------------------\n";
        std::cout << std::left;
        std::cout << std::setprecision(2);
        for(int y = 0; y < 4; y++) {
            for(int x = 0; x < 4; x++) {
                std::cout << std::setw(8) << a[y * 4 + x];
            }
            std::cout << '\n';
        }
        std::cout << "--------------------------------\n";
    }

    void print_mat3(float a[], int a_length) {
        if(a_length != 9) {
            print_mat3(IDENTITY_3, 9);
        }
        std::cout << "--------------------------------\n";
        std::cout << std::left;
        std::cout << std::setprecision(2);
        for(int y = 0; y < 3; y++) {
            for(int x = 0; x < 3; x++) {
                std::cout << std::setw(8) << a[y * 3 + x];
            }
            std::cout << '\n';
        }
        std::cout << "--------------------------------\n";
    }

    void print_vector(float a[], int a_length) {
        std::cout << std::left;
        std::cout << std::setprecision(2);
        for(int i = 0; i < a_length; i++) {
            std::cout << std::setw(8) << a[i];
        }
        std::cout << '\n';
    }

}