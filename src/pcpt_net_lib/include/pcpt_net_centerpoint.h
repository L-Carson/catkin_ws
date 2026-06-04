#pragma once

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <limits>
#include <map>
#include <memory>
#include <string>
#include <vector>

// 屏蔽库文件编译告警
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#include "NvInfer.h"
#include "NvOnnxParser.h"
#include "pcpt_net_base.h"
#include "pcpt_net_pre_process.h"
#include "pcpt_net_post_process2.h"
#pragma GCC diagnostic pop

using namespace std;

/**************************************************************************************
功能描述: CenterPoint
修改记录:
**************************************************************************************/
class CenterPoint : public Net_Base
{
public:
    /*score_threshold 过滤输出的分数阈值 nms_overlap_threshold NMS的IOU阈值*/
    CenterPoint(const float scoreThreshold, const float nmsOverlapThreshold,
                const bool useOnnx, const std::string modelFile);
    ~CenterPoint() {
        GPU_CHECK(cudaFree(paramsInput_));
        GPU_CHECK(cudaFree(devPointsNumPerPillar_));
        GPU_CHECK(cudaFree(dev_x_coors_));
        GPU_CHECK(cudaFree(dev_y_coors_));
        GPU_CHECK(cudaFree(devPillarPointFeature_));
        GPU_CHECK(cudaFree(devPillarCoors_));
        GPU_CHECK(cudaFree(devSparsePillarMap_));
        GPU_CHECK(cudaFree(devPfeGatherFeature_));
        GPU_CHECK(cudaFree(pfeBuffers_[0]));
        GPU_CHECK(cudaFree(rpnBuffers_[0]));
        GPU_CHECK(cudaFree(devFiltereBox_));
        GPU_CHECK(cudaFree(devFiltereScore_));
        GPU_CHECK(cudaFree(devFilterCount_));
        delete(netContext_);
        cudaStreamDestroy(stream_);
    }

    void DoInference(const float* pointsArrayIn, const int pointsNumIn, std::vector<float>* detectionsOut,
                     std::vector<int>* labelsOut, std::vector<float>* scoresOut);

 private:
    void DeviceMemoryMalloc();
    void SetDeviceMemoryToZero(); /*Memory set to 0 for device memory*/
    void InitParams();

    float               kPillarXSize_{0.2};
    float               kPillarYSize_{0.2};
    float               kPillarZSize_{6.0};
    float               kMinXRange_{-31.2};
    float               kMinYRange_{-31.2};
    float               kMinZRange_{-3.0};
    float               kMaxXRange_{31.2};
    float               kMaxYRange_{31.2};
    float               kMaxZRange_{3.0};

    int                 kNumClass_{3};
    int                 kMaxNumPillars_{60000};
    int                 kMaxNumPointsPerPillar_{24};    //36
    int                 kNumPointFeature_{5};
    int                 kNumGatherPointFeature_{11};
    int                 kNumPostOutputBoxFeature_{7};
    int                 kNumOutputBoxFeature_{8};
    int                 kNumIndsForScan_{1024};
    int                 kNumThreads_{64};
    int                 kNumBoxCorners_{8};
    int                 kNmsPreMaxsize_{1000};
    int                 kNmsPostMaxsize_{83};
    int                 kStrides_{4};

    int                 kGridXSize_;
    int                 kGridYSize_;
    int                 kGridZSize_;

    int                 kRpnInputSize_;
    int                 kRpnBoxOutputSize_;
    int                 kRpnClsOutputSize_;

    int                 kNumPerCls_;
    int                 kNumXinds_;
    int                 kNumYinds_;
    int                 kNumFeature_;

    int                 hostPillarCount_[1];

    int*   dev_x_coors_;
    int*   dev_y_coors_;
    float* devPointsNumPerPillar_;
    int*   devSparsePillarMap_;

    float* devPillarPointFeature_;
    float* devPillarCoors_;

    float* devPfeGatherFeature_;
    void*  pfeBuffers_[1];
    void*  rpnBuffers_[1];
    int*   paramsInput_;

    float* devFiltereBox_;
    float* devFiltereScore_;
    int*   devFilterCount_;

    std::unique_ptr<PreprocessPointsCuda> preprocessPointsCudaPtr_;
    std::unique_ptr<PostprocessCuda2>     postprocessCudaPtr_;
    cudaStream_t                          stream_;

    nvinfer1::IExecutionContext* netContext_{nullptr};
};

