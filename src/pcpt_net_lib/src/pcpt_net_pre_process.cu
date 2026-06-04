#include <stdio.h>
#include "pcpt_net_comm.h"
#include "pcpt_net_pre_process.h"

/**************************************************************************************
功能描述: make_pillar_histo_kernel
修改记录:
**************************************************************************************/
__global__ void make_pillar_histo_kernel(const float* dev_points, float* dev_pillar_point_feature_in_coors,
                  int* pillar_count_histo, const int num_points,const int max_points_per_pillar,
                  const int grid_x_size,const int grid_y_size, const int grid_z_size, const float min_x_range,
                  const float min_y_range, const float min_z_range, const float pillar_x_size,
                  const float pillar_y_size, const float pillar_z_size,const int num_point_feature)
{
    int th_i = blockIdx.x * blockDim.x +  threadIdx.x ;
    if (th_i >= num_points) return;
    int x_coor = floor((dev_points[th_i * num_point_feature + 0] - min_x_range) / pillar_x_size);
    int y_coor = floor((dev_points[th_i * num_point_feature + 1] - min_y_range) / pillar_y_size);
    int z_coor = floor((dev_points[th_i * num_point_feature + 2] - min_z_range) / pillar_z_size);

    if (x_coor >= 0 && x_coor < grid_x_size && y_coor >= 0 &&
        y_coor < grid_y_size && z_coor >= 0 && z_coor < grid_z_size) {
        int count = atomicAdd(&pillar_count_histo[y_coor * grid_x_size + x_coor], 1);
        if (count < max_points_per_pillar) {
            int ind = y_coor * grid_x_size * max_points_per_pillar * num_point_feature +
            x_coor * max_points_per_pillar * num_point_feature + count * num_point_feature;

            for (int i = 0; i < num_point_feature; ++i) {
                dev_pillar_point_feature_in_coors[ind + i] = dev_points[th_i * num_point_feature + i];
            }
        }
    }
}

/**************************************************************************************
功能描述: make_pillar_index_kernel
修改记录:
**************************************************************************************/
__global__ void make_pillar_index_kernel(int* dev_pillar_count_histo, int* dev_counter, int* dev_pillar_count,int* dev_x_coors,
                      int* dev_y_coors, float* dev_num_points_per_pillar,int* dev_sparse_pillar_map,const int max_pillars,
                      const int max_points_per_pillar, const int grid_x_size,const int num_inds_for_scan)
{
    int x = blockIdx.x;
    int y = threadIdx.x;
    int num_points_at_this_pillar = dev_pillar_count_histo[y * grid_x_size + x];
    if (num_points_at_this_pillar == 0) return;

    int count = atomicAdd(dev_counter, 1);
    if (count < max_pillars) {
        atomicAdd(dev_pillar_count, 1);
        if (num_points_at_this_pillar >= max_points_per_pillar) {
            dev_num_points_per_pillar[count] = max_points_per_pillar;
        } else {
            dev_num_points_per_pillar[count] = num_points_at_this_pillar;
        }
        dev_x_coors[count] = x;
        dev_y_coors[count] = y;
        dev_sparse_pillar_map[y * num_inds_for_scan + x] = 1;
    }
}

/**************************************************************************************
功能描述: make_pillar_feature_kernel
修改记录:
**************************************************************************************/
__global__ void make_pillar_feature_kernel(float* dev_pillar_point_feature_in_coors, float* dev_pillar_point_feature,
                      float* dev_pillar_coors, int* dev_x_coors, int* dev_y_coors,float* dev_num_points_per_pillar,
                      const int max_points,const int num_point_feature, const int grid_x_size)
{
    int ith_pillar = blockIdx.x;
    int num_points_at_this_pillar = dev_num_points_per_pillar[ith_pillar];
    int ith_point = threadIdx.x;
    if (ith_point >= num_points_at_this_pillar) return;

    int x_ind      = dev_x_coors[ith_pillar];
    int y_ind      = dev_y_coors[ith_pillar];
    int pillar_ind = ith_pillar * max_points * num_point_feature + ith_point * num_point_feature;
    int coors_ind  = y_ind * grid_x_size * max_points * num_point_feature + x_ind * max_points * num_point_feature +
                     ith_point * num_point_feature;
#pragma unroll
    for (int i = 0; i < num_point_feature; ++i) {
        dev_pillar_point_feature[pillar_ind + i] = dev_pillar_point_feature_in_coors[coors_ind + i];
    }

    float coor_x = static_cast<float>(x_ind);
    float coor_y = static_cast<float>(y_ind);
    dev_pillar_coors[ith_pillar * 4 + 0] = 0;  // batch idx
    dev_pillar_coors[ith_pillar * 4 + 1] = 0;  // z
    dev_pillar_coors[ith_pillar * 4 + 2] = coor_y;
    dev_pillar_coors[ith_pillar * 4 + 3] = coor_x;
}

/**************************************************************************************
功能描述: pillar_mean_kernel
修改记录:
**************************************************************************************/
__global__ void pillar_mean_kernel(float* dev_points_mean, const int num_point_feature,const float* dev_pillar_point_feature,
                                  const float* dev_num_points_per_pillar,int max_pillars ,int max_points_per_pillar)
{
    extern __shared__ float temp[];
    int ith_pillar  = blockIdx.x;
    int ith_point   = threadIdx.x;
    int axis        = threadIdx.y;

    int reduce_size = max_points_per_pillar > 32 ? 64 : 32;
    temp[threadIdx.x * 3 + axis] =  dev_pillar_point_feature[ith_pillar * max_points_per_pillar * num_point_feature +
                                    ith_point * num_point_feature + axis];
    if (threadIdx.x < reduce_size - max_points_per_pillar) {
        temp[(threadIdx.x + max_points_per_pillar) * 3 + axis] = 0.0f; //--> dummy placeholds will set as 0
    }
    __syncthreads();
    int num_points_at_this_pillar = dev_num_points_per_pillar[ith_pillar];

    if (ith_point >= num_points_at_this_pillar) {
          return;
    }

    for (unsigned int d = reduce_size >> 1 ; d > 0.6; d >>= 1) {
        if (ith_point < d) {
            temp[ith_point*3 +axis] += temp[(ith_point + d) * 3 + axis];
        }
        __syncthreads();
    }

    if (ith_point == 0) {
        dev_points_mean[ith_pillar * 3 + axis] = temp[ith_point + axis] / num_points_at_this_pillar ;
    }
}

/**************************************************************************************
功能描述: gather_point_feature_kernel
修改记录:
**************************************************************************************/
__global__ void gather_point_feature_kernel(const int max_num_pillars_,const int max_num_points_per_pillar,
                        const int num_point_feature,const float min_x_range, const float min_y_range, const float min_z_range,
                        const float pillar_x_size,  const float pillar_y_size, const float pillar_z_size,
                        const float* dev_pillar_point_feature, const float* dev_num_points_per_pillar,
                        const float* dev_pillar_coors,float* dev_points_mean, float* dev_pfe_gather_feature)
{
    int ith_pillar = blockIdx.x;
    int ith_point = threadIdx.x;
    int num_gather_feature = 11;
    int num_points_at_this_pillar = dev_num_points_per_pillar[ith_pillar];

    if (ith_point >= num_points_at_this_pillar) return;

    dev_pfe_gather_feature[ith_pillar * max_num_points_per_pillar * num_gather_feature + ith_point * num_gather_feature + 0]
    =  dev_pillar_point_feature[ith_pillar * max_num_points_per_pillar * num_point_feature + ith_point * num_point_feature + 0];

    dev_pfe_gather_feature[ith_pillar * max_num_points_per_pillar * num_gather_feature + ith_point * num_gather_feature + 1]
    =  dev_pillar_point_feature[ith_pillar * max_num_points_per_pillar * num_point_feature + ith_point * num_point_feature + 1];

    dev_pfe_gather_feature[ith_pillar * max_num_points_per_pillar * num_gather_feature + ith_point * num_gather_feature + 2]
    =  dev_pillar_point_feature[ith_pillar * max_num_points_per_pillar * num_point_feature + ith_point * num_point_feature + 2];

    dev_pfe_gather_feature[ith_pillar * max_num_points_per_pillar * num_gather_feature + ith_point * num_gather_feature + 3]
    =  dev_pillar_point_feature[ith_pillar * max_num_points_per_pillar * num_point_feature + ith_point * num_point_feature + 3];

    dev_pfe_gather_feature[ith_pillar * max_num_points_per_pillar * num_gather_feature + ith_point * num_gather_feature + 4]
    =  dev_pillar_point_feature[ith_pillar * max_num_points_per_pillar * num_point_feature + ith_point * num_point_feature + 4];

    dev_pfe_gather_feature[ith_pillar * max_num_points_per_pillar * num_gather_feature + ith_point * num_gather_feature + 5]
    =  dev_pillar_point_feature[ith_pillar * max_num_points_per_pillar * num_point_feature + ith_point * num_point_feature + 0] -
       dev_points_mean[ith_pillar * 3 + 0 ];

    dev_pfe_gather_feature[ith_pillar * max_num_points_per_pillar * num_gather_feature + ith_point * num_gather_feature + 6]
    =  dev_pillar_point_feature[ith_pillar * max_num_points_per_pillar * num_point_feature + ith_point * num_point_feature + 1] -
       dev_points_mean[ith_pillar * 3 + 1 ];

    dev_pfe_gather_feature[ith_pillar * max_num_points_per_pillar * num_gather_feature + ith_point * num_gather_feature + 7]
    =  dev_pillar_point_feature[ith_pillar * max_num_points_per_pillar * num_point_feature + ith_point * num_point_feature + 2] -
       dev_points_mean[ith_pillar * 3 + 2 ];

    dev_pfe_gather_feature[ith_pillar * max_num_points_per_pillar * num_gather_feature + ith_point * num_gather_feature + 8]
    =  dev_pillar_point_feature[ith_pillar * max_num_points_per_pillar * num_point_feature + ith_point * num_point_feature + 0] -
       (dev_pillar_coors[ith_pillar * 4 + 3] * pillar_x_size + (pillar_x_size/2 + min_x_range));

    dev_pfe_gather_feature[ith_pillar * max_num_points_per_pillar * num_gather_feature + ith_point * num_gather_feature + 9]
    =  dev_pillar_point_feature[ith_pillar * max_num_points_per_pillar * num_point_feature + ith_point * num_point_feature + 1] -
       (dev_pillar_coors[ith_pillar * 4 + 2] * pillar_y_size + (pillar_y_size/2 + min_y_range));

    dev_pfe_gather_feature[ith_pillar * max_num_points_per_pillar * num_gather_feature + ith_point * num_gather_feature + 10]
    =  dev_pillar_point_feature[ith_pillar * max_num_points_per_pillar * num_point_feature + ith_point * num_point_feature + 2] -
       (dev_pillar_coors[ith_pillar * 4 + 1] * pillar_z_size + (pillar_z_size/2 + min_z_range));

}

/**************************************************************************************
功能描述: PreprocessPointsCuda
修改记录:
**************************************************************************************/
PreprocessPointsCuda::PreprocessPointsCuda(const int num_threads, const int max_num_pillars,
                                  const int max_points_per_pillar, const int num_point_feature,
                                  const int num_inds_for_scan, const int grid_x_size, const int grid_y_size,
                                  const int grid_z_size, const float pillar_x_size, const float pillar_y_size,
                                  const float pillar_z_size, const float min_x_range, const float min_y_range,
                                  const float min_z_range)
    : num_threads_(num_threads)
    , max_num_pillars_(max_num_pillars)
    , max_num_points_per_pillar_(max_points_per_pillar)
    , num_point_feature_(num_point_feature)
    , num_inds_for_scan_(num_inds_for_scan)
    , grid_x_size_(grid_x_size)
    , grid_y_size_(grid_y_size)
    , grid_z_size_(grid_z_size)
    , pillar_x_size_(pillar_x_size)
    , pillar_y_size_(pillar_y_size)
    , pillar_z_size_(pillar_z_size)
    , min_x_range_(min_x_range)
    , min_y_range_(min_y_range)
    , min_z_range_(min_z_range)
{
    GPU_CHECK(cudaMalloc(reinterpret_cast<void**>(&dev_pillar_point_feature_in_coors_),
        grid_y_size_ * grid_x_size_ * max_num_points_per_pillar_ *  num_point_feature_ * sizeof(float)));
    GPU_CHECK(cudaMalloc(reinterpret_cast<void**>(&dev_pillar_count_histo_),grid_y_size_ * grid_x_size_ * sizeof(int)));
    GPU_CHECK(cudaMalloc(reinterpret_cast<void**>(&dev_counter_), sizeof(int)));
    GPU_CHECK(cudaMalloc(reinterpret_cast<void**>(&dev_pillar_count_), sizeof(int)));
    GPU_CHECK(cudaMalloc(reinterpret_cast<void**>(&dev_points_mean_), max_num_pillars_ * 3 *sizeof(float)));
}

/**************************************************************************************
功能描述: ～PreprocessPointsCuda
修改记录:
**************************************************************************************/
PreprocessPointsCuda::~PreprocessPointsCuda()
{
    GPU_CHECK(cudaFree(dev_pillar_point_feature_in_coors_));
    GPU_CHECK(cudaFree(dev_pillar_count_histo_));
    GPU_CHECK(cudaFree(dev_counter_));
    GPU_CHECK(cudaFree(dev_pillar_count_));
    GPU_CHECK(cudaFree(dev_points_mean_));
}

/**************************************************************************************
功能描述: DoPreprocessPointsCuda
修改记录:
**************************************************************************************/
void PreprocessPointsCuda::DoPreprocessPointsCuda(const float* dev_points, const int in_num_points, int* dev_x_coors,
            int* dev_y_coors, float* dev_num_points_per_pillar, float* dev_pillar_point_feature, float* dev_pillar_coors,
            int* dev_sparse_pillar_map, int* host_pillar_count, float* dev_pfe_gather_feature)
{
    /* initialize paraments */
    GPU_CHECK(cudaMemset(dev_pillar_point_feature_in_coors_, 0 , grid_y_size_ * grid_x_size_ *
                        max_num_points_per_pillar_ *  num_point_feature_ * sizeof(float)));
    GPU_CHECK(cudaMemset(dev_pillar_count_histo_, 0 , grid_y_size_ * grid_x_size_ * sizeof(int)));
    GPU_CHECK(cudaMemset(dev_counter_, 0, sizeof(int)));
    GPU_CHECK(cudaMemset(dev_pillar_count_, 0, sizeof(int)));
    GPU_CHECK(cudaMemset(dev_points_mean_, 0,  max_num_pillars_ * 3 * sizeof(float)));
    int num_block = DIVUP(in_num_points , num_threads_);

    /*1 输出coors中的支柱点特征 dev_pillar_point_feature_in_coors_ */
    make_pillar_histo_kernel<<<num_block , num_threads_>>>(
                        dev_points, dev_pillar_point_feature_in_coors_, dev_pillar_count_histo_,
                        in_num_points, max_num_points_per_pillar_, grid_x_size_, grid_y_size_,
                        grid_z_size_, min_x_range_, min_y_range_, min_z_range_, pillar_x_size_,
                        pillar_y_size_, pillar_z_size_, num_point_feature_);

    /*2 生成点柱 dev_x_coors,ev_y_coors, dev_num_points_per_pillar 的索引*/
    make_pillar_index_kernel<<<grid_x_size_, grid_y_size_>>>(
                        dev_pillar_count_histo_, dev_counter_, dev_pillar_count_, dev_x_coors,
                        dev_y_coors, dev_num_points_per_pillar, dev_sparse_pillar_map,
                        max_num_pillars_, max_num_points_per_pillar_, grid_x_size_,num_inds_for_scan_);

    GPU_CHECK(cudaMemcpy(host_pillar_count, dev_pillar_count_, 1 * sizeof(int),cudaMemcpyDeviceToHost));

    /*3 生成 dev_pillar_point_feature,dev_pillar_coors 特征*/
    make_pillar_feature_kernel<<<host_pillar_count[0],max_num_points_per_pillar_>>>(
                        dev_pillar_point_feature_in_coors_, dev_pillar_point_feature,
                        dev_pillar_coors, dev_x_coors, dev_y_coors, dev_num_points_per_pillar,
                        max_num_points_per_pillar_, num_point_feature_, grid_x_size_);


    /*4 点柱均值 dev_points_mean_ */
    dim3 mean_block(max_num_points_per_pillar_,3);
    pillar_mean_kernel<<<host_pillar_count[0],mean_block,64 * 3 *sizeof(float)>>>(dev_points_mean_  ,num_point_feature_,
                        dev_pillar_point_feature, dev_num_points_per_pillar, max_num_pillars_ , max_num_points_per_pillar_);

    /*5 合并点特征 dev_pfe_gather_feature */
    gather_point_feature_kernel<<<max_num_pillars_, max_num_points_per_pillar_>>>(
                        max_num_pillars_,max_num_points_per_pillar_,num_point_feature_,
                        min_x_range_, min_y_range_, min_z_range_,
                        pillar_x_size_, pillar_y_size_, pillar_z_size_,
                        dev_pillar_point_feature, dev_num_points_per_pillar, dev_pillar_coors,
                        dev_points_mean_,dev_pfe_gather_feature);
}


