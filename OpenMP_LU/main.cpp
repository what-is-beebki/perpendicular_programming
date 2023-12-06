#include <cstdio>
// reading arguments from the command line is for girls
#define SIZE 10

void read_from_file(double* matrix_A, const char* path)
// error handling is for girls
{
    FILE* f;
    f = fopen(path, "rb");
    fread(matrix_A, SIZE*SIZE, sizeof(double), f);
    fclose(f);
    return;
}

void print_matrix(double* any_matrix, const char* what_matrix = "")
{
    if (what_matrix != "")
    {
        printf("That's matrix %s\n", what_matrix);
    }

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            printf("%lf ", any_matrix[i * SIZE + j]);
        }
        printf("\n");
    }
    return;
}

void decompose(double* matrix_A, double* matrix_L, double* matrix_U, int depth)
// recursion is also for girls but i like recursion
{
    double a_ii = *(matrix_A + depth * SIZE + depth);

    for (int i = depth; i < SIZE; i++)
    {

        *(matrix_L + i * SIZE + depth) = *(matrix_A + i * SIZE + depth) / a_ii;
        *(matrix_U + depth * SIZE + i) = *(matrix_A + depth * SIZE + i);

    }

    #pragma omp parallel for collapse(2)
    for (int i = depth + 1; i < SIZE; i++)
    {
        for (int j = depth + 1; j < SIZE; j++)
        {
            *(matrix_A + SIZE * i + j) = *(matrix_A + SIZE * i + j) - *(matrix_L + i * SIZE + depth) * *(matrix_U + depth * SIZE + j);
        }
    }

    if (depth + 1 == SIZE)
        return;
    else
        decompose(matrix_A, matrix_L, matrix_U, depth + 1);
}

void save_matrix(double* matrix_L, const char* path)
{
    FILE* f;
    f = fopen(path, "wb");
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            fprintf(f, "%lf ", *(matrix_L + i * SIZE + j));
        }
        fprintf(f, "\n");
    }
    fclose(f);
    return;
}

int main()
{
    double matrix_A[SIZE * SIZE];
    double matrix_L[SIZE * SIZE] = {0};
    double matrix_U[SIZE * SIZE] = {0};

//     did i mention that reading arguments from the command line is for girls?
    read_from_file(matrix_A, "matrix_A.txt"); //SIZE = 10
//     print_matrix(matrix_A); //SIZE = 3
    decompose(matrix_A, matrix_L, matrix_U, 0);
    save_matrix(matrix_L, "matrix_L.txt");
    save_matrix(matrix_U, "matrix_U.txt");

    return 0;
}
