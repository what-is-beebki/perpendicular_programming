#include <stdio.h>

#define SIZE_X 100
#define SIZE_Y 100
// origin is located in the upper left corner of the image/kernel


void read_from_file(int* matrix, const char* path)
// have you ever heard who is error handling for?
{
    FILE* f;

//     f = fopen(path, "rb");
//     fread(matrix, SIZE_Y*SIZE_X, sizeof(int), f);

// non binary files
    f = fopen(path, "r");
    for (int i = 0; i < SIZE_X*SIZE_Y; i++)
        fscanf(f, "%d ", (matrix + i));

    fclose(f);
    return;
}

int conv(int y, int x, float* kernel, int ker_size_y, int ker_size_x, int* matrix)
{
    float sum = 0;
    int i = 0;
    int j = 0;
    for (; i < ker_size_y; i++)
        for (; j < ker_size_x; j++)
            sum += *(kernel + i * ker_size_x + j) * *(matrix + (y + i) * SIZE_X + x + j);
    return static_cast<int>(sum); // is it good to do this in terms of efficiency?
                                  // should i use round() from <cmath> instead?
}

void save_matrix(int* matrix, const char* path, int res_size_y, int res_size_x)
{
    FILE* f;
    f = fopen(path, "w");
    for (int i = 0; i < res_size_y; i++)
    {
        for (int j = 0; j < res_size_x; j++)
        {
            fprintf(f, "%d ", *(matrix + i * res_size_x + j));
        }
        fprintf(f, "\n");
    }
    fclose(f);
    return;
}

int main()
{
    int ker_size_y = 3; // vertical axis
    int ker_size_x = 3; // horizontal axis
    // origin is located in the upper left corner of the image/kernel
    float kernel[ker_size_y * ker_size_x] = {-1, 0, 1,
                                            -1, 0, 1,
                                            -1, 0, 1};

    int matrix[SIZE_Y * SIZE_X];
    int res_size_y = SIZE_Y - ker_size_y + 1;
    int res_size_x = SIZE_X - ker_size_x + 1;
    int convolved_matrix[res_size_y * res_size_x] = {0};

    read_from_file(matrix, "matrix.txt");

    for (int i = 0; i < res_size_y; i++)
    {
        for (int j = 0; j < res_size_x; j++)
        {
            convolved_matrix[i * res_size_x + j] = conv(i, j, kernel, ker_size_y, ker_size_x, matrix);
        }
    }
    save_matrix(convolved_matrix, "example_big.txt", res_size_y, res_size_x);
    return 0;
}
