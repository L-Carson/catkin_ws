
#include <thrust/sort.h>
#include "pcpt_net_comm.h"
#include "pcpt_net_post_process.h"
#include <stdio.h>

/**************************************************************************************
功能描述: box_decode_warp
修改记录:
**************************************************************************************/
__device__ void box_decode_warp(int head_offset, const float* box_pred,int tid , int num_anchors_per_head,
                                int counter, float* filtered_box)
{
    filtered_box[blockIdx.z * num_anchors_per_head * 7  + counter * 7 + 0] = box_pred[ head_offset + tid * 7 + 0];
    filtered_box[blockIdx.z * num_anchors_per_head * 7  + counter * 7 + 1] = box_pred[ head_offset + tid * 7 + 1];
    filtered_box[blockIdx.z * num_anchors_per_head * 7  + counter * 7 + 2] = box_pred[ head_offset + tid * 7 + 2];
    filtered_box[blockIdx.z * num_anchors_per_head * 7  + counter * 7 + 3] = box_pred[ head_offset + tid * 7 + 3];
    filtered_box[blockIdx.z * num_anchors_per_head * 7  + counter * 7 + 4] = box_pred[ head_offset + tid * 7 + 4];
    filtered_box[blockIdx.z * num_anchors_per_head * 7  + counter * 7 + 5] = box_pred[ head_offset + tid * 7 + 5];
    filtered_box[blockIdx.z * num_anchors_per_head * 7  + counter * 7 + 6] = box_pred[ head_offset + tid * 7 + 6];
}


__global__ void  _filter_kernel(float* cls_pred_0,const float* box_preds,float* filtered_box,
                    float* filtered_score, int* filter_count,const float score_threshold,int num_class)
{
    int num_anchors_per_head = gridDim.x * gridDim.y * blockDim.x;
    extern __shared__ float cls_score[];
    cls_score[threadIdx.x + blockDim.x] = -1.0f;

    int tid = blockIdx.x * gridDim.y * blockDim.x + blockIdx.y *  blockDim.x + threadIdx.x;

    if ( blockIdx.z == 0) {
        cls_score[ threadIdx.x ] = 1 / (1 + expf(-cls_pred_0[ tid ]));
    }
    if ( blockIdx.z == 1) {
        cls_score[ threadIdx.x ] = 1 / (1 + expf(-cls_pred_0[ tid + num_anchors_per_head]));
    }
    if ( blockIdx.z == 2) {
        cls_score[ threadIdx.x ] = 1 / (1 + expf(-cls_pred_0[ tid + num_anchors_per_head*2]));
    }

    __syncthreads();

    if( cls_score[ threadIdx.x ] > score_threshold)
    {
        int counter = atomicAdd(&filter_count[blockIdx.z], 1);
        if ( blockIdx.z == 0) {
            box_decode_warp(0 ,box_preds , tid , num_anchors_per_head , counter , filtered_box);
            filtered_score[blockIdx.z * num_anchors_per_head + counter] = cls_score[ threadIdx.x ];
        }else
        if ( blockIdx.z == 1) {
            box_decode_warp(0 ,&box_preds[1 * 12168 * 7] , tid , num_anchors_per_head , counter , filtered_box);
            filtered_score[blockIdx.z * num_anchors_per_head + counter] = cls_score[ threadIdx.x ];
        }else
        if ( blockIdx.z == 2) {
            box_decode_warp(0 ,&box_preds[2 * 12168 * 7]  , tid , num_anchors_per_head , counter , filtered_box);
            filtered_score[blockIdx.z * num_anchors_per_head + counter] = cls_score[ threadIdx.x ];
        }
    }

}

/**************************************************************************************
功能描述: sort_boxes_by_indexes_kernel
修改记录:
**************************************************************************************/
__global__ void sort_boxes_by_indexes_kernel(float* filtered_box, float* filtered_scores, int* indexes, int filter_count,
                            float* sorted_filtered_boxes, float* sorted_filtered_scores,const int num_output_box_feature)
{
    int tid = threadIdx.x + blockIdx.x * blockDim.x;
    if(tid < filter_count)  {
        int sort_index = indexes[tid];
        sorted_filtered_boxes[tid * num_output_box_feature + 0] = filtered_box[sort_index * num_output_box_feature + 0];
        sorted_filtered_boxes[tid * num_output_box_feature + 1] = filtered_box[sort_index * num_output_box_feature + 1];
        sorted_filtered_boxes[tid * num_output_box_feature + 2] = filtered_box[sort_index * num_output_box_feature + 2];
        sorted_filtered_boxes[tid * num_output_box_feature + 3] = filtered_box[sort_index * num_output_box_feature + 3];
        sorted_filtered_boxes[tid * num_output_box_feature + 4] = filtered_box[sort_index * num_output_box_feature + 4];
        sorted_filtered_boxes[tid * num_output_box_feature + 5] = filtered_box[sort_index * num_output_box_feature + 5];
        sorted_filtered_boxes[tid * num_output_box_feature + 6] = filtered_box[sort_index * num_output_box_feature + 6];
        sorted_filtered_scores[tid] = filtered_scores[tid];
    }
}

/**************************************************************************************
功能描述: PostprocessCuda
修改记录:
**************************************************************************************/
PostprocessCuda::PostprocessCuda(const int num_threads, const float float_min, const float float_max,
                        const int num_class, const int num_anchor_per_cls, const float score_threshold,
                        const float nms_overlap_threshold, const int nms_pre_maxsize, const int nms_post_maxsize,
                        const int num_box_corners, const int num_input_box_feature, const int num_output_box_feature)
    : num_threads_(num_threads)
    , float_min_(float_min)
    , float_max_(float_max)
    , num_class_(num_class)
    , num_anchor_per_cls_(num_anchor_per_cls)
    , score_threshold_(score_threshold)
    , nms_overlap_threshold_(nms_overlap_threshold)
    , nms_pre_maxsize_(nms_pre_maxsize)
    , nms_post_maxsize_(nms_post_maxsize)
    , num_box_corners_(num_box_corners)
    , num_input_box_feature_(num_input_box_feature)
    , num_output_box_feature_(num_output_box_feature)
{
    nms_cuda_ptr_.reset(new NmsCuda(num_threads_, num_box_corners_, nms_overlap_threshold_));
}


/**************************************************************************************
功能描述: DoPostprocessCuda
修改记录:
**************************************************************************************/
void PostprocessCuda::DoPostprocessCuda(float* cls_pred_0, const float* box_preds, float* dev_filtered_box,
                            float* dev_filtered_score, int* dev_filter_count, std::vector<float>& out_detection,
                            std::vector<int>& out_label, std::vector<float>& out_score)
{
    dim3 gridsize(18, 4 , num_class_);
    _filter_kernel<<< gridsize, 169 , 169 * 2 * sizeof(float)>>>(cls_pred_0,
        box_preds,dev_filtered_box, dev_filtered_score, dev_filter_count, score_threshold_,num_class_);
    cudaDeviceSynchronize();
    int host_filter_count[num_class_] = {0};
    GPU_CHECK(cudaMemcpy(host_filter_count, dev_filter_count, num_class_ * sizeof(int), cudaMemcpyDeviceToHost));

    for (int i = 0; i < num_class_; ++ i) {
        if(host_filter_count[i] <= 0) continue;
        int* dev_indexes;
        float* dev_sorted_filtered_box;
        float* dev_sorted_filtered_scores;
        GPU_CHECK(cudaMalloc((void**)&dev_indexes, host_filter_count[i] * sizeof(int)));
        GPU_CHECK(cudaMalloc((void**)&dev_sorted_filtered_box, host_filter_count[i] * num_output_box_feature_ * sizeof(float)));
        GPU_CHECK(cudaMalloc((void**)&dev_sorted_filtered_scores, host_filter_count[i]*sizeof(float)));

        thrust::sequence(thrust::device, dev_indexes, dev_indexes + host_filter_count[i]);
        thrust::sort_by_key(thrust::device, &dev_filtered_score[i * num_anchor_per_cls_],
                            &dev_filtered_score[i * num_anchor_per_cls_ + host_filter_count[i]],
                            dev_indexes, thrust::greater<float>());
        const int num_blocks = DIVUP(host_filter_count[i], num_threads_);

        sort_boxes_by_indexes_kernel<<<num_blocks, num_threads_>>>(
                            &dev_filtered_box[i * num_anchor_per_cls_ * num_output_box_feature_],
                            &dev_filtered_score[i * num_anchor_per_cls_],dev_indexes,host_filter_count[i],
                            dev_sorted_filtered_box,dev_sorted_filtered_scores,num_output_box_feature_);

        int num_box_for_nms = min(nms_pre_maxsize_, host_filter_count[i]);
        long* keep_inds = new long[num_box_for_nms];
        memset(keep_inds, 0, num_box_for_nms * sizeof(int));
        int num_out = 0;
        nms_cuda_ptr_->DoNmsCuda(num_box_for_nms, dev_sorted_filtered_box, keep_inds, &num_out);

        num_out = min(num_out, nms_post_maxsize_);
        float* host_filtered_box    = new float[host_filter_count[i] * num_output_box_feature_]();
        float* host_filtered_scores = new float[host_filter_count[i]]();

        cudaMemcpy(host_filtered_box, dev_sorted_filtered_box,
                   host_filter_count[i] * num_output_box_feature_ * sizeof(float),
                   cudaMemcpyDeviceToHost);
        cudaMemcpy(host_filtered_scores, dev_sorted_filtered_scores,
                   host_filter_count[i] * sizeof(float),
                   cudaMemcpyDeviceToHost);

        for (int j = 0; j < num_out; ++j) {
            out_detection.emplace_back(host_filtered_box[keep_inds[j] * num_output_box_feature_ + 0]);
            out_detection.emplace_back(host_filtered_box[keep_inds[j] * num_output_box_feature_ + 1]);
            out_detection.emplace_back(host_filtered_box[keep_inds[j] * num_output_box_feature_ + 2]);
            out_detection.emplace_back(host_filtered_box[keep_inds[j] * num_output_box_feature_ + 3]);
            out_detection.emplace_back(host_filtered_box[keep_inds[j] * num_output_box_feature_ + 4]);
            out_detection.emplace_back(host_filtered_box[keep_inds[j] * num_output_box_feature_ + 5]);
            out_detection.emplace_back(host_filtered_box[keep_inds[j] * num_output_box_feature_ + 6]);
            out_score.emplace_back(host_filtered_scores[keep_inds[j]]);
            out_label.emplace_back(i);
        }

        delete[] keep_inds;
        delete[] host_filtered_scores;
        delete[] host_filtered_box;

        GPU_CHECK(cudaFree(dev_indexes));
        GPU_CHECK(cudaFree(dev_sorted_filtered_box));
        GPU_CHECK(cudaFree(dev_sorted_filtered_scores));
    }
}
