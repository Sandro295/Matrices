// single:  clang -O2 -march=native gemm.c
// multi:   clang -O2 -march=native gemm.c -DNTHREADS=32 -lpthread
// #define _GNU_SOURCE

// https://en.wikichip.org/wiki/amd/microarchitectures/zen_2
#include <assert.h>
#include <chrono>
#include <immintrin.h>
#include <math.h>
#include <sched.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <array>

// #define DEBUG

#ifdef DEBUG
#define N 4
#endif

constexpr double EPS = 1e-3;

#ifndef N
// NOTE: if you change this you have to rerun gemm.py
#define N 1024
#endif

// // aligned?
// float A[N * N] __attribute__((aligned(64)));
// float B[N * N] __attribute__((aligned(64)));
// float C[N * N] __attribute__((aligned(64)));
// float npResult[N * N] __attribute__((aligned(64)));

float A[N * N];
float B[N * N];
float C[N * N];
float npResult[N * N];

// std::array<float, N * N> A;
// std::array<float, N * N> B;
// std::array<float, N * N> C;
// std::array<float, N * N> npResult;

__m256 *Am = (__m256 *)A;
__m256 *Bm = (__m256 *)B;
__m256 *Cm = (__m256 *)C;

uint64_t nanos()
{
    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    return (uint64_t)start.tv_sec * 1e9 + (uint64_t)start.tv_nsec;
}

float Bf[N * N] __attribute__((aligned(64)));
__m256 *Bfm = (__m256 *)Bf;

#define BLOCK 8
#define BLOCK_Y 4
#define BLOCK_X 2

void matmul_naive()
{
    for (int y = 0; y < N; y += 1)
    {
        for (int k = 0; k < N; k++)
        {
            // float acc{0};
            for (int x = 0; x < N; x += 1)
            {
                // acc += A[y*N + k] * B[k*N + x];
                C[y * N + x] += A[y * N + k] * B[k * N + x];
            }
            // C[y*N + x] = acc;
        }
    }
}

void matmulBlocks()
{
    // dopest blocksize is 16 for my Ryzen 5600x
    // accumulator variable doesn't matter
    static_assert(N % BLOCK == 0);
    for (int jj = 0; jj < N; jj += BLOCK) {
        int jjMin = std::min(jj + BLOCK, N);
        for (int kk = 0; kk < N; kk += BLOCK) {
            int kkMin = std::min(kk + BLOCK, N);
            for (int i = 0; i < N; i++) {
                for (int j = jj; j < jjMin; j++) {
                    float temp = 0;
                    for (int k = kk; k < kkMin; k++) {
                        // temp += A[i * N + k] * B[k * N + j];
                        C[i * N + j] += A[i * N + k] * B[k * N + j];
                    }
                    // C[i * N + j] += temp;
                }
            }
        }
    }
}

void matmul(int sy, int ey)
{
    // 136.77 GFLOPS on single core numpy
    // 4.9 GHz is max boost for 5950X
    // 32 FLOPS/cycle (16 FMAs, aka 2x 8 single wide / 32 byte FMAs)
    // theoretical max is 156.8 GFLOPS, we see 150
    // multicore theo max = 2508.8 GFLOPS, we see 1501.434299

    // Bf = (y/8, k, 8)
#if 1
    for (int y = sy; y < ey; y += BLOCK_Y)
    {
        for (int x = 0; x < N; x += BLOCK * BLOCK_X)
        {
            __m256 acc[BLOCK_Y][BLOCK_X] = {};
            for (int k = 0; k < N; k++)
            {
                for (int iy = 0; iy < BLOCK_Y; iy++)
                {
                    __m256 ta = _mm256_broadcast_ss(&A[(y + iy) * N + k]);
                    for (int ix = 0; ix < BLOCK_X; ix++)
                    {
                        acc[iy][ix] = _mm256_fmadd_ps(
                            ta, Bfm[((x + ix * BLOCK) * N + k * 8) / 8], acc[iy][ix]);
                    }
                }
            }
            for (int iy = 0; iy < BLOCK_Y; iy++)
            {
                for (int ix = 0; ix < BLOCK_X; ix++)
                {
                    Cm[((y + iy) * N + x + ix * BLOCK) / 8] = acc[iy][ix];
                }
            }
        }
    }
#endif
}

int main()
{
    printf("hello\n");

#ifdef DEBUG
    for (int i = 0; i < N * N; i++)
        A[i] = i;
    for (int i = 0; i < N * N; i++)
        B[i] = i;
#else
    FILE *f = fopen("/tmp/matmul", "rb");
    if (f == NULL)
    {
        printf("please pregenerate python /tmp/matmul file\n");
        return -1;
    }
    fread(A, 1, sizeof(float) * N * N, f);
    fread(B, 1, sizeof(float) * N * N, f);
    fread(npResult, 1, sizeof(float) * N * N, f);
    fclose(f);
#endif

    for (int i = 0; i < 10; i++)
    {
        memset(C, 0, N * N * sizeof(float));
        auto start = std::chrono::high_resolution_clock::now();
        matmul(0, N);
        // matmul_naive();
        // matmulBlocks();
        auto end = std::chrono::high_resolution_clock::now();

        double gflop = (2.0 * N * N * N) * 1e-9;
        double s = (end - start).count() * 1e-9;
        printf("%f GFLOP/S -- %.2f ms\n", gflop / s, s * 1e3);

        // hack around throttling
        // if (i%4 == 0) sleep(1);
    }

#ifdef DEBUG
    for (int i = 0; i < N * N; i++)
    {
        if (i % N == 0 && i != 0)
            printf("\n");
        printf("%f\t", C[i]);
    }
    printf("\n");
#else
    for (int k = 0; k < N * N; k++)
    {
        if (std::abs(C[k] - npResult[k]) > EPS)
        {
            printf("MISMATCH AT %d, %f != %f\n", k, C[k], npResult[k]);
            return -1;
        }
    }

    // for (int k = 0; k < N * N; k++) {
    //     for (int x = 0; x < N * N; x++) {
    //         if (std::abs(C[k][x] - npResult[k][x]) > EPS)
    //         {
    //             printf("MISMATCH AT %d, %f != %f\n", k, C[k][x],
    //             npResult[k][x]); return -1;
    //         }
    //     }
    // }

    printf("match\n");
#endif

    return 0;
}
