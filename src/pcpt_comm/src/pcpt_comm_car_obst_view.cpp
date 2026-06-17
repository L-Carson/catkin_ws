#include "pcpt_comm/pcpt_comm_car_obst_view.h"

static const std::string my_base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/**
 * base64加密
 * @param str 加密前数据
 * @param len 加密数据长度
 * @return 加密后数据
 */
static std::string Base64Encode(const unsigned char* bytes_to_encode, unsigned int in_len)
{
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len --)
    {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3)
        {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for(i = 0; (i < 4) ; i ++)
                ret += my_base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i)
    {
        for(j = i; j < 3; j ++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            ret += my_base64_chars[char_array_4[j]];

        while((i++ < 3))
            ret += '=';
    }

    return ret;
}

std::string Base64Encode1(const std::vector<unsigned char>& buf)
{
    std::ostringstream oss;
    typedef boost::archive::iterators::base64_from_binary<boost::archive::iterators::transform_width<decltype(buf.begin()), 6, 8>> base64_stream_iterator;
    std::copy(base64_stream_iterator(std::begin(buf)), base64_stream_iterator(std::end(buf)), boost::archive::iterators::ostream_iterator<char>(oss));
 
    return oss.str();
}

std::string CvMatToBase64(const cv::Mat &cvMat) 
{
    std::string strFormat = ".png";
    if (cvMat.empty()) {
        ST_LOG_ERR("cvMat Is Empty.");
        return "";
    }

    std::vector<uint8_t> buf;
    bool success = cv::imencode(strFormat, cvMat, buf);
    if (!success) {
        ST_LOG_ERR("Encode cvMat To %s Fail.", strFormat.c_str());
        return "";
    }

    //std::string strBase64 = base64_encode(buf);
    std::string strBase64 = Base64Encode(buf.data(), buf.size());
    
    return strBase64;
}


void PCPT_CAR_OBST_VIEW_C::AddGridMark(void)
{
    /* 在画布上画上刻度尺，以车体坐标原点为参考，间隔1米 */
    float spacingDist = 1.0;
#define IMAGE_TICK_MARK(scale, axis)                                                    \
    for (int i = 0; i <= int(fabsf(scale / spacingDist)); ++i) {                        \
        COORDINATE_2D_STRU stPoint1, stPoint2;                                          \
        if (axis == 'x') {                                                              \
            stPoint1.SetValue(spacingDist * i * SIGN(scale), stViewReg_.minY + 0.0001); \
            stPoint2.SetValue(spacingDist * i * SIGN(scale), stViewReg_.maxY - 0.0001); \
        } else if (axis == 'y') {                                                       \
            stPoint1.SetValue(stViewReg_.minX + 0.0001, spacingDist * i * SIGN(scale)); \
            stPoint2.SetValue(stViewReg_.maxX - 0.0001, spacingDist * i * SIGN(scale)); \
        }                                                                               \
        cv::Point point1, point2;                                                       \
        if (0 != Real2ImageCoord(stPoint1, point1)) continue;                           \
        if (0 != Real2ImageCoord(stPoint2, point2)) continue;                           \
        cv::line(cvBaseImg_, point1, point2, cv::Scalar(245, 245, 245), 1, 4);          \
    }                                                                                   \

    IMAGE_TICK_MARK(stViewReg_.minX, 'x');
    IMAGE_TICK_MARK(stViewReg_.maxX, 'x');
    IMAGE_TICK_MARK(stViewReg_.minY, 'y');
    IMAGE_TICK_MARK(stViewReg_.maxY, 'y');
}

void PCPT_CAR_OBST_VIEW_C::AddOriginMark(void)
{
    cv::Point stOriginImgCoord;
    if (0 != Real2ImageCoord(COORDINATE_2D_STRU(0.0, 0.0), stOriginImgCoord)) return;
    cv::circle(cvBaseImg_, stOriginImgCoord, 2, cv::Scalar(230, 230, 230), -1);    //-1标识实心
}

void PCPT_CAR_OBST_VIEW_C::AddCarShape(void)
{
    CFG_FILE_MNG_C objCfgFileMng;
    std::string strPcdFilePath = objCfgFileMng.GetCarBodyVoxelPcdFilePath();
    PointCloudXYZ::Ptr pCarBody(new PointCloudXYZ);
    if (-1 == pcl::io::loadPCDFile(strPcdFilePath, *pCarBody) ) {
        ST_LOG_WARN("车身模型: <%s>   失败", strPcdFilePath.c_str());     
        return;
    }

    ProjectionToImage(pCarBody, 100, cvBaseImg_);
}

void PCPT_CAR_OBST_VIEW_C::AddCarRecMark(const COORDINATE_2D_STRU &stLeftTop,
                                         const COORDINATE_2D_STRU &stRigthDown)
{
    cv::Point point1, point2;  
    if (0 != Real2ImageCoord(stLeftTop,   point1)) return;
    if (0 != Real2ImageCoord(stRigthDown, point2)) return;
    cv::rectangle(cvBaseImg_, point1, point2, cv::Scalar(230, 230, 230));
}
                                         
bool PCPT_CAR_OBST_VIEW_C::IsInImage(const int u, const int v)  
{
    if (u < 0 || u > cvBaseImg_.cols) return false;
    if (v < 0 || v > cvBaseImg_.rows) return false;
    return true;
}

void PCPT_CAR_OBST_TOP_2_DOWN_VIEW_C::ProjectionToImage(const PointCloudXYZ::Ptr &pObstPoints, const int pixelValue, cv::Mat &cvImage)
{
    cvImage = cvBaseImg_.clone();
    for (size_t i = 0; i < pObstPoints->size(); i++) {
        cv::Point p;
        PointXYZ &point = pObstPoints->points[i];
        if (0 != Real2ImageCoord(COORDINATE_2D_STRU(point.x, point.y), p)) continue;
        int u = (int)p.x, v = (int)p.y;
        cvImage.at<uint8_t>(v, u) = pixelValue;
    }
}

int  PCPT_CAR_OBST_TOP_2_DOWN_VIEW_C::Real2ImageCoord(const COORDINATE_2D_STRU& stRealCoord, cv::Point &stImgCoord)
{
    int gridX, gridY;
    if (0 != objGrid_.GetGridXYIndex(stRealCoord.x, stRealCoord.y, gridX, gridY)) return -1;
    int u = cvBaseImg_.cols - gridY - 1;
    int v = cvBaseImg_.rows - gridX - 1;
    if (!IsInImage(u, v)) return -1;

    stImgCoord = cv::Point(u, v);

    return 0;
}

void PCPT_CAR_OBST_BACK_2_FRONT_VIEW_C::ProjectionToImage(const PointCloudXYZ::Ptr &pObstPoints, const int pixelValue, cv::Mat &cvImage)
{
    cvImage = cvBaseImg_.clone();
    for (size_t i = 0; i < pObstPoints->size(); i++) {
        cv::Point p;
        PointXYZ &point = pObstPoints->points[i];
        if (0 != Real2ImageCoord(COORDINATE_2D_STRU(point.z, point.y), p)) continue;
        int u = (int)p.x, v = (int)p.y;
        cvImage.at<uint8_t>(v, u) = pixelValue;
    }   
}

int  PCPT_CAR_OBST_BACK_2_FRONT_VIEW_C::Real2ImageCoord(const COORDINATE_2D_STRU& stRealCoord, cv::Point &stImgCoord)
{
    int gridX, gridY;
    if (0 != objGrid_.GetGridXYIndex(stRealCoord.x, stRealCoord.y, gridX, gridY)) return -1;
    int u = cvBaseImg_.cols - gridY - 1;
    int v = cvBaseImg_.rows - gridX - 1;
    if (!IsInImage(u, v)) return -1;
    stImgCoord = cv::Point(u, v);
    return 0;   
}

void PCPT_CAR_OBST_LEFT_2_RIGHT_VIEW_C::ProjectionToImage(const PointCloudXYZ::Ptr &pObstPoints, const int pixelValue, cv::Mat &cvImage)
{
    cvImage = cvBaseImg_.clone();
    for (size_t i = 0; i < pObstPoints->size(); i++) {
        cv::Point p;
        PointXYZ &point = pObstPoints->points[i];
        if (0 != Real2ImageCoord(COORDINATE_2D_STRU(point.x, point.z), p)) continue;
        int u = (int)p.x, v = (int)p.y;
        cvImage.at<uint8_t>(v, u) = pixelValue;
    }
}

int  PCPT_CAR_OBST_LEFT_2_RIGHT_VIEW_C::Real2ImageCoord(const COORDINATE_2D_STRU& stRealCoord, cv::Point &stImgCoord)
{
    int gridX, gridY;
    if (0 != objGrid_.GetGridXYIndex(stRealCoord.x, stRealCoord.y, gridX, gridY)) return -1;
    int u = cvBaseImg_.cols - gridX - 1;
    int v = cvBaseImg_.rows - gridY - 1;
    if (!IsInImage(u, v)) return -1;
    stImgCoord = cv::Point(u, v);
    return 0;      
}


