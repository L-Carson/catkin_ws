#pragma once

class NmsCuda
{

public:

  NmsCuda(const int num_threads, const int num_box_corners,const float nms_overlap_threshold);

  /*host_filter_count过滤输出数 dev_sorted_box_for_nms 按分数排序的边界框输出 out_keep_inds 选定边界框的索引
  out_num_to_keep 保留的边界框数*/
  void DoNmsCuda(const int host_filter_count, float* dev_sorted_box_for_nms,long* out_keep_inds, int* out_num_to_keep);

 private:

  const int num_threads_;
  const int num_box_corners_;
  const float nms_overlap_threshold_;
};
