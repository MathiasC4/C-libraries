//
// Created by Matyas Cupr on 16/05/2022.
//

#ifndef MATRIX_H
#define MATRIX_H


#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


struct matrix {
    size_t rows;
    size_t cols;

    double content[];
};

struct matrix *create_matrix(size_t rows, size_t cols);

struct matrix *create_matrix_from_array(size_t rows, size_t cols, double array[], size_t array_len);

void set_matrix_element(struct matrix *matrix, size_t row, size_t col, double value);

double get_matrix_element(struct matrix *matrix, size_t row, size_t col);


struct matrix *scale_matrix(struct matrix *matrix, double scalar);

struct matrix *transpose_matrix(struct matrix *matrix);

struct matrix *add_matrices(struct matrix *matrix1, struct matrix *matrix2);

struct matrix *multiply_matrices(struct matrix *matrix1, struct matrix *matrix2);


void swap_rows(struct matrix *matrix, size_t row1, size_t row2);

void multiply_row(struct matrix *matrix, double multiplier, size_t row);


double gaussian_elimination(struct matrix *matrix);


struct matrix *get_submatrix(struct matrix *matrix, size_t removed_row, size_t removed_col);

double get_determinant(struct matrix *matrix);


void fprint_matrix(FILE *file, struct matrix *matrix, unsigned int max_precision);

void print_matrix(struct matrix *matrix, unsigned int max_precision);


#endif //MATRIX_H
