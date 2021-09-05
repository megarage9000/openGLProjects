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
					int rowAIndex = (rowA * matrixARowSize) + i;
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
// -- Matrix 4
matrix4 createMatrix4() {
	matrix4 resultMatrix = {
		4,
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

	memcpy(values, newValues, 16 * sizeof(float));
}


// Look at this site: http://www.euclideanspace.com/maths/algebra/matrix/functions/determinant/fourD/index.htm 
// To work on determinant
float determinantMatrix4(matrix4 matrix) {
	float* values = matrix.values;
	
	matrix3 M1 = createMatrix3(new float[] {
		values[5], values[6], values[7],
		values[9], values[10], values[11],
		values[13], values[14], values[15]
	}, 9);

	matrix3 M2 = createMatrix3(new float[] {
		values[4], values[6], values[7],
			values[8], values[10], values[11],
			values[12], values[14], values[15]
		}, 9);

	matrix3 M3 = createMatrix3(new float[] {
		values[4], values[5], values[7],
			values[8], values[9], values[11],
			values[12], values[13], values[15]
		}, 9);

	matrix3 M4 = createMatrix3(new float[] {
		values[4], values[5], values[6],
			values[8], values[9], values[10],
			values[12], values[13], values[14]
		}, 9);

	return values[0] * determinantMatrix3(M1) 
		- values[1] * determinantMatrix3(M2)
		+ values[2] * determinantMatrix3(M3)
		- values[3] * determinantMatrix3(M4);
}

	

matrix4 matrix4Multiplication(matrix4 matrixA, matrix4 matrixB) {
	float resultingValues[16];
	matrixMultiplication(matrixA.values, matrixB.values, resultingValues, matrixA.dimension, matrixB.dimension);
	return createMatrix4(resultingValues, 16);
}


// Debugging matrix 4 operations
void printMatrix4(matrix4 matrix) {
	float * values = matrix.values;
	printf("| %f %f %f %f |\n| %f %f %f %f |\n| %f %f %f %f |\n| %f %f %f %f |\n",
		values[0], values[1], values[2], values[3],
		values[4], values[5], values[6], values[7],
		values[8], values[9], values[10], values[11],
		values[12], values[13], values[14], values[15]); 
}


bool areEqualMatrix4(matrix4 matrixA, matrix4 matrixB) {
	float* valuesA = matrixA.values;
	float* valuesB = matrixB.values;

	for (int i = 0; i < 16; i += 4) {
		float a1 = valuesA[i];
		float a2 = valuesA[i + 1];
		float a3 = valuesA[i + 2];
		float a4 = valuesA[i + 3];

		float b1 = valuesB[i];
		float b2 = valuesB[i + 1];
		float b3 = valuesB[i + 2];
		float b4 = valuesB[i + 3];

		if (!(a1 == b1 && a2 == b2 && a3 == b3 && a4 == b4)) {
			return false;
		}
	}
	return true;
}

// -- Matrix3
matrix3 createMatrix3() {
	matrix3 resultMatrix = {
		3,
		{
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f
		}
	};
	return resultMatrix;
}

matrix3 createMatrix3(float values[], int valuesSize) {
	if (valuesSize == 9) {
		matrix3 matrix = {
			3,
			{
				values[0], values[1], values[2],
				values[3], values[4], values[5],
				values[6], values[7], values[8]
			}
		};
		return matrix;
	}
	return createMatrix3();

}

float determinantMatrix3(matrix3 matrix) {
	float* values = matrix.values;
	return (values[0] * values[4] * values[8])	// aei
		+ (values[1] * values[5] * values[6])	// bfg
		+ (values[2] * values[3] * values[7])	// cdh
		- (values[2] * values[4] * values[6])	// ceg
		- (values[1] * values[3] * values[8])	// bdi
		- (values[0] * values[5] * values[7]);	// afh
}

// --- MATRIX METHODS --- //

