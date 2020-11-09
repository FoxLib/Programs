#include <stdio.h>
#include <SDL.h>

__global__
void saxpy(int n, float a, float *x, float *y)
{
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  if (i < n) y[i] = cos(sin(a*x[i]) + y[i]);
}

int main(void)
{
    // 256 млн
    int N = 1 << 27;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    SDL_EnableUNICODE(1);
    SDL_Surface* screen = SDL_SetVideoMode(320, 200, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("CUDA NVCC", 0);

    float *x, *y, *d_x, *d_y;

    // Выделить память на CPU
    x = (float*) malloc(N*sizeof(float));
    y = (float*) malloc(N*sizeof(float));

    // Выделить память на GPU
    cudaMalloc(&d_x, N*sizeof(float));
    cudaMalloc(&d_y, N*sizeof(float));

    printf("step1\n");

    for (int i = 0; i < N; i++) {
        x[i] = 1.0f;
        y[i] = 2.0f;
    }

    printf("step2\n");

    // Скопировать данные
    cudaMemcpy(d_x, x, N*sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_y, y, N*sizeof(float), cudaMemcpyHostToDevice);

    printf("start\n");

    // Perform SAXPY on 1M elements
    saxpy<<<(N+255) / 256, 256>>>(N, 2.0f, d_x, d_y);

    // Подождать пока завершится
    cudaDeviceSynchronize();

    printf("stop\n");

    // Скопировать обратно
    cudaMemcpy(y, d_y, N*sizeof(float), cudaMemcpyDeviceToHost);

    printf(">>\n");

    cudaFree(d_x);
    cudaFree(d_y);
    free(x);
    free(y);
}

