#pragma once
#include <memory>
#include <vector>
#include "pcpt_net_nms.h"

class PostprocessCuda2
{
public:
    PostprocessCuda2(const int num_threads, const float float_min, const float float_max, const int num_class,
                     const int num_anchor_per_cls, const float score_threshold, const float nms_overlap_threshold,
                     const int nms_pre_maxsize, const int nms_post_maxsize, const int num_box_corners,
                     const int num_input_box_feature, const int num_output_box_feature);
    ~PostprocessCuda2(){}

    void DoPostprocessCuda(const float* box_preds, float* dev_filtered_box, float* dev_filtered_score, int* dev_filter_count,
                           std::vector<float>& out_detection, std::vector<int>& out_label, std::vector<float>& out_score);

private:
    const int       num_threads_;
    const float     float_min_;
    const float     float_max_;
    const int       num_class_;
    const int       num_anchor_per_cls_;
    const float     score_threshold_;
    const float     nms_overlap_threshold_;
    const int       nms_pre_maxsize_;
    const int       nms_post_maxsize_;
    const int       num_box_corners_;
    const int       num_input_box_feature_;
    const int       num_output_box_feature_;

    std::unique_ptr<NmsCuda> nms_cuda_ptr_;
};
