#ifndef __PCPT_HUNGARIAN_H__
#define __PCPT_HUNGARIAN_H__

#include <iostream>
#include <vector>

using namespace std;

/**************************************************************************************
功能描述: 匈牙利匹配算法
修改记录:
**************************************************************************************/
class HUNGARIAN_ALGORITHM_C
{
public:
    HUNGARIAN_ALGORITHM_C(){};
    ~HUNGARIAN_ALGORITHM_C(){};

    double Munkres(vector<vector<double>>& vvDistMatrix, vector<int>& vAssignment);

private:
    void Assignmentoptimal(int *pAssignment, double *pCost, double *pDistMatrix, int nOfRows, int nOfColumns);
    void BuildAssignmentVector(int *pAssignment, bool *pStarMatrix, int nOfRows, int nOfColumns);
    void ComputeAssignmentCost(int *pAssignment, double *pCost, double *pDistMatrix, int nOfRows);
    void Step2a(int *pAssignment, double *pDistMatrix, bool *pStarMatrix, bool *pNewStarMatrix, bool *pPrimeMatrix, bool *pCoveredColumns, bool *pCoveredRows, int nOfRows, int nOfColumns, int minDim);
    void Step2b(int *pAssignment, double *pDistMatrix, bool *pStarMatrix, bool *pNewStarMatrix, bool *pPrimeMatrix, bool *pCoveredColumns, bool *pCoveredRows, int nOfRows, int nOfColumns, int minDim);
    void Step3(int *pAssignment, double *pDistMatrix, bool *pStarMatrix, bool *pNewStarMatrix, bool *pPrimeMatrix, bool *pCoveredColumns, bool *pCoveredRows, int nOfRows, int nOfColumns, int minDim);
    void Step4(int *pAssignment, double *pDistMatrix, bool *pStarMatrix, bool *pNewStarMatrix, bool *pPrimeMatrix, bool *pCoveredColumns, bool *pCoveredRows, int nOfRows, int nOfColumns, int minDim, int row, int col);
    void Step5(int *pAssignment, double *pDistMatrix, bool *pStarMatrix, bool *pNewStarMatrix, bool *pPrimeMatrix, bool *pCoveredColumns, bool *pCoveredRows, int nOfRows, int nOfColumns, int minDim);

    void Test();
};


#endif
