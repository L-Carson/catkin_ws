#ifndef CAR_BODY_H
#define CAR_BODY_H

#include <vector>
#include <string>

#include "comm_layer/comm_base/comm_type.h"
#include "comm_layer/comm_geometry/comm_geometry.h"


// 车辆类型
enum class CarTypeEnum
{
    S310 = 0,      // 小蜗牛车型

    S320 = 1,      // 降本前车型
    S323 = 2,      // 标准版：前扫刷不可左右伸缩
    S324 = 3,      // 标准版：前扫刷可左右伸缩
    S325 = 4,      // 侨银定制
    S326 = 5,      // 垃圾打包车型（基于S324）

    S330 = 6,      // 降本前车型
    S332 = 7,      // 一字尾翼
    S333 = 8,      // V 形尾翼
    S334 = 9,      // V 形尾翼（展开宽度变窄）
    S335 = 10,      // V 形尾翼（带有拖挂装置）

    S350 = 11,      // 350车型

    S520 = 12,      // 中型车（1吨）
    S530 = 13,      // 中型车（3吨）

    NULL       // 必须在最后面
};


// 碰撞区域
enum class CollideRegionTypeEnum
{
    NONE = 0,          // 无区域
    CAR_LEFT = 1,      // 车体左
    CAR_HEAD = 2,      // 车体前
    CAR_RIGHT = 3,     // 车体右
    CAR_CENTER = 4,    // 车体正中
    CAR_BACK = 5,      // 车体后

    NULL       // 必须在最后面
};

struct CollideRegion
{
    CollideRegionTypeEnum region_type;
    std::vector<Point2D> vectexs; // 顶点
}

// 立方体参数结构
struct Cube
{
    float length; // 立方体长
    float width;  // 立方体宽
    float height; // 立方体高
};

struct CarPartsCube
{
    Pose2D center_pose; // 立方体中心坐标
    Cube cube;           // 立方体参数
};

// 车体基本参数
struct CarBodyBaseParam {
    CarTypeEnum car_type;               // 车辆类型
    std::string car_type_name;          // 车辆名字
    float ego_length;                   // 车身长度
    float ego_width;                    // 车身宽度
    float ego_height;                   // 车身高度
    float wheel_base_length;            // 轴距
    float back_wheel_width;             // 后轮轮距
    float tail_to_rear_axle_lenght;     // 车尾到后轴的距离
    float brush_fold_length;            // 扫刷收起长度
    float brush_fold_width;             // 扫刷收起宽度
    float brush_spread_length;          // 扫刷展开长度
    float brush_spread_width;           // 扫刷展开宽度
    float brush_body_height;            // 扫刷结构高度
    float brush_disk_rudia;             // 刷盘半径
    float brush_feather_length;         // 刷毛长度
    float brush_disk_height;            // 刷盘高度
    float max_steer_angle;              // 最大转向角度
};

// 刷盘结构基本参数
struct BrushDiskCylinder
{
    Point2D center_pose; // 刷盘中心坐标
    float rudia;         // 刷盘半径
    float height;        // 刷盘高度
};


// 车身
class CarBody
{
public:
    CarBody(void)
    {
        InitModel(void);
    }

    virtual ~CarBody()
    {
    }

    static void InitModel(void);
    static const CarBodyBaseParam *GetCarBodyBasePtr(const std::string &car_type_name);

    // 处理车型信息
    bool   IsS310(void) const;  /* 判断是否是某种车型，待优化，应用方不能按照类型来进行区分处理，应该修改按能力和配置进行处理。 */
    bool   IsS320(void) const;
    bool   IsS323(void) const;
    bool   IsS324(void) const;
    bool   IsS325(void) const;
    bool   IsS326(void) const;
    bool   IsS330(void) const;
    bool   IsS332(void) const;
    bool   IsS333(void) const;
    bool   IsS334(void) const;
    bool   IsS335(void) const;
    bool   IsS350(void) const;
    bool   IsS520(void) const;
    bool   IsS530(void) const;

    // 设置当前车辆自身的位姿
    void SetEgoPose(const Pose2D &ego_pose);

    // 获取车辆当前位置
    Pose2D GetEgoPose() const;

    // 获取车体的高度
    float GetCarBodyHeight() const;

    // 获取车体的宽度
    float GetCarBodyWidth() const;

    // 获取扫刷结构的高度
    float GetBrushBodyHeight() const;

    // 车尾到后轴距离
    float GetTailToRearAxleLength() const;

    // 获取轴距
    float GetWheelBaseLength() const;

    // 获取车辆最小转弯半径
    float GetMinTurnRadius() const;

    // 获取车辆最大转弯角
    float GetMaxSteerAngle() const;

    // 获取车轮模型的立方体
    Cube GetCarWheelCubeByCar(void) const;

    // 获取车体模型的立方体
    Cube GetCarBodyCubeByCar(void) const;

    // 获取车轮模型的四个角点（车身坐标系）
    void Get4CornerWheelModelByCar(Point2D &left_head_point, Point2D &left_back_point,
                                         Point2D &right_back_point, Point2D &right_head_point) const;

    // 获取车轮模型的四个角点（全局地图坐标系）
    void Get4CornerWheelModelByMap(Point2D &left_head_point, Point2D &left_back_point,
                                         Point2D &right_back_point, Point2D &right_head_point) const;

    // 获取车体模型的四个角点（车身坐标系）
    void Get4CornerCarModelByCar(Point2D &left_head_point, Point2D &left_back_point,
                                       Point2D &right_back_point, Point2D &right_head_point) const;

    // 获取车体模型的四个角点（全局地图坐标系）
    void Get4CornerCarModelByMap(Point2D &left_head_point, Point2D &left_back_point,
                                       Point2D &right_back_point, Point2D &right_head_point) const;

protected:
    Pose2D ego_pose_;

private:
    static const CarBodyBaseParam   all_car_body_base_param_[];
    static CarBodyBaseParam         car_body_base_param_;

};

#endif // CAR_BODY_H

