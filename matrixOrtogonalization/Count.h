#pragma once
#include <iostream>

void matrixMultiplication(double* first, double* second, int rowLength, int columnLength, int length, double* resultMatrix) {
    int i, j, c;
    double sum;
    for (i = 0; i < length; i++) {
        for (j = 0; j < columnLength; j++) {
            sum = 0;
            for (c = 0; c < rowLength; c++) {
                sum += *(first + j * columnLength + c) * *(second + c * length + i);
            }
            *(resultMatrix + j * length + i) = sum;
        }
    }
}

void numericMultiplication(double* matrix, double number, int size) {
    int i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            *(matrix + i * size + j) = number * *(matrix + i * size + j);
        }
    }
}

double vectorMultiplication(double* r, double* a, int size, int R, int A) {
    int i;
    double numerator = 0, denominator = 0, result;
    for (i = 0; i < size; i++) {
        numerator += *(a + i * size + A) * *(r + i * size + R);
        denominator += *(r + i * size + R) * *(r + i * size + R);
    }
    result = numerator / denominator;
    return result;
}

void copyMatrix(double* matrixToCopy, double* matrixFromCopy, int size) {
    int i, j, c;
    for (i = 0; i < size; i++) {
        c = i * size;
        for (j = 0; j < size; j++) {
            *(matrixToCopy + c + j) = *(matrixFromCopy + c + j);
        }
    }
}

void matrixOrtogonalize(double* inputMatrix, double* ortoMatrix, double* upperTriangularMatrix, int size) {
    int i, j, c;
    copyMatrix(ortoMatrix, inputMatrix, size);
    *(upperTriangularMatrix) = 1;
    for (i = 1; i < size; i++) {
        *(upperTriangularMatrix + i * size) = 0;
    }
    for (i = 1; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (j < i) {
                *(upperTriangularMatrix + j * size + i) = vectorMultiplication(ortoMatrix, inputMatrix, size, j, i);
            }
            else
                if (i == j) {
                    *(upperTriangularMatrix + j * size + i) = 1;
                }
                else {
                    *(upperTriangularMatrix + j * size + i) = 0;
                }
        }
        for (j = 0; j < size; j++) {
            for (c = 0; c < i; c++) {
                *(ortoMatrix + j * size + i) -= *(upperTriangularMatrix + c * size + i) * *(ortoMatrix + j * size + c);
            }
        }
    }
}


void matrixTransposition(double* matrix, int size) {
    int i, j;
    double* helpMatrix = new double[size * size];
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            *(helpMatrix + i * size + j) = *(matrix + j * size + i);
        }
    }
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            *(matrix + j * size + i) = *(helpMatrix + j * size + i);
        }
    }
}

double detMatrix(double* matrix, int size) {
    int i, j, sign, c, a, b;
    double result = 0;
    if (size == 1) {
        result = *matrix;
    }
    else {
        for (i = 0; i < size; i++) {
            for (j = 0; j < size; j++) {
                if ((j + i) % 2 == 0) {
                    sign = 1;
                }
                else {
                    sign = -1;
                }
                double* minorMatrix = new double[(size - 1) * (size - 1)];
                c = 0;
                for (a = 0; a < size; a++) {
                    if (a != i) {
                        for (b = 0; b < size; b++) {
                            if (b != j) {
                                *(minorMatrix + c) = *(matrix + a * size + b);
                                c++;
                            }
                        }
                    }
                }
                result += *(matrix + i * size) * sign * detMatrix(minorMatrix, size - 1);
                delete[] minorMatrix;
            }
        }
    }
    return result;
}

void unionMatrix(double* unionMatrix, int size) {
    int i, j, sign, a, b, c;
    double* matrix = new double[size * size];
    copyMatrix(matrix, unionMatrix, size);
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if ((j + i) % 2 == 0) {
                sign = 1;
            }
            else {
                sign = -1;
            }
            double* minorMatrix = new double[(size - 1) * (size - 1)];
            c = 0;
            for (a = 0; a < size; a++) {
                if (a != i) {
                    for (b = 0; b < size; b++) {
                        if (b != j) {
                            *(minorMatrix + c) = *(matrix + a * size + b);
                            c++;
                        }
                    }
                }
            }
            *(unionMatrix + i * size + j) = sign * detMatrix(minorMatrix, size - 1);
            delete[] minorMatrix;
        }
    }
}

void inverseMatrix(double* inverseMatrix, int size) {
    double k = 1 / detMatrix(inverseMatrix, size);
    matrixTransposition(inverseMatrix, size);
    unionMatrix(inverseMatrix, size);
    numericMultiplication(inverseMatrix, k, size);
}

double* calculate(double* inpMatrix, double* vector, int size)
{

    double* ortoMatrix = new double[size * size];
    double* upperTriangularMatrix = new double[size * size];
    double* transpOrtoMatrix = new double[size * size];
    double* diagonalMatrix = new double[size * size];
    double* firstMatrixMult = new double[size * size];
    double* secondMatrixMult = new double[size * size];
    double* result = new double[size];

    if (detMatrix(inpMatrix, size) == 0) {
        result = NULL;
    }
    else {
        matrixOrtogonalize(inpMatrix, ortoMatrix, upperTriangularMatrix, size);
        delete[] inpMatrix;
        inverseMatrix(upperTriangularMatrix, size);
        copyMatrix(transpOrtoMatrix, ortoMatrix, size);
        matrixTransposition(transpOrtoMatrix, size);
        matrixMultiplication(transpOrtoMatrix, ortoMatrix, size, size, size, diagonalMatrix);
        delete[] ortoMatrix;
        inverseMatrix(diagonalMatrix, size);
        matrixMultiplication(upperTriangularMatrix, diagonalMatrix, size, size, size, firstMatrixMult);
        delete[] diagonalMatrix;
        delete[] upperTriangularMatrix;
        matrixMultiplication(firstMatrixMult, transpOrtoMatrix, size, size, size, secondMatrixMult);
        delete[] firstMatrixMult;
        delete[] transpOrtoMatrix;
        matrixMultiplication(secondMatrixMult, vector, size, size, 1, result);
        delete[] vector;
        delete[] secondMatrixMult;
    }
    return result;
}