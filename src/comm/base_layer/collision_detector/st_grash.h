#include <map>

#include "comm_layer/comm_base/comm_type.h"
#include "comm_layer/comm_geometry/comm_base.h"
#include "comm_layer/comm_math/comm_math.h"
#include "comm_layer/comm_geometry/oriented_box.h"
#include "base_layer/vehicle_model/car_body.h"



class STGrash
{
public:
    STGrash(const std::vector<TrajPoint> &traj_points,
            const std::vector<PredictObst> &predict_obsts)
        : traj_points_(traj_points), predict_obsts_(predict_obsts)
    {
        Init();
    }

    void Init();
    void UpdateTrajPoints(const std::vector<TrajPoint> &traj_points);
    void UpdatePredictObsts(const std::vector<PredictObst> &predict_obsts);

    void GetObstVertex(cosnt unsigned int& obst_id, std::vector<STPoint>& obst_st_vertex);

    void GetTrajSTPoints(std::vector<STPoint>& traj_st_points);

private:
    void CalaculateTrajSTPoints();
    void CalaculateObstSTRect();
    
private:
    float max_t_{10.0f};    // t最大值，横坐标t的取值范围
    std::vector<STPoint> traj_st_points_;   // 轨迹的st点集
    std::unordered_map<unsigned int, std::vector<STPoint>> obst_st_vertex_map_; // ST顶点形式的障碍物索引
    std::vector<PredictObst> predict_obsts_; // 预测的障碍物
    std::vector<TrajPoint> traj_points_; // 轨迹
}