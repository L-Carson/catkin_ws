#ifndef DYNAMIC_CAR_H
#define DYNAMIC_CAR_H

#include <vector>
#include <string>

#include "comm_layer/comm_base/comm_type.h"
#include "comm_layer/comm_geometry/comm_geometry.h"
#include "base_layer/vehicle_model/car_body.h"


class DynamicCar : public CarBody {
public: 
    DynamicCar(void);
    virtual ~DynamicCar()
    {
    }

    // 根据扫刷展开的角度更新扫刷结构的长宽参数
    void SetBrushTheta(const float &theta);

    // 获取左刷盘的圆柱形模型
    const BrushDiskCylinder GetLeftDiskCylinderByCar(void) const;
    const BrushDiskCylinder GetLeftDiskCylinderByMap(void) const;

    // 获取右刷盘的圆柱形模型
    const BrushDiskCylinder GetRightDiskCylinderByCar(void) const;
    const BrushDiskCylinder GetRightDiskCylinderByMap(void) const;


    // 获取刷盘的扫刷结构立方体(宽度包括刷盘半径)
    CarPartsCube GetBrushCubeByCar(void) const;

    // 获取刷毛的扫刷结构立方体(宽度包括刷盘半径和刷毛)
    CarPartsCube GetBrushFeatherCubeByCar(void) const;

    // 获取扫刷结构模型四个角点(含刷盘半径)
    const void Get4CornerBrushByCar(Point2D &left_head_point, Point2D &left_back_point,
                                       Point2D &right_back_point, Point2D &right_head_point) const;
    const void Get4CornerBrushByMap(Point2D &left_head_point, Point2D &left_back_point,
                                       Point2D &right_back_point, Point2D &right_head_point) const;

    // 获取扫刷结构模型四个角点(含刷盘半径和刷毛长度)
    const void Get4CornerBrushFeatherByCar(Point2D &left_head_point, Point2D &left_back_point,
                                          Point2D &right_back_point, Point2D &right_head_point) const;
    const void Get4CornerBrushFeatherByMap(Point2D &left_head_point, Point2D &left_back_point,
                                          Point2D &right_back_point, Point2D &right_head_point) const;
  
    // 获取扫刷和车体组成的整个矩形模型的四个角点
    const void Get4CornerWholeByCar(Point2D &left_head_point, Point2D &left_back_point,
                                         Point2D &right_back_point, Point2D &right_head_point) const;
    const void Get4CornerWholeByMap(Point2D &left_head_point, Point2D &left_back_point,
                                         Point2D &right_back_point, Point2D &right_head_point) const;

private:
    const void GetBrushBodyParam(float &brush_body_length, float &brush_body_width) const;


private:
    float  brush_theta_{0.0};     // 扫刷展开角度默认0度
};

#endif // DYNAMIC_CAR_H

