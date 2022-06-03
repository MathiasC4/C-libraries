//
// Created by Matyas Cupr on 16/05/2022.
//

#include "matrix.h"

struct matrix *create_matrix(size_t rows, size_t cols)
{
    struct matrix *matrix = calloc(1, sizeof(struct matrix) + (rows * cols * sizeof(double)));
    if (matrix == NULL) {
        fprintf(stderr, "Failed to create array\n");
        return NULL;
    }

    matrix->rows = rows;
    matrix->cols = cols;

    return matrix;
}

struct matrix *create_matrix_from_array(size_t rows, size_t cols, double array[], size_t array_len)
{
    assert(array != NULL);

    struct matrix *matrix = create_matrix(rows, cols);
    if (matrix == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < (array_len > rows * cols ? rows * cols : array_len); i += 1) {
        size_t row = i / cols;
        size_t col = i % cols;

        set_matrix_element(matrix, row, col, array[i]);
    }

    return matrix;
}

void set_matrix_element(struct matrix *matrix, size_t row, size_t col, double value)
{
    assert(matrix != NULL);

    if (row >= matrix->rows || col >= matrix->cols) {
        fprintf(stderr, "Invalid matrix index\n");
        return;
    }

    matrix->content[row * matrix->cols + col] = value;
}

double get_matrix_element(struct matrix *matrix, size_t row, size_t col)
{
    assert(matrix != NULL);

    if (row >= matrix->rows || col >= matrix->cols) {
        fprintf(stderr, "Invalid matrix index\n");
        return NAN;
    }

    return matrix->content[row * matrix->cols + col];
}


struct matrix *scale_matrix(struct matrix *matrix, double scalar)
{
    for (size_t row = 0; row < matrix->rows; row += 1) {
        for (size_t col = 0; col < matrix->cols; col += 1) {
            set_matrix_element(matrix, row, col, scalar * get_matrix_element(matrix, row, col));
        }
    }
}

struct matrix *transpose_matrix(struct matrix *matrix)
{
    struct matrix *transpose = create_matrix(matrix->cols, matrix->rows);
    if (transpose == NULL) {
        return NULL;
    }

    for (size_t row = 0; row < matrix->rows; row += 1) {
        for (size_t col = 0; col < matrix->cols; col += 1) {
            set_matrix_element(transpose, col, row, get_matrix_element(matrix, row, col));
        }
    }

    return transpose;
}

struct matrix *add_matrices(struct matrix *matrix1, struct matrix *matrix2)
{
    assert(matrix1 != NULL);
    assert(matrix2 != NULL);

    if (matrix1->rows != matrix2->rows || matrix1->cols != matrix2->cols) {
        fprintf(stderr, "Incompatible matrices\n");
        return NULL;
    }

    struct matrix *result = create_matrix(matrix1->rows, matrix1->cols);
    if (result == NULL) {
        return NULL;
    }

    for (size_t row = 0; row < matrix1->rows; row += 1) {
        for (size_t col = 0; col < matrix1->cols; col += 1) {
            set_matrix_element(result, row, col, (get_matrix_element(matrix1, row, col) + get_matrix_element(matrix2, row, col)));
        }
    }

    return result;
}

struct matrix *multiply_matrices(struct matrix *matrix1, struct matrix *matrix2)
{
    assert(matrix1 != NULL);
    assert(matrix2 != NULL);

    if (matrix1->cols != matrix2->rows) {
        fprintf(stderr, "Incompatible matrices\n");
        return NULL;
    }

    struct matrix *result = create_matrix(matrix1->rows, matrix2->cols);
    if (result == NULL) {
        return NULL;
    }

    for (size_t row1 = 0; row1 < matrix1->rows; row1 += 1) {
        for (size_t col2 = 0; col2 < matrix2->cols; col2 += 1) {
            double value = 0;

            for (size_t col1 = 0; col1 < matrix1->cols; col1 += 1) {
                size_t row2 = col1;

                value += get_matrix_element(matrix1, row1, col1) * get_matrix_element(matrix2, row2, col2);
            }

            set_matrix_element(result, row1, col2, value);
        }
    }

    return result;
}


void swap_rows(struct matrix *matrix, size_t row1, size_t row2)
{
    for (size_t col = 0; col < matrix->cols; col += 1) {
        double value1 = get_matrix_element(matrix, row1, col);
        set_matrix_element(matrix, row1, col, get_matrix_element(matrix, row2, col));
        set_matrix_element(matrix, row2, col, value1);
    }
}

void multiply_row(struct matrix *matrix, double multiplier, size_t row)
{
    for (size_t col = 0; col < matrix->cols; col += 1) {
        set_matrix_element(matrix, row, col, multiplier * get_matrix_element(matrix, row, col));
    }
}

double gaussian_elimination(struct matrix *matrix)
{
    double determinant_multiplier = 1;

    for (size_t col = 0; col < matrix->cols; col += 1) {
        size_t first_zero = 0;
        for (size_t row = 1; row < matrix->rows; row += 1) {
            if (get_matrix_element(matrix, row, col) != 0) {
                swap_rows(matrix, first_zero, row);
                determinant_multiplier *= -1;
                first_zero += 1;
            }
        }
    }

    return determinant_multiplier;
}


struct matrix *get_submatrix(struct matrix *matrix, size_t removed_row, size_t removed_col)
{
    assert(matrix != NULL);

    struct matrix *submatrix = create_matrix(matrix->rows - 1, matrix->cols - 1);
    if (submatrix == NULL) {
        return NULL;
    }

    for (size_t r = 0; r < matrix->rows; r += 1) {
        if (r == removed_row) {
            continue;
        }

        for (size_t c = 0; c < matrix->cols; c += 1) {
            if (c == removed_col) {
                continue;
            }

            double value = get_matrix_element(matrix, r, c);

            set_matrix_element(submatrix, (r > removed_row ? r - 1 : r), (c > removed_col ? c - 1 : c), value);
        }
    }

    return submatrix;
}

static double get_minor(struct matrix *matrix, size_t removed_row, size_t removed_col)
{
    assert(matrix != NULL);

    struct matrix *submatrix = get_submatrix(matrix, removed_row, removed_col);
    if (submatrix == NULL) {
        return NAN;
    }

    double minor = get_determinant(submatrix);

    free(submatrix);

    return minor;
}

static double get_cofactor(struct matrix *matrix, size_t removed_row, size_t removed_col)
{
    assert(matrix != NULL);

    double cofactor = get_minor(matrix, removed_row, removed_col);
    if (!isnan(cofactor)) {
        cofactor = pow(-1.0, removed_row + removed_col) * cofactor;
    }

    return cofactor;
}

double get_determinant(struct matrix *matrix)
{
    if (matrix->rows != matrix->cols) {
        fprintf(stderr, "Not a square matrix\n");
        return NAN;
    }

    if (matrix->rows == 1) {
        return get_matrix_element(matrix, 0, 0);
    }

    size_t most_zeros = 0;
    size_t most_zeros_row = 0;

    for (size_t row = 0; row < matrix->rows; row += 1) {
        size_t zeros = 0;

        for (size_t col = 0; col < matrix->cols; col += 1) {
            if (get_matrix_element(matrix, row, col) == 0) {
                zeros += 1;
            }
        }

        if (zeros > most_zeros) {
            most_zeros_row = row;
            most_zeros = zeros;
        }
    }

    double determinant = 0;

    for (size_t col = 0; col < matrix->cols; col += 1) {
        double coefficient = get_matrix_element(matrix, most_zeros_row, col);
        if (coefficient == 0) {
            continue;
        }

        determinant += coefficient * get_cofactor(matrix, most_zeros_row, col);
    }

    return determinant;
}


static unsigned int count_digits(double number)
{
    unsigned int count = 0;

    number = trunc(fabs(number));

    while (number > 0) {
        count += 1;
        number = trunc(number / 10);
    }

    return count;
}

static unsigned int get_precision(double number, unsigned int max_precision)
{
    unsigned int precision = max_precision;

    long long unsigned int integer = (long long unsigned int)(trunc(fabs(number * pow(10, max_precision))));

    while (integer % 10 == 0 && precision > 0) {
        precision -= 1;
        integer /= 10;
    }

    return precision;
}

static void get_column_sizes(struct matrix *matrix, unsigned int number_sizes[], unsigned int max_precision)
{
    for (size_t col = 0; col < matrix->cols; col += 1) {
        unsigned int size = 0;

        for (size_t row = 0; row < matrix->rows; row += 1) {
            double number = get_matrix_element(matrix, row, col);
            unsigned int precision = get_precision(number, max_precision);

            unsigned int number_size = 1 + count_digits(number) + (precision > 0 ? 1 + precision : precision);

            if (number_size > size) {
                size = number_size;
            }
        }

        number_sizes[col] = size;
    }
}

void fprint_matrix(FILE *file, struct matrix *matrix, unsigned int max_precision)
{
    assert(file != NULL);
    assert(matrix != NULL);

    unsigned int column_sizes[matrix->cols];
    get_column_sizes(matrix, column_sizes, max_precision);

    for (size_t row = 0; row < matrix->rows; row += 1) {
        fprintf(file, "[");

        for (size_t col = 0; col < matrix->cols; col += 1) {
            double value = get_matrix_element(matrix, row, col);
            fprintf(file, "% *.*f", column_sizes[col], get_precision(value, max_precision), value);
            if (col < matrix->cols - 1) {
                putc(' ', file);
            }
        }

        fprintf(file, "]\n");
    }
}

void print_matrix(struct matrix *matrix, unsigned int max_precision)
{
    assert(matrix != NULL);

    fprint_matrix(stdout, matrix, max_precision);
}
