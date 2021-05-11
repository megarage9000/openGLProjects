#pragma once
#ifndef LINEAR_ALGEBRA_H
#define LINEAR_ALGEBRA_H

#include <stdio.h>
#include <math.h>

void matrixMultiplication(float matrixA[], float matrixB[], float resultingMatrix[], int size);
void matrixMultiplication(float matrixA[], float matrixB[], float resultingMatrix[], int matrixARowSize, int matrixBColSize);
void matrixVectorMultiplication(float matrixA[], float vectorB[], float resultingVector[], int matrixSize, int vectorSize);
void test();


// --- MATRIX 4 --- //
typedef struct matrix4Struct {
	int dimension;
	int valuesSize;
	float values[16];
} matrix4;

const matrix4 IDENTITY_MATRIX_4 = {
	4, 
	16,
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	}
};

matrix4 createMatrix4();
matrix4 createMatrix4(float values[], int valuesSize);
matrix4 matrix4Multiplication(matrix4 matrixA, matrix4 matrixB);
void transposeMatrix4(matrix4* matrix);
void printMatrix4(matrix4 matrix);
float determinantMatrix4(matrix4 matrix);
// --- MATRIX 4 --- //


typedef struct matrix3Struct {
	int dimension;
	float values[3];
} matrix3;

matrix3 createMatrix3();
matrix3 createMatrix3(float values[], int valuesSize);

//TODO 
/*
	1. Work on matrix3 methods and determinant method
	2. Work on matrix4 determinant and inverse method
	3. Add another class to introduce transformations
*/

#endif LINEAR_ALGEBRA_H