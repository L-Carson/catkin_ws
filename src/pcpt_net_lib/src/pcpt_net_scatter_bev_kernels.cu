
#include <iostream>

#include <cuda_runtime_api.h>

#include "pcpt_net_scatter_bev_kernels.h"

/**************************************************************************************
功能描述: scatterBEV_kernel
修改记录:
**************************************************************************************/
__global__ void scatterBEV_kernel(const float *pillar_features_data,const float *coords_data, const unsigned int *params_data,
                                  unsigned int featureX, unsigned int featureY,float *spatial_feature_data)
{
  int pillar_idx = blockIdx.x * PILLARS_PER_BLOCK + threadIdx.x;
  int valid_pillars_inBlock = PILLARS_PER_BLOCK;

  const int num_pillars = params_data[4];
  if ((blockIdx.x * PILLARS_PER_BLOCK) > num_pillars) return;
  int valid_blocks = (num_pillars+PILLARS_PER_BLOCK-1)/PILLARS_PER_BLOCK;
  if(blockIdx.x >= valid_blocks) return;

  if(blockIdx.x == (valid_blocks-1))
  {
    valid_pillars_inBlock = num_pillars % PILLARS_PER_BLOCK;
  }

  valid_pillars_inBlock = (valid_pillars_inBlock==0) ? PILLARS_PER_BLOCK : valid_pillars_inBlock;

  __shared__ float pillarSM[PILLARS_PER_BLOCK][FEATURE_SIZE];

  for (int i = 0; i < valid_pillars_inBlock; i++)
  {
    pillarSM[i][threadIdx.x] = pillar_features_data[ (blockIdx.x * PILLARS_PER_BLOCK +i)*FEATURE_SIZE + threadIdx.x];
  }

  __syncthreads();

  if(pillar_idx >= num_pillars) return;

  float4 coord = ((const float4 *)coords_data)[pillar_idx];

  int x = (int)coord.w;
  int y = (int)coord.z;

  for (int i = 0; i < FEATURE_SIZE; i++)
  {
    spatial_feature_data[i*featureY*featureX + y*featureX + x] = pillarSM[threadIdx.x][i];
  }

}

/**************************************************************************************
功能描述: scatterBEV_kernel_launcher
修改记录:
**************************************************************************************/
cudaError_t scatterBEV_kernel_launcher(const float *pillar_features_data,const float *coords_data,const unsigned int *params_data,
                              unsigned int featureX, unsigned int featureY,float *spatial_feature_data,cudaStream_t stream)
{

  dim3 blocks((featureX * featureY + PILLARS_PER_BLOCK - 1) / PILLARS_PER_BLOCK);
  dim3 threads(PILLARS_PER_BLOCK);

  scatterBEV_kernel<<<blocks, threads, 0, stream>>>(pillar_features_data, coords_data, params_data,
                                                   featureX, featureY, spatial_feature_data);
  return cudaGetLastError();
}

/**************************************************************************************
功能描述: reduceMax_kernel
修改记录:
**************************************************************************************/
__global__ void reduceMax_kernel(const float *in,float *out, unsigned int pillarCount)
{
  int pillar_idx = blockIdx.x ;
  if(pillar_idx >= pillarCount) return;
  int feature_idx =threadIdx.x;
  unsigned int indexIn = pillar_idx * POINTS_PER_PILLAR*FEATURE_SIZE + feature_idx * FEATURE_SIZE;
  float maxFeature =0;
  for (int i = 0; i < POINTS_PER_PILLAR; i++)
  {
    indexIn = pillar_idx * POINTS_PER_PILLAR * FEATURE_SIZE + i * FEATURE_SIZE + feature_idx;
    float currentFeature = *((float*)(in + indexIn));
    maxFeature = max(maxFeature, currentFeature);
  }
  unsigned int indexOut = pillar_idx*FEATURE_SIZE + feature_idx;
  *((float *)(out + indexOut)) = max(maxFeature, 0.0);
}

/**************************************************************************************
功能描述: reduceMax_kernel_launcher
修改记录:
**************************************************************************************/
cudaError_t reduceMax_kernel_launcher(const float *in,float *out, unsigned int pillarCount,cudaStream_t stream)
{
  dim3 threads(FEATURE_SIZE);
  dim3 blocks(pillarCount);
  reduceMax_kernel<<<blocks, threads, 0, stream>>>(in, out, pillarCount);
  return cudaGetLastError();
}
