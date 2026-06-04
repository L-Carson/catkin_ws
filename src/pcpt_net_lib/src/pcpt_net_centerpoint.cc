#include "pcpt_net_centerpoint.h"
#include <chrono>
#include <iostream>

/**************************************************************************************
功能描述: CenterPoint
修改记录:
**************************************************************************************/
CenterPoint::CenterPoint(const float scoreThreshold,
                         const float nmsOverlapThreshold,
                         const bool useOnnx,
                         const std::string modelFile)
{
    InitParams();
    InitTRT(useOnnx, modelFile, netContext_);
    DeviceMemoryMalloc();
    preprocessPointsCudaPtr_.reset(new PreprocessPointsCuda(
                                   kNumThreads_,
                                   kMaxNumPillars_,
                                   kMaxNumPointsPerPillar_,
                                   kNumPointFeature_,
                                   kNumIndsForScan_,
                                   kGridXSize_, kGridYSize_, kGridZSize_,
                                   kPillarXSize_, kPillarYSize_, kPillarZSize_,
                                   kMinXRange_, kMinYRange_, kMinZRange_));

    const float float_min = std::numeric_limits<float>::lowest();
    const float float_max = std::numeric_limits<float>::max();
    postprocessCudaPtr_.reset(new PostprocessCuda2(
                              kNumThreads_, float_min, float_max,
                              kNumClass_, kNumPerCls_,
                              scoreThreshold, nmsOverlapThreshold,
                              kNmsPreMaxsize_, kNmsPostMaxsize_, kNumBoxCorners_,
                              kNumOutputBoxFeature_, kNumPostOutputBoxFeature_));
    GPU_CHECK(cudaStreamCreate(&stream_));
}

void CenterPoint::InitParams()
{
    kGridXSize_  = static_cast<int>((kMaxXRange_ - kMinXRange_) / kPillarXSize_);
    kGridYSize_  = static_cast<int>((kMaxYRange_ - kMinYRange_) / kPillarYSize_);
    kGridZSize_  = static_cast<int>((kMaxZRange_ - kMinZRange_) / kPillarZSize_);

    kNumXinds_   = static_cast<int>(kGridXSize_ / kStrides_);
    kNumYinds_   = static_cast<int>(kGridYSize_ / kStrides_);
    kNumFeature_ = kNumXinds_ * kNumYinds_  * kNumClass_;
    kNumPerCls_  = kNumXinds_ * kNumYinds_ ;
}

/**************************************************************************************
功能描述: DeviceMemoryMalloc
修改记录:
**************************************************************************************/
void CenterPoint::DeviceMemoryMalloc()
{
    // for pillars
    GPU_CHECK(cudaMalloc(reinterpret_cast<void**>(&paramsInput_), 5 * sizeof(int)));
    GPU_CHECK(cudaMalloc(reinterpret_cast<void**>(&devPointsNumPerPillar_), kMaxNumPillars_ * sizeof(float)));
    GPU_CHECK(cudaMalloc(reinterpret_cast<void**>(&dev_x_coors_), kMaxNumPillars_ * sizeof(int)));
    GPU_CHECK(cudaMalloc(reinterpret_cast<void**>(&dev_y_coors_), kMaxNumPillars_ * sizeof(int)));
    GPU_CHECK(cudaMalloc(reinterpret_cast<void**>(&devPillarPointFeature_), kMaxNumPillars_ * kMaxNumPointsPerPillar_ * kNumPointFeature_ * sizeof(float)));
    GPU_CHECK(cudaMalloc(reinterpret_cast<void**>(&devPillarCoors_), kMaxNumPillars_ * 4 * sizeof(float)));

    // for sparse map
    GPU_CHECK(cudaMalloc(reinterpret_cast<void**>(&devSparsePillarMap_), kNumIndsForScan_ * kNumIndsForScan_ * sizeof(int)));
    GPU_CHECK(cudaMalloc(reinterpret_cast<void**>(&devPfeGatherFeature_), kMaxNumPillars_ * kMaxNumPointsPerPillar_ * kNumGatherPointFeature_ * sizeof(float)));
    GPU_CHECK(cudaMalloc(&pfeBuffers_[0], kMaxNumPillars_ * kMaxNumPointsPerPillar_ * kNumGatherPointFeature_ * sizeof(float)));
    GPU_CHECK(cudaMalloc(&rpnBuffers_[0],  kNumPerCls_ * kNumClass_ *  kNumOutputBoxFeature_ * sizeof(float)));
    GPU_CHECK(cudaMalloc(reinterpret_cast<void**>(&devFiltereBox_), kNumFeature_ * kNumOutputBoxFeature_ * sizeof(float)));
    GPU_CHECK(cudaMalloc(reinterpret_cast<void**>(&devFiltereScore_), kNumFeature_ * sizeof(float)));
    GPU_CHECK(cudaMalloc(reinterpret_cast<void**>(&devFilterCount_), kNumClass_ * sizeof(int)));
}

/**************************************************************************************
功能描述: SetDeviceMemoryToZero
修改记录:
**************************************************************************************/
void CenterPoint::SetDeviceMemoryToZero()
{
    GPU_CHECK(cudaMemset(paramsInput_,           0, 5 * sizeof(int)));
    GPU_CHECK(cudaMemset(devPointsNumPerPillar_, 0, kMaxNumPillars_ * sizeof(float)));
    GPU_CHECK(cudaMemset(dev_x_coors_,           0, kMaxNumPillars_ * sizeof(int)));
    GPU_CHECK(cudaMemset(dev_y_coors_,           0, kMaxNumPillars_ * sizeof(int)));
    GPU_CHECK(cudaMemset(devPillarPointFeature_, 0, kMaxNumPillars_ * kMaxNumPointsPerPillar_ * kNumPointFeature_ * sizeof(float)));
    GPU_CHECK(cudaMemset(devPillarCoors_,        0, kMaxNumPillars_ * 4 * sizeof(float)));
    GPU_CHECK(cudaMemset(devSparsePillarMap_,    0, kNumIndsForScan_ * kNumIndsForScan_ * sizeof(int)));
    GPU_CHECK(cudaMemset(devPfeGatherFeature_,   0, kMaxNumPillars_ * kMaxNumPointsPerPillar_ * kNumGatherPointFeature_ * sizeof(float)));

    GPU_CHECK(cudaMemset(pfeBuffers_[0],         0, kMaxNumPillars_ * kMaxNumPointsPerPillar_ * kNumGatherPointFeature_ * sizeof(float)));

    GPU_CHECK(cudaMemset(rpnBuffers_[0],         0, kNumPerCls_ * kNumClass_ * kNumOutputBoxFeature_* sizeof(float)));
    GPU_CHECK(cudaMemset(devFiltereBox_,         0, kNumFeature_ * kNumOutputBoxFeature_ * sizeof(float)));
    GPU_CHECK(cudaMemset(devFiltereScore_,       0, kNumFeature_ * sizeof(float)));
    GPU_CHECK(cudaMemset(devFilterCount_,        0, kNumClass_ * sizeof(int)));
}

/**************************************************************************************
功能描述: DoInference
修改记录:
**************************************************************************************/
void CenterPoint::DoInference(const float* pointsArrayIn,
                              const int pointsNumIn,
                              std::vector<float>* detectionsOut,
                              std::vector<int>* labelsOut,
                              std::vector<float>* scoresOut)
{
    float* devPoints;
    GPU_CHECK(cudaMalloc(reinterpret_cast<void**>(&devPoints), pointsNumIn * kNumPointFeature_ * sizeof(float)));
    GPU_CHECK(cudaMemset(devPoints, 0, pointsNumIn * kNumPointFeature_ * sizeof(float)));
    GPU_CHECK(cudaMemcpy(devPoints, pointsArrayIn, pointsNumIn * kNumPointFeature_ * sizeof(float), cudaMemcpyHostToDevice));
    SetDeviceMemoryToZero();
    cudaDeviceSynchronize();
    hostPillarCount_[0] = 0;
    preprocessPointsCudaPtr_->DoPreprocessPointsCuda(devPoints, pointsNumIn, dev_x_coors_, dev_y_coors_,
                                                     devPointsNumPerPillar_, devPillarPointFeature_,
                                                     devPillarCoors_, devSparsePillarMap_,
                                                     hostPillarCount_, devPfeGatherFeature_);
    cudaDeviceSynchronize();
    GPU_CHECK(cudaMemcpyAsync(pfeBuffers_[0], devPfeGatherFeature_,
                              kMaxNumPillars_ * kMaxNumPointsPerPillar_ * kNumGatherPointFeature_ * sizeof(float),
                              cudaMemcpyDeviceToDevice, stream_));
    unsigned int params_input_cpu[5];
    params_input_cpu[0] = 1;
    params_input_cpu[1] = kNumThreads_;
    params_input_cpu[2] = kGridXSize_;
    params_input_cpu[3] = kGridYSize_;
    params_input_cpu[4] = hostPillarCount_[0];
    GPU_CHECK(cudaMemcpy(paramsInput_, params_input_cpu, 5 * sizeof(int), cudaMemcpyHostToDevice));

    cudaDeviceSynchronize();
    void * buffers[] ={pfeBuffers_[0],devPillarCoors_,paramsInput_,rpnBuffers_[0]};
    if (netContext_) netContext_->enqueueV2(buffers, stream_, nullptr);
    cudaDeviceSynchronize();
    postprocessCudaPtr_->DoPostprocessCuda(reinterpret_cast<float*>(rpnBuffers_[0]),
                                           devFiltereBox_, devFiltereScore_, devFilterCount_,
                                           *detectionsOut, *labelsOut, *scoresOut);
    cudaDeviceSynchronize();

    GPU_CHECK(cudaFree(devPoints));
}
