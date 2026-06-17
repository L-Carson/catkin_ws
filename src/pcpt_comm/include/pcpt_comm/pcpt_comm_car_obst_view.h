#pragma once

#include <memory>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/ostream_iterator.hpp>
#include <iostream>
#include <sstream>
#include <boost/algorithm/string.hpp>

#include "comm/comm_msg_nav_state.h"
#include "pcpt_comm/pcpt_comm_base.h"
#include "pcpt_comm/pcpt_comm_ros.h"
#include "pcpt_comm/pcpt_comm_car.h"
#include "pcpt_comm/pcpt_comm_points.h"
#include "pcpt_comm/pcpt_comm_singleton.h"
#include <opencv2/opencv.hpp>

namespace bai = boost::archive::iterators;

std::string CvMatToBase64(const cv::Mat &cvMat);

class PCPT_CAR_OBST_VIEW_C
{
public:
    typedef std::shared_ptr<PCPT_CAR_OBST_VIEW_C> Ptr;
    
public:
    PCPT_CAR_OBST_VIEW_C(const RECTANGLE_STRU &stViewReg, const float pixelSize = 0.05) 
        : stViewReg_(stViewReg), pixelSize_(pixelSize)
    {
        
    }
  
    virtual void ProjectionToImage(const PointCloudXYZ::Ptr &pObstPoints, const int pixelValue, cv::Mat &cvImage) = 0;

protected:
    virtual int Real2ImageCoord(const COORDINATE_2D_STRU& stRealCoord, cv::Point &stImgCoord) = 0;
    void AddGridMark(void);
    void AddOriginMark(void);
    void AddCarShape(void);
    void AddCarRecMark(const COORDINATE_2D_STRU &stLeftTop,
                       const COORDINATE_2D_STRU &stRigthDown);
    bool IsInImage(const int u, const int v);
    
protected:
    cv::Mat         cvBaseImg_;
    PCPT_GRID_C     objGrid_; 
    RECTANGLE_STRU  stViewReg_;
    float           pixelSize_;     
};

class PCPT_CAR_OBST_TOP_2_DOWN_VIEW_C : public PCPT_CAR_OBST_VIEW_C
{
public:
    typedef std::shared_ptr<PCPT_CAR_OBST_TOP_2_DOWN_VIEW_C> Ptr;

public:
    PCPT_CAR_OBST_TOP_2_DOWN_VIEW_C(const COORDINATE_2D_STRU &stLeftTop,
                                    const COORDINATE_2D_STRU &stRigthDown,
                                    const COORDINATE_2D_STRU &stExtLeftTop,
                                    const COORDINATE_2D_STRU &stExtRigthDown,
                                    const RECTANGLE_STRU &stViewReg, const float pixelSize = 0.05)
        : PCPT_CAR_OBST_VIEW_C(stViewReg, pixelSize)
    {
        /* 初始化画布 */
        int row = ceil((stViewReg_.maxX - stViewReg_.minX) / pixelSize_);
        int col = ceil((stViewReg_.maxY - stViewReg_.minY) / pixelSize_);
        cvBaseImg_ = cv::Mat(row, col, CV_8UC1, cv::Scalar(255, 255, 255));
        objGrid_.Reset(stViewReg_, pixelSize_);

        AddGridMark();
        AddOriginMark();
        //AddCarShape();
        AddCarRecMark(stLeftTop, stRigthDown);
        AddCarRecMark(stExtLeftTop, stExtRigthDown);
    }
    void ProjectionToImage(const PointCloudXYZ::Ptr &pObstPoints, const int pixelValue, cv::Mat &cvImage) override;
        
private:
    int Real2ImageCoord(const COORDINATE_2D_STRU& stRealCoord, cv::Point &stImgCoord) override;         
};

class PCPT_CAR_OBST_BACK_2_FRONT_VIEW_C : public PCPT_CAR_OBST_VIEW_C
{
public:
    typedef std::shared_ptr<PCPT_CAR_OBST_BACK_2_FRONT_VIEW_C> Ptr;

public:
    PCPT_CAR_OBST_BACK_2_FRONT_VIEW_C(const COORDINATE_2D_STRU &stLeftTop,
                                      const COORDINATE_2D_STRU &stRigthDown,
                                      const COORDINATE_2D_STRU &stExtLeftTop,
                                      const COORDINATE_2D_STRU &stExtRigthDown,
                                      const RECTANGLE_STRU &stViewReg, const float pixelSize = 0.05)
        : PCPT_CAR_OBST_VIEW_C(stViewReg, pixelSize)
    {
        /* 初始化画布 */
        int row = ceil((stViewReg_.maxX - stViewReg_.minX) / pixelSize_);
        int col = ceil((stViewReg_.maxY - stViewReg_.minY) / pixelSize_);
        cvBaseImg_ = cv::Mat(row, col, CV_8UC1, cv::Scalar(255, 255, 255));
        objGrid_.Reset(stViewReg_, pixelSize_);

        AddGridMark();
        AddOriginMark();
        AddCarShape();
        AddCarRecMark(stLeftTop, stRigthDown);
        AddCarRecMark(stExtLeftTop, stExtRigthDown);
    }
    void ProjectionToImage(const PointCloudXYZ::Ptr &pObstPoints, const int pixelValue, cv::Mat &cvImage) override;
        
private:
    int Real2ImageCoord(const COORDINATE_2D_STRU& stRealCoord, cv::Point &stImgCoord) override;         
};

class PCPT_CAR_OBST_LEFT_2_RIGHT_VIEW_C : public PCPT_CAR_OBST_VIEW_C
{
public:
    typedef std::shared_ptr<PCPT_CAR_OBST_LEFT_2_RIGHT_VIEW_C> Ptr;

public:
    PCPT_CAR_OBST_LEFT_2_RIGHT_VIEW_C(const COORDINATE_2D_STRU &stLeftTop,
                                      const COORDINATE_2D_STRU &stRigthDown,
                                      const RECTANGLE_STRU &stViewReg, const float pixelSize = 0.05)
        : PCPT_CAR_OBST_VIEW_C(stViewReg, pixelSize)
    {
        /* 初始化画布 */
        int row = ceil((stViewReg_.maxY - stViewReg_.minY) / pixelSize_);
        int col = ceil((stViewReg_.maxX - stViewReg_.minX) / pixelSize_);
        cvBaseImg_ = cv::Mat(row, col, CV_8UC1, cv::Scalar(255, 255, 255));
        objGrid_.Reset(stViewReg_, pixelSize_);

        AddGridMark();
        AddOriginMark();
        AddCarShape();
        AddCarRecMark(stLeftTop, stRigthDown);
    }
    void ProjectionToImage(const PointCloudXYZ::Ptr &pObstPoints, const int pixelValue, cv::Mat &cvImage) override;
        
private:
    int Real2ImageCoord(const COORDINATE_2D_STRU& stRealCoord, cv::Point &stImgCoord) override;         
};

