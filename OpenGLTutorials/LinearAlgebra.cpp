#include "LinearAlgebra.h"


// --- LINEAR ALGEBRA METHODS --- //

// Square matrices only 
void matrixMultiplication(float matrixA[], float matrixB[], float resultingMatrix[], int matrixSize) {
	int index = 0;
	int length = (int)sqrt((double)matrixSize);

	for (int rowA = 0; rowA < length; rowA++) {
		for (int colB = 0; colB < length; colB++) {
			float result = 0;
			for (int i = 0; i < length; i++) {
				int columnResult = colB + (i * length);
				int rowResult = (rowA * length) + i;
				result += matrixA[rowResult] * matrixB[columnResult];
			}
			resultingMatrix[index] = result;
			index++;
		}
	}
}

void matrixMultiplication(float matrixA[], float matrixB[], float resultingMatrix[], int matrixARowSize, int matrixBColSize) {

	if (matrixARowSize == matrixBColSize) {
		int index = 0;
		int numSum = matrixARowSize;
		for (int rowA = 0; rowA < matrixARowSize; rowA++) {
			for (int colB = 0; colB < matrixBColSize; colB++) {
				float result = 0;
				for (int i = 0; i < numSum; i++) {
					int colBIndex = colB + (matrixBColSize * i);
					int rowAIndex = (rowA * matrixARowSize) ;
					result += matrixA[rowAIndex] * matrixB[colBIndex];
				}
				resultingMatrix[index] = result;
				index++;
			}
		}
	}
}


void matrixVectorMultiplication(float matrixA[], float vectorB[], float resultingVector[], int matrixSize, int vectorSize) {
	int numColumns = (int)sqrt((double)matrixSize);
	if (numColumns == vectorSize) {
		for (int col = 0; col < numColumns; col++) {
			float result = 0;
			for (int row = 0; row < vectorSize; row++) {
				int matrixResult = (col * vectorSize) + row;
				result += matrixA[matrixResult] * vectorB[row];
			}
			resultingVector[col] = result;
		}
	}
}

// --- LINEAR ALGEBRA METHODS --- //

// --- MATRIX METHODS --- //
matrix4 createMatrix4() {
	matrix4 resultMatrix = {
		4,
		16,
		{
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
		}
	};
	return resultMatrix;
}

matrix4 createMatrix4(float values[], int valuesSize) {
	if (valuesSize == 16) {
		matrix4 resultMatrix = {
			4,
			16,
			{
				values[0], values[1], values[2], values[3],
				values[4], values[5], values[6], values[7],
				values[8], values[9], values[10], values[11],
				values[12], values[13], values[14], values[15]
			}
		};
		return resultMatrix;
	}
	else {
		return createMatrix4();
	}
}

void transposeMatrix4(matrix4* matrix) {
	float * values = matrix->values;
	float newValues[] = {
		values[0], values[4], values[8],  values[12],
		values[1], values[5], values[9],  values[13],
		values[2], values[6], values[10], values[14],
		values[3], values[7], values[11], values[15]
	};

	for (int i = 0; i < 16; i++) {
		values[i] = newValues[i];
	}
}

void printMatrix4(matrix4 matrix) {
	float * values = matrix.values;
	printf("| %f %f %f %f |\n| %f %f %f %f |\n| %f %f %f %f |\n| %f %f %f %f |\n",
		values[0], values[1], values[2], values[3],
		values[4], values[5], values[6], values[7],
		values[8], values[9], values[10], values[11],
		values[12], values[13], values[14], values[15]);
}

matrix4 matrix4Multiplication(matrix4 matrixA, matrix4 matrixB) {
	float resultingValues[16];
	matrixMultiplication(matrixA.values, matrixB.values, resultingValues, matrixA.dimension, matrixB.dimension);
	return createMatrix4(resultingValues, 16);
}


// --- MATRIX METHODS --- //



void test() {
	float matrixA[] = {
		2.0f, 2.0f, 2.0f, 2.0f,
		3.0f, 3.0f, 3.0f, 3.0f,
		4.0f, 4.0f, 4.0f, 4.0f,
		5.0f, 5.0f, 5.0f, 5.0f
	};

	float matrixB[] = {
		3.0f, 3.0f, 3.0f, 3.0f,
		6.0f, 6.0f, 6.0f, 6.0f,
		7.0f, 7.0f, 7.0f, 7.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};

	float vectorA[] = {
		3.0f, 7.0f, 5.0f, 1.0f
	};


	float resultingMatrix[16];

	matrixMultiplication(matrixA, matrixB, resultingMatrix, 4, 4);

	int size = sizeof(resultingMatrix) / sizeof(resultingMatrix[0]);
	for (int i = 0; i < size; i++) {
		printf("%f, ", resultingMatrix[i]);
	}
	printf("Size = %d\n", size);

	//float resultingMatrixB[16];


	//size = sizeof(matrixA) / sizeof(matrixA[0]);
	//matrixMultiplication(matrixA, matrixB, resultingMatrixB, size);


	//for (int i = 0; i < size; i++) {
	//	printf("%f, ", resultingMatrixB[i]);
	//}
	//printf("Size = %d\n", size);

	//float resultVector[4];

	//matrixVectorMultiplication(matrixA, vectorA, resultVector, (int)sizeof(matrixA) / sizeof(matrixA[0]), (int)sizeof(vectorA) / sizeof(vectorA[0]));

	//int size = sizeof(vectorA) / sizeof(vectorA[0]);
	//for (int i = 0; i < size; i++) {
	//	printf("%f, ", resultVector[i]);
	//}
	//printf("Size = %d", size);

	matrix4 AMatrix = createMatrix4(matrixA, sizeof(matrixA) / sizeof(matrixA[0]));
	matrix4 BMatrix = createMatrix4(matrixB, sizeof(matrixB) / sizeof(matrixB[0]));
	matrix4 resultMatrix = matrix4Multiplication(AMatrix, BMatrix);

	printMatrix4(resultMatrix);
	printf("\n");
	transposeMatrix4(&resultMatrix);
	printMatrix4(resultMatrix);
	printf("\n");
	transposeMatrix4(&resultMatrix);
	printMatrix4(resultMatrix);
	printf("\n");
	printMatrix4(IDENTITY_MATRIX_4);
}