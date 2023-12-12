#include "device_launch_parameters.h"
#include "cuda_runtime.h"
#include <stdio.h>

#define BLOCK_SIZE 4


void read_from_file(int* matrix, const char* path, int size_y, int size_x)
// have you ever heard who is error handling for?
{
    FILE* f;

//     f = fopen(path, "rb");
//     fread(matrix, SIZE_Y*SIZE_X, sizeof(int), f);

// non binary files for now
    f = fopen(path, "r");
    for (int i = 0; i < size_x*size_y; i++)
        fscanf(f, "%d ", (matrix + i));

    fclose(f);
    return;
}

__global__ void conv(float* kernel, int ker_size_y, int ker_size_x, int* matrix, int size_x, int res_size_y, int res_size_x, int* convolved_matrix)
{
    float sum = 0;
    int x = blockDim.x * blockIdx.x + threadIdx.x;
    int y = blockDim.y * blockIdx.y + threadIdx.y;
    if ((y < res_size_y) and (x < res_size_x)) // the image size may not be a multiple of BLOCK_SIZE, therefore an extra check is performed
    {
        printf("You are here (%d, %d)\n", x, y);
        for (int i = 0; i < ker_size_y; i++)
            for (int j = 0; j < ker_size_x; j++)
                sum += *(kernel + i * ker_size_x + j) * *(matrix + (y + i) * size_x + x + j);
        printf("Are you here yet? (%d, %d)\n", x, y);
        convolved_matrix[y * res_size_x + x] = static_cast<int>(sum); // is it good to do this in terms of efficiency?
                                                                      // should i use round() from <cmath> instead?
    }
    printf("You're not here (%d, %d)\n", x, y);
    return;
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
// events
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
// kernel parameters and the kernel itself
    int ker_size_y = 1;
    int ker_size_x = 1;
    float kernel[ker_size_y * ker_size_x] = {1};
// host: the matrix and convolved matrix
    int size_x = 10;
    int size_y = 10;
    int* matrix = (int *)malloc(size_y * size_x * sizeof(int));

    int res_size_y = size_y - ker_size_y + 1;
    int res_size_x = size_x - ker_size_x + 1;
    printf("res_x = %d; res_y = %d\n", res_size_x, res_size_y);
    int* convolved_matrix = (int *)malloc(res_size_y * res_size_x * sizeof(int));

    read_from_file(matrix, "matrix.txt", size_y, size_x);
// device: same
    int *d_matrix = NULL;
    cudaMalloc((void **)&d_matrix, size_y * size_x * sizeof(int));

    int *d_convolved_matrix = NULL;
    cudaMalloc((void **)&d_convolved_matrix, res_size_y * res_size_x * sizeof(int));
// copy matrix
    cudaMemcpy(d_matrix, matrix, size_y * size_x * sizeof(int), cudaMemcpyHostToDevice);
// calculating grid and block parameters
    dim3 threadsPerBlock = dim3(BLOCK_SIZE, BLOCK_SIZE);
    int bpg_x = res_size_x / BLOCK_SIZE; // bpg goes for "blocks per grid"
    if (res_size_x % BLOCK_SIZE != 0)
        bpg_x++;
    int bpg_y = res_size_y / BLOCK_SIZE;
    if (res_size_y % BLOCK_SIZE != 0)
        bpg_y++;
    dim3 blocksPerGrid = dim3(bpg_x, bpg_y);
//     printf("shape of res = (%d, %d)\nblocksPerGrid = dim3(%d, %d)\n", res_size_x, res_size_y, bpg_x, bpg_y);

    cudaEventRecord(start, 0);
// magic
    conv<<<blocksPerGrid, threadsPerBlock>>>(kernel, ker_size_y, ker_size_x, d_matrix, size_x, res_size_y, res_size_x, d_convolved_matrix);

    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop);
    float KernelTime;
    cudaEventElapsedTime(&KernelTime, start, stop);
    printf("KernelTime: %.2f milliseconds\n", KernelTime);
// copy convolved matrix
    cudaMemcpy(convolved_matrix, d_convolved_matrix, res_size_y * res_size_x * sizeof(int), cudaMemcpyDeviceToHost);

    save_matrix(convolved_matrix, "conv_matrix.txt", res_size_y, res_size_x);

    cudaFree(d_matrix);
    cudaFree(d_convolved_matrix);
    free(matrix);
    free(convolved_matrix);
    cudaEventDestroy(start);
    cudaEventDestroy(stop);
    return 0;
}
