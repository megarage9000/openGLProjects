#include "LinearAlgebra.h"


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

	float resultingMatrixB[16];


	size = sizeof(matrixA) / sizeof(matrixA[0]);
	matrixMultiplication(matrixA, matrixB, resultingMatrixB, size);


	for (int i = 0; i < size; i++) {
		printf("%f, ", resultingMatrixB[i]);
	}
	printf("Size = %d\n", size);

	//float resultVector[4];

	//matrixVectorMultiplication(matrixA, vectorA, resultVector, (int)sizeof(matrixA) / sizeof(matrixA[0]), (int)sizeof(vectorA) / sizeof(vectorA[0]));

	//int size = sizeof(vectorA) / sizeof(vectorA[0]);
	//for (int i = 0; i < size; i++) {
	//	printf("%f, ", resultVector[i]);
	//}
	//printf("Size = %d", size);


}
