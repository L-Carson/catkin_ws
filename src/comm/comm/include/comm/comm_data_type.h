#ifndef __COMM_DATA_TYPE_H__
#define __COMM_DATA_TYPE_H__

#include <string>
#include <sstream>      /* For std::ostringstream */
#include <iomanip>      /* For setw setfill等设置字符串输出格式的函数 */
#include <algorithm>    // 包含 std::transform
#include <cctype>       // 包含 std::tolower


typedef unsigned char      UINT08;
typedef unsigned short     UINT16;
typedef unsigned int       UINT32;
typedef signed long long   INT64;
typedef unsigned long long UINT64;
typedef char               INT08;
typedef short              INT16;
typedef int                INT32;
typedef bool               BOOL;
typedef char               CHAR;

#define UINT08_NULL     0xFF
#define UINT32_NULL     0xFFFFFFFF

#ifndef INT32_MAX
#define INT32_MAX       0x7FFFFFFF
#endif


/* 计算数组大小 */
#define ARRAY_SIZE(arrayNuam_)                  (sizeof(arrayNuam_) / sizeof(arrayNuam_[0]))

/* 变量交换 */
#define SWAP_DATA(a_, b_)                       {auto tmp_ = a_; a_ = b_; b_ = tmp_;}

/* 字节处理 */
#define COMB_WORD(byteH_, byteL_)               ((((unsigned short)(byteH_)) << 8) + ((unsigned short)(byteL_)))
#define GET_BYTE(val_, byteIdx_)                 ((UINT32(val_) >> ((byteIdx_) * 8)) & 0x000000FF)

/* 位处理 */
#define BIT_MASK(bitNo_)                        (0x01 << (bitNo_))
#define GET_BIT(byte_, bitNo_)                  (((byte_) & BIT_MASK(bitNo_)) != 0)
#define SET_BIT(byte_, bitNo_)                  (byte_) = (byte_) | BIT_MASK(bitNo_)
#define CLR_BIT(byte_, bitNo_)                  (byte_) = (byte_) & (~ BIT_MASK(bitNo_))
#define SET_BIT_V(byte_, bitNo_, v_)            if (v_) { SET_BIT(byte_, bitNo_); } else { CLR_BIT(byte_, bitNo_); }
#define CLR_HIGH_BITS(data_, beginBit_)         ((data_) & (~ (0xFFFFFFFF << (beginBit_))))

#define ABS(x_)                                 (((x_) < 0) ? ((x_) * (-1)) : (x_))
#define SIGN(x_)                                (((x_) >= 0) ? 1 : -1)
#define LIMITO                                  1E-8
#define SIG(x_)                                 (INT32(x_ > LIMITO) - INT32(x_ < -LIMITO))          //返回-1, 0, 1

#ifndef MIN
#define MIN(x_, y_)                             (((x_) < (y_)) ? (x_) : (y_))
#endif

#ifndef MAX
#define MAX(x_, y_)                             (((x_) < (y_)) ? (y_) : (x_))
#endif

#define LIMIT(v_, min_, max_)                   (MAX(min_, MIN(max_, v_)))
#define DIFF_ABS(v1_, v2_)                      (((v1_) > (v2_)) ? ((v1_) - (v2_)) : ((v2_) - (v1_)))
#define GET_NEAR_VAL(srcVal_, val1_, val2_)     ((DIFF_ABS(srcVal_, val1_) < DIFF_ABS(srcVal_, val2_)) ? (val1_) : (val2_))
#define GET_CLOSEST_MULTIPLE(v1_, v2_)          INT32((float(v1_) + 0.5 * float(v2_)) / float(v2_))

/* 判定是否制定数字是否在指定范围(开区间，闭区间，开闭区间，闭开区间等) */
#define IS_IN_O_O_RANGE(val_, min_, max_)       (((val_) >  (min_)) && ((val_) <  (max_)))
#define IS_IN_C_C_RANGE(val_, min_, max_)       (((val_) >= (min_)) && ((val_) <= (max_)))
#define IS_IN_C_O_RANGE(val_, min_, max_)       (((val_) >= (min_)) && ((val_) <  (max_)))
#define IS_IN_O_C_RANGE(val_, min_, max_)       (((val_) >  (min_)) && ((val_) <= (max_)))

/* 判断浮现点数(几乎)相等 */
#define IS_FLOAT_ALMOST_EQUAL_ZERO(v_)          (ABS(v_) < 1.0e-10)
#define IS_FLOAT_ALMOST_EQUAL(v1_, v2_)         (IS_FLOAT_ALMOST_EQUAL_ZERO((v1_) - (v2_)))

/* 小数四舍五入 */
#define ROUND_UP_DOWN(x_)                       (((x_) >= 0.5) ? 1 : 0)

/* 4舍5入，保留n位小数 */
#define DBL_DECIMAL(x_, n)                      int(x_ * pow(10, n) + 0.5) / pow(10, n)

/* 向上取整 */
#define CEIL(x_, zero_)                         (ABS(x_ - round(x_)) < zero_ ? (x_ - round(x_) >= 0 ? floor(x_) : ceil(x_)) : (ceil(x_)))

/* 向下取整 */
#define FLOOR(x_, zero_)                        (ABS(x_ - round(x_)) < zero_ ? (x_ - round(x_) >= 0 ? floor(x_) : ceil(x_)) : (floor(x_)))

/* 计算平方 */
#define SQUARE(x_)                              ((x_) * (x_))

/* 浮点转字符串 */
inline std::string DoubleToString(double value, UINT32 precision)
{
    std::ostringstream os;
    os << std::setiosflags(std::ios::fixed) << std::setprecision(precision) << value;

    return os.str();
}

inline std::string DoubleToString(double value, UINT32 precision, UINT32 width, CHAR fill)
{
    std::ostringstream os;
    os << std::setiosflags(std::ios::fixed) << std::setprecision(precision) << std::setw(width) << std::setfill(fill) << value;

    return os.str();
}

/* 整形转字符串 */
inline std::string ToString(UINT32 value)
{
    std::ostringstream os;
    os << value;

    return os.str();
}

inline std::string ToString(UINT32 value, UINT32 width, CHAR fill)
{
    std::ostringstream os;
    os << std::setw(width) << std::setfill(fill) << value;

    return os.str();
}

inline std::string ToLower(const std::string &strSrc)
{
    std::string strTgt(strSrc);

    std::transform(std::begin(strTgt),
                   std::end(strTgt),
                   std::begin(strTgt),
                   [](unsigned char c)
                   {
                       return std::tolower(c);
                   });

    return strTgt;
}

#endif

