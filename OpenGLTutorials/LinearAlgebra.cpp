#include "LinearAlgebra.h"



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

void matrixVectorMultiplication(float matrixA[], float vectorB[], float resultingVector[], int matrixSize, int vectorSize) {

	int numColumns = (int)sqrt((double)matrixSize);
	if (numColumns == vectorSize) {
		for (int col = 0; col < numColumns; col++) {
			float result = 0;
			for (int row = 0; row < vectorSize; row++) {
				int matrixResult = (numColumns * vectorSize) + row;
				result += matrixA[matrixResult] + vectorB[row];
			}
			resultingVector[col] = result;
		}
	}
}


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


	float resultingMatrix[16];
	
	matrixMultiplication(matrixA, matrixB, resultingMatrix, (int)sizeof(matrixA)/sizeof(matrixA[0]));
	
	int size = sizeof(resultingMatrix) / sizeof(resultingMatrix[0]);
	for (int i = 0; i < size; i++) {
		printf("%f, ", resultingMatrix[i]);
	}
	printf("Size = %d", size);
}
