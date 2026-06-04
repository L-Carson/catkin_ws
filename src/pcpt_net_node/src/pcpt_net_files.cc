#include "pcpt_net_files.h"

/**************************************************************************************
功能描述: 读取bin格式文件
修改记录:
**************************************************************************************/
int npy2Arrary(float *&points_array, string file_name)
{
    fstream input(file_name.c_str(), ios::in | ios::binary);
    if (!input.good()) {
        cerr << "Could not read file: " << file_name << endl;
        return -1;
    }

    input.seekg(0, ios::beg);
    int i;
    float x_temp;
    vector<float> temp_points;

    for (i=0; input.good() && !input.eof(); i++) {
        input.read((char *) &x_temp, sizeof(float));
        temp_points.push_back(x_temp);
    }
    input.close();

    points_array = new float[temp_points.size()];
    for (int i = 0 ; i < (int)temp_points.size() ; ++i) {
        points_array[i] = temp_points[i];
    }
    return temp_points.size();
}

/**************************************************************************************
功能描述: 读取bin格式点云文件
修改记录:
**************************************************************************************/
int bin2Arrary(float *&points_array, string file_name)
{
    fstream input(file_name.c_str(), ios::in | ios::binary);
    if(!input.good()) {
        cerr << "Could not read file: " << file_name << endl;
        return -1;
    }
    input.seekg(0, ios::beg);

    int i;
    float x_temp;
    float y_temp;
    float z_temp;
    float i_temp;
    float c_temp;

    vector<float> temp_points;
    for (i=0; input.good() && !input.eof(); i++) {
        input.read((char *) &x_temp, sizeof(float));
        input.read((char *) &y_temp, sizeof(float));
        input.read((char *) &z_temp, sizeof(float));
        input.read((char *) &i_temp, sizeof(float));
        input.read((char *) &c_temp, sizeof(float));
        temp_points.push_back(x_temp);
        temp_points.push_back(y_temp);
        // temp_points.push_back(z_temp -1.8);
        temp_points.push_back(z_temp);
        temp_points.push_back(i_temp);
        temp_points.push_back(0);
    }
    input.close();

    points_array = new float[temp_points.size()];
    for (int i = 0 ; i < (int)temp_points.size() ; ++i) {
        points_array[i] = temp_points[i];
    }
    return temp_points.size() / 5;
}

/**************************************************************************************
功能描述: 数据清洗
修改记录:
**************************************************************************************/
int shufferArrary(float *&points_array, int num_points)
{
    srand(time(NULL));
    int rd  ;
    for(int i = num_points-1; i >= 0; i--)
    {
        rd = rand()%(i+1);
        float x_temp = points_array[rd*5 ];
        float y_temp = points_array[rd*5 +1 ];
        float z_temp = points_array[rd*5  +2];
        float i_temp = points_array[rd*5  +3];
        points_array[rd*5]        = points_array[i*5];
        points_array[rd*5 + 1]    = points_array[i*5 + 1];
        points_array[rd*5 + 2]    = points_array[i*5 + 2];
        points_array[rd*5 + 3]    = points_array[i*5 + 3];
        points_array[i*5 ]        = x_temp;
        points_array[i*5 + 1]     = y_temp;
        points_array[i*5 + 2]     = z_temp;
        points_array[i*5 + 3]     = i_temp;
    }
    return 1;
}
