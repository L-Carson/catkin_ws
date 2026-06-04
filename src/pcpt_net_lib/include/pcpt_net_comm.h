#pragma once

// headers in STL
#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

// headers in CUDA
#include "cuda_runtime_api.h"

// 屏蔽库文件编译告警
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#include "NvInfer.h"
#include "NvOnnxParser.h"
#pragma GCC diagnostic pop

using namespace std;

// using MACRO to allocate memory inside CUDA kernel
#define NUM_3D_BOX_CORNERS_MACRO 8

#define NUM_2D_BOX_CORNERS_MACRO 4

#define NUM_THREADS_MACRO 64

// need to be changed when num_threads_ is changed

#define DIVUP(m, n) ((m) / (n) + ((m) % (n) > 0))

class Logger : public nvinfer1::ILogger
{
public:
    void log(Severity severity, const char* msg) noexcept override {
        if (severity == Severity::kERROR || severity == Severity::kINTERNAL_ERROR) {
            std::cerr << "trt_infer: " << msg << std::endl;
        }
    }
};

#define GPU_CHECK(ans)                    \
{                                         \
    GPUAssert((ans), __FILE__, __LINE__); \
}

inline void GPUAssert(cudaError_t code, const char *file, int line, bool abort = true)
{
    if (code != cudaSuccess) {
        fprintf(stderr, "GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
        if (abort)
        exit(code);
    }
};

template <typename T>
void HOST_SAVE(T *array, int size, string filename, string root = "../test/result", string postfix = ".txt")
{
    string filepath = root + "/" + filename + postfix;
    if (postfix == ".bin") {
        fstream file(filepath, ios::out | ios::binary);
        file.write(reinterpret_cast<char *>(array), sizeof(size * sizeof(T)));
        file.close();
        std::cout << "|>>>|  Data has been written in " << filepath << "  |<<<|" << std::endl;
        return;
    } else if (postfix == ".txt") {
        ofstream file(filepath, ios::out);
        for (int i = 0; i < size; ++i)
        file << array[i] << " ";
        file.close();
        std::cout << "|>>>|  Data has been written in " << filepath << "  |<<<|" << std::endl;
        return;
    }
};

template <typename T>
void DEVICE_SAVE(T *array, int size, string filename, string root = "../test/result", string postfix = ".txt")
{
    T *temp_ = new T[size];
    cudaMemcpy(temp_, array, size * sizeof(T), cudaMemcpyDeviceToHost);
    HOST_SAVE<T>(temp_, size, filename, root, postfix);
    delete[] temp_;
};

