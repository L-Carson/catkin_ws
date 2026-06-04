#pragma once

/**************************************************************************************
功能描述: PreprocessPointsCuda
修改记录:
**************************************************************************************/
class PreprocessPointsCuda
{
public:
    /* num_threads 线程数  num_inds_for_scan 扫描索引数（总和） min_x_range 点云检测范围 */
    PreprocessPointsCuda(const int num_threads, const int num_point_feature, const int num_inds_for_scan,
                         const int max_num_pillars, const int max_points_per_pillar,
                         const int grid_x_size, const int grid_y_size, const int grid_z_size,  // grid size
                         const float pillar_x_size, const float pillar_y_size, const float pillar_z_size, //voxel size
                         const float min_x_range, const float min_y_range, const float min_z_range); // point cloud range
    ~PreprocessPointsCuda();

    /**
     * dev_points 输入点云 in_num_points 点数 dev_x_coors 相应点柱的过滤输出X坐标索引数 dev_y_coors 相应点柱的过滤输出Y坐标索引数
     * dev_num_points_per_pillar 点柱点数 dev_pillar_point_feature 每个点柱特征 点柱阵列 dev_sparse_pillar_map 点柱占用的栅格地图
     * host_pillar_count 点柱有效数量  dev_pfe_gather_feature pfe输入通道的11维特征
     */
    void DoPreprocessPointsCuda(const float* dev_points, const int in_num_points, int* dev_x_coors, int* dev_y_coors,
                                float* dev_num_points_per_pillar, float* dev_pillar_point_feature, float* dev_pillar_coors,
                                int* dev_sparse_pillar_map, int* host_pillar_count, float* dev_pfe_gather_feature);

private:
    const int   num_threads_;
    const int   max_num_pillars_;
    const int   max_num_points_per_pillar_;
    const int   num_point_feature_;
    const int   num_inds_for_scan_;
    const int   grid_x_size_;
    const int   grid_y_size_;
    const int   grid_z_size_;
    const float pillar_x_size_;
    const float pillar_y_size_;
    const float pillar_z_size_;
    const float min_x_range_;
    const float min_y_range_;
    const float min_z_range_;

    float*      dev_pillar_point_feature_in_coors_;
    int*        dev_pillar_count_histo_;

    int*        dev_counter_;
    int*        dev_pillar_count_;
    float*      dev_points_mean_;
};
