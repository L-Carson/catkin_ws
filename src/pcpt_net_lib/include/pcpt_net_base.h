#pragma once

#include <memory>
#include "pcpt_net_comm.h"

using namespace std;

extern int batch_size;

class Net_Base
{
public:
    Net_Base(){}
    virtual ~Net_Base() {
        delete(netEngine_);
    }

    virtual void DoInference(const float* pointsArrayIn, const int pointsNumIn, std::vector<float>* detectionsOut,
                             std::vector<int>* labelsOut, std::vector<float>* scoresOut) = 0;
    static void GenerateEngine(string onnxFile, string engineFile);

protected:
    void InitTRT(const bool useOnnx, const string& modelFile, nvinfer1::IExecutionContext*& netContext);

private:
    Logger                  g_logger_;
    nvinfer1::ICudaEngine*  netEngine_{nullptr};

    void OnnxToTRTModel(const std::string& modelFile, nvinfer1::ICudaEngine** enginePtr);
    void EngineToTRTModel(const std::string& engineFile, nvinfer1::ICudaEngine** enginePtr);
};
