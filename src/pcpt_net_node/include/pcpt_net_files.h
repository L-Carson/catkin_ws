#ifndef __PCPT_FILES_H__
#define __PCPT_FILES_H__
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

using namespace std;

/**************************************************************************************
功能描述:
修改记录:
**************************************************************************************/
int npy2Arrary(float *&points_array, string file_name);
int bin2Arrary(float *&points_array, string file_name);
int shufferArrary(float *&points_array, int num_points);

#endif
