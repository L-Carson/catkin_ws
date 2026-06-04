#include "pcpt_net_base.h"

int batch_size{1};

/**************************************************************************************
功能描述: GenerateEngine
修改记录:
**************************************************************************************/
void Net_Base::GenerateEngine(string onnxFile, string engineFile)
{
    Logger g_logger;
    int verbosity = static_cast<int>(nvinfer1::ILogger::Severity::kWARNING);

    // create the builder
    const auto explicit_batch = static_cast<uint32_t>(1) << static_cast<uint32_t>(
                                nvinfer1::NetworkDefinitionCreationFlag::kEXPLICIT_BATCH);
    nvinfer1::IBuilder* builder           = nvinfer1::createInferBuilder(g_logger);
    nvinfer1::INetworkDefinition* network = builder->createNetworkV2(explicit_batch);

    // parse onnx model
    auto parser = nvonnxparser::createParser(*network, g_logger);
    if (!parser->parseFromFile(onnxFile.c_str(), verbosity)) {
        std::string msg("failed to parse model file");
        g_logger.log(nvinfer1::ILogger::Severity::kERROR, msg.c_str());
        exit(EXIT_FAILURE);
    }

    // Build the engine
    builder->setMaxBatchSize(batch_size);

    //builder->setHalf2Mode(true);
    nvinfer1::IBuilderConfig* config = builder->createBuilderConfig();
    config->setFlag(nvinfer1::BuilderFlag::kFP16);
    std::cout << "Enable fp16!" << std::endl;
    config->setMaxWorkspaceSize(1 << 30);

    nvinfer1::ICudaEngine* engine = builder->buildEngineWithConfig(*network, *config);
    if (engine != nullptr) {
        auto trtModelStream = (engine->serialize());
        std::fstream trtOut(engineFile, std::ifstream::out);
        if (!trtOut.is_open()) {
            std::cout << "Can't store trt cache.\n";
            return;
        }
        trtOut.write((char*)trtModelStream->data(), trtModelStream->size());
        trtOut.close();
        delete(trtModelStream);
    }

    delete(parser);
    delete(network);
    delete(config);
    delete(builder);
}

/**************************************************************************************
功能描述: InitTRT
修改记录:
**************************************************************************************/
void Net_Base::InitTRT(const bool useOnnx, const string& modelFile, nvinfer1::IExecutionContext*& netContext)
{
    if (useOnnx) {
        OnnxToTRTModel(modelFile, &netEngine_);
    } else {
        EngineToTRTModel(modelFile, &netEngine_);
    }

    if (netEngine_ == nullptr) {
        std::cerr << "Failed to load ONNX file.";
    }

    netContext = netEngine_->createExecutionContext();
    if (netContext == nullptr) {
        std::cerr << "Failed to create TensorRT Execution Context.";
    }

    std::cout<<"success sparse model."<<std::endl;
}

/**************************************************************************************
功能描述: OnnxToTRTModel
修改记录:
**************************************************************************************/
void Net_Base::OnnxToTRTModel(const std::string& modelFile, nvinfer1::ICudaEngine** enginePtr)
{
    int verbosity = static_cast<int>(nvinfer1::ILogger::Severity::kWARNING);

    // create the builder
    const auto explicit_batch = static_cast<uint32_t>(batch_size) << static_cast<uint32_t>(
                                nvinfer1::NetworkDefinitionCreationFlag::kEXPLICIT_BATCH);
    nvinfer1::IBuilder* builder = nvinfer1::createInferBuilder(g_logger_);
    nvinfer1::INetworkDefinition* network = builder->createNetworkV2(explicit_batch);

    // parse onnx model
    auto parser = nvonnxparser::createParser(*network, g_logger_);
    if (!parser->parseFromFile(modelFile.c_str(), verbosity)) {
        std::string msg("failed to parse model file");
        g_logger_.log(nvinfer1::ILogger::Severity::kERROR, msg.c_str());
        exit(EXIT_FAILURE);
    }

    // Build the engine
    builder->setMaxBatchSize(batch_size);

    // builder->setHalf2Mode(true);
    nvinfer1::IBuilderConfig* config = builder->createBuilderConfig();
    config->setMaxWorkspaceSize(1 << 30);
    nvinfer1::ICudaEngine* engine = builder->buildEngineWithConfig(*network, *config);
    *enginePtr = engine;

    delete(parser);
    delete(network);
    delete(config);
    delete(builder);
}

/**************************************************************************************
功能描述: EngineToTRTModel
修改记录:
**************************************************************************************/
void Net_Base::EngineToTRTModel(const std::string& engineFile, nvinfer1::ICudaEngine** enginePtr)
{
    int verbosity = static_cast<int>(nvinfer1::ILogger::Severity::kWARNING);
    std::stringstream gieModelStream;
    gieModelStream.seekg(0, gieModelStream.beg);
    std::ifstream cache(engineFile);
    gieModelStream << cache.rdbuf();
    cache.close();
    nvinfer1::IRuntime* runtime = nvinfer1::createInferRuntime(g_logger_);

    if (runtime == nullptr) {
        std::string msg("failed to build runtime parser");
        g_logger_.log(nvinfer1::ILogger::Severity::kERROR, msg.c_str());
        exit(EXIT_FAILURE);
    }
    gieModelStream.seekg(0, std::ios::end);
    const int modelSize = gieModelStream.tellg();

    gieModelStream.seekg(0, std::ios::beg);
    void* modelMem = malloc(modelSize);
    gieModelStream.read((char*)modelMem, modelSize);

    nvinfer1::ICudaEngine* engine = runtime->deserializeCudaEngine(modelMem, modelSize, NULL);
    if (engine == nullptr) {
        std::string msg("failed to build engine parser");
        g_logger_.log(nvinfer1::ILogger::Severity::kERROR, msg.c_str());
        exit(EXIT_FAILURE);
    }

    *enginePtr = engine;
    free(modelMem);
}
