#include <stdlib.h>
#include <cfloat> // for DBL_MAX
#include <cmath>  // for fabs()
#include "pcpt_comm/pcpt_comm_hungarian.h"

/**************************************************************************************
功能描述: A single function wrapper for solving assignment problem.
修改记录:
**************************************************************************************/
double HUNGARIAN_ALGORITHM_C::Munkres(vector<vector<double>>& vvDistMatrix, vector<int>& vAssignment)
{
    unsigned int nRows = vvDistMatrix.size();
    unsigned int nCols = vvDistMatrix[0].size();

    double *pDistMatrixIn = new double[nRows * nCols];
    int *pAssignment = new int[nRows];
    double cost = 0.0;

    // Fill in the vvDistMatrixIn. Mind the index is "i + nRows * j".
    // Here the cost matrix of size MxN is defined as a double precision array of N*M elements.
    // In the solving functions matrices are seen to be saved in row-order.
    // (i.e. the matrix [1 2; 3 4] will be stored as a vector [1 3 2 4], NOT [1 2 3 4]).
    for (unsigned int i = 0; i < nRows; i++)
        for (unsigned int j = 0; j < nCols; j++)
            pDistMatrixIn[i + nRows * j] = vvDistMatrix[i][j];

    // call solving function
    Assignmentoptimal(pAssignment, &cost, pDistMatrixIn, nRows, nCols);

    vAssignment.clear();
    for (unsigned int r = 0; r < nRows; r++)
        vAssignment.push_back(pAssignment[r]);

    delete[] pDistMatrixIn;
    delete[] pAssignment;
    return cost;
}

/**************************************************************************************
功能描述: Solve optimal solution for assignment problem using Munkres algorithm, also known as Hungarian Algorithm.
修改记录:
**************************************************************************************/
void HUNGARIAN_ALGORITHM_C::Assignmentoptimal(int *pAssignment, double *pCost, double *pDistMatrixIn, int nOfRows, int nOfColumns)
{
    double *distMatrix, *distMatrixTemp, *distMatrixEnd, *columnEnd, value, minValue;
    bool *coveredColumns, *coveredRows, *starMatrix, *newStarMatrix, *primeMatrix;
    int nOfElements, minDim, row, col;

    /* initialization */
    *pCost = 0;
    for (row = 0; row < nOfRows; row++)
        pAssignment[row] = -1;

    /* generate working copy of distance Matrix */
    /* check if all matrix elements are positive */
    nOfElements = nOfRows * nOfColumns;
    distMatrix = (double *)malloc(nOfElements * sizeof(double));
    distMatrixEnd = distMatrix + nOfElements;

    for (row = 0; row < nOfElements; row++) {
        value = pDistMatrixIn[row];
        if (value < 0)
            cerr << "All matrix elements have to be non-negative." << endl;
        distMatrix[row] = value;
    }

    /* memory allocation */
    coveredColumns = (bool *)calloc(nOfColumns,  sizeof(bool));
    coveredRows    = (bool *)calloc(nOfRows,     sizeof(bool));
    starMatrix     = (bool *)calloc(nOfElements, sizeof(bool));
    primeMatrix    = (bool *)calloc(nOfElements, sizeof(bool));
    newStarMatrix  = (bool *)calloc(nOfElements, sizeof(bool)); /* used in step4 */

    /* preliminary steps */
    if (nOfRows <= nOfColumns) {
        minDim = nOfRows;

        for (row = 0; row < nOfRows; row++) {
            /* find the smallest element in the row */
            distMatrixTemp = distMatrix + row;
            minValue = *distMatrixTemp;
            distMatrixTemp += nOfRows;
            while (distMatrixTemp < distMatrixEnd) {
                value = *distMatrixTemp;
                if (value < minValue)
                    minValue = value;
                distMatrixTemp += nOfRows;
            }

            /* subtract the smallest element from each element of the row */
            distMatrixTemp = distMatrix + row;
            while (distMatrixTemp < distMatrixEnd) {
                *distMatrixTemp -= minValue;
                distMatrixTemp += nOfRows;
            }
        }

        /* Steps 1 and 2a */
        for (row = 0; row < nOfRows; row++)
            for (col = 0; col<nOfColumns; col++)
                if (fabs(distMatrix[row + nOfRows*col]) < DBL_EPSILON)
                    if (!coveredColumns[col]) {
                        starMatrix[row + nOfRows*col] = true;
                        coveredColumns[col] = true;
                        break;
                    }
    } else {
        minDim = nOfColumns;

        for (col = 0; col<nOfColumns; col++) {
            /* find the smallest element in the column */
            distMatrixTemp = distMatrix + nOfRows*col;
            columnEnd = distMatrixTemp + nOfRows;

            minValue = *distMatrixTemp++;
            while (distMatrixTemp < columnEnd) {
                value = *distMatrixTemp++;
                if (value < minValue)
                    minValue = value;
            }

            /* subtract the smallest element from each element of the column */
            distMatrixTemp = distMatrix + nOfRows*col;
            while (distMatrixTemp < columnEnd)
                *distMatrixTemp++ -= minValue;
        }

        /* Steps 1 and 2a */
        for (col = 0; col < nOfColumns; col++)
            for (row = 0; row < nOfRows; row++)
                if (fabs(distMatrix[row + nOfRows*col]) < DBL_EPSILON)
                    if (!coveredRows[row]) {
                        starMatrix[row + nOfRows*col] = true;
                        coveredColumns[col] = true;
                        coveredRows[row] = true;
                        break;
                    }
        for (row = 0; row<nOfRows; row++)
            coveredRows[row] = false;

    }

    /* move to step 2b */
    Step2b(pAssignment, distMatrix, starMatrix, newStarMatrix, primeMatrix, coveredColumns, coveredRows, nOfRows, nOfColumns, minDim);

    /* compute cost and remove invalid assignments */
    ComputeAssignmentCost(pAssignment, pCost, pDistMatrixIn, nOfRows);

    /* free allocated memory */
    free(distMatrix);
    free(coveredColumns);
    free(coveredRows);
    free(starMatrix);
    free(primeMatrix);
    free(newStarMatrix);

    return;
}

/**************************************************************************************
功能描述:
修改记录:
**************************************************************************************/
void HUNGARIAN_ALGORITHM_C::BuildAssignmentVector(int *pAssignment, bool *pStarMatrix, int nOfRows, int nOfColumns)
{
    int row, col;
    for (row = 0; row < nOfRows; row++) {
        for (col = 0; col < nOfColumns; col++) {
            if (pStarMatrix[row + nOfRows*col]) {
                pAssignment[row] = col;
                break;
            }
        }
    }
}

/**************************************************************************************
功能描述:
修改记录:
**************************************************************************************/
void HUNGARIAN_ALGORITHM_C::ComputeAssignmentCost(int *pAssignment, double *pCost, double *pDistMatrix, int nOfRows)
{
    int row, col;
    for (row = 0; row < nOfRows; row++) {
        col = pAssignment[row];
        if (col >= 0)
            *pCost += pDistMatrix[row + nOfRows * col];
    }
}

/**************************************************************************************
功能描述:
修改记录:
**************************************************************************************/
void HUNGARIAN_ALGORITHM_C::Step2a(int *pAssignment, double *pDistMatrix, bool *pStarMatrix, bool *pNewStarMatrix, bool *pPrimeMatrix, bool *pCoveredColumns, bool *pCoveredRows, int nOfRows, int nOfColumns, int minDim)
{
    bool *starMatrixTemp, *columnEnd;
    int col;

    /* cover every column containing a starred zero */
    for (col = 0; col < nOfColumns; col++) {
        starMatrixTemp = pStarMatrix + nOfRows*col;
        columnEnd = starMatrixTemp + nOfRows;
        while (starMatrixTemp < columnEnd) {
            if (*starMatrixTemp++) {
                pCoveredColumns[col] = true;
                break;
            }
        }
    }

    /* move to step 3 */
    Step2b(pAssignment, pDistMatrix, pStarMatrix, pNewStarMatrix, pPrimeMatrix, pCoveredColumns, pCoveredRows, nOfRows, nOfColumns, minDim);
}

/**************************************************************************************
功能描述:
修改记录:
**************************************************************************************/
void HUNGARIAN_ALGORITHM_C::Step2b(int *pAssignment, double *pDistMatrix, bool *pStarMatrix, bool *pNewStarMatrix, bool *pPrimeMatrix, bool *pCoveredColumns, bool *pCoveredRows, int nOfRows, int nOfColumns, int minDim)
{
    int col, nOfCoveredColumns;

    /* count covered columns */
    nOfCoveredColumns = 0;
    for (col = 0; col < nOfColumns; col++)
        if (pCoveredColumns[col])
            nOfCoveredColumns++;

    if (nOfCoveredColumns == minDim) {
        /* algorithm finished */
        BuildAssignmentVector(pAssignment, pStarMatrix, nOfRows, nOfColumns);
    } else {
        /* move to step 3 */
        Step3(pAssignment, pDistMatrix, pStarMatrix, pNewStarMatrix, pPrimeMatrix, pCoveredColumns, pCoveredRows, nOfRows, nOfColumns, minDim);
    }
}

/**************************************************************************************
功能描述:
修改记录:
**************************************************************************************/
void HUNGARIAN_ALGORITHM_C::Step3(int *pAssignment, double *pDistMatrix, bool *pStarMatrix, bool *pNewStarMatrix, bool *pPrimeMatrix, bool *pCoveredColumns, bool *pCoveredRows, int nOfRows, int nOfColumns, int minDim)
{
    bool zerosFound;
    int row, col, starCol;

    zerosFound = true;
    while (zerosFound) {
        zerosFound = false;
        for (col = 0; col < nOfColumns; col++)
            if (!pCoveredColumns[col])
                for (row = 0; row < nOfRows; row++)
                    if ((!pCoveredRows[row]) && (fabs(pDistMatrix[row + nOfRows*col]) < DBL_EPSILON)) {
                        /* prime zero */
                        pPrimeMatrix[row + nOfRows * col] = true;

                        /* find starred zero in current row */
                        for (starCol = 0; starCol < nOfColumns; starCol++)
                            if (pStarMatrix[row + nOfRows*starCol])
                                break;

                        if (starCol == nOfColumns) { /* no starred zero found */
                            /* move to step 4 */
                            Step4(pAssignment, pDistMatrix, pStarMatrix, pNewStarMatrix, pPrimeMatrix, pCoveredColumns, pCoveredRows, nOfRows, nOfColumns, minDim, row, col);
                            return;
                        } else {
                            pCoveredRows[row] = true;
                            pCoveredColumns[starCol] = false;
                            zerosFound = true;
                            break;
                        }
                    }
    }

    /* move to step 5 */
    Step5(pAssignment, pDistMatrix, pStarMatrix, pNewStarMatrix, pPrimeMatrix, pCoveredColumns, pCoveredRows, nOfRows, nOfColumns, minDim);
}

/**************************************************************************************
功能描述:
修改记录:
**************************************************************************************/
void HUNGARIAN_ALGORITHM_C::Step4(int *pAssignment, double *pDistMatrix, bool *pStarMatrix, bool *pNewStarMatrix, bool *pPrimeMatrix, bool *pCoveredColumns, bool *pCoveredRows, int nOfRows, int nOfColumns, int minDim, int row, int col)
{
    int n, starRow, starCol, primeRow, primeCol;
    int nOfElements = nOfRows*nOfColumns;

    /* generate temporary copy of starMatrix */
    for (n = 0; n < nOfElements; n++)
        pNewStarMatrix[n] = pStarMatrix[n];

    /* star current zero */
    pNewStarMatrix[row + nOfRows*col] = true;

    /* find starred zero in current column */
    starCol = col;
    for (starRow = 0; starRow<nOfRows; starRow++)
        if (pStarMatrix[starRow + nOfRows*starCol])
            break;

    while (starRow < nOfRows) {
        /* unstar the starred zero */
        pNewStarMatrix[starRow + nOfRows*starCol] = false;

        /* find primed zero in current row */
        primeRow = starRow;
        for (primeCol = 0; primeCol < nOfColumns; primeCol++)
            if (pPrimeMatrix[primeRow + nOfRows * primeCol])
                break;

        /* star the primed zero */
        pNewStarMatrix[primeRow + nOfRows*primeCol] = true;

        /* find starred zero in current column */
        starCol = primeCol;
        for (starRow = 0; starRow < nOfRows; starRow++)
            if (pStarMatrix[starRow + nOfRows * starCol])
                break;
    }

    /* use temporary copy as new starMatrix */
    /* delete all primes, uncover all rows */
    for (n = 0; n < nOfElements; n++) {
        pPrimeMatrix[n] = false;
        pStarMatrix[n] = pNewStarMatrix[n];
    }
    for (n = 0; n < nOfRows; n++)
        pCoveredRows[n] = false;

    /* move to step 2a */
    Step2a(pAssignment, pDistMatrix, pStarMatrix, pNewStarMatrix, pPrimeMatrix, pCoveredColumns, pCoveredRows, nOfRows, nOfColumns, minDim);
}

/**************************************************************************************
功能描述:
修改记录:
**************************************************************************************/
void HUNGARIAN_ALGORITHM_C::Step5(int *pAssignment, double *pDistMatrix, bool *pStarMatrix, bool *pNewStarMatrix, bool *pPrimeMatrix, bool *pCoveredColumns, bool *pCoveredRows, int nOfRows, int nOfColumns, int minDim)
{
    double h, value;
    int row, col;

    /* find smallest uncovered element h */
    h = DBL_MAX;
    for (row = 0; row < nOfRows; row++)
        if (!pCoveredRows[row])
            for (col = 0; col < nOfColumns; col++)
                if (!pCoveredColumns[col]) {
                    value = pDistMatrix[row + nOfRows*col];
                    if (value < h)
                        h = value;
                }

    /* add h to each covered row */
    for (row = 0; row < nOfRows; row++)
        if (pCoveredRows[row])
            for (col = 0; col < nOfColumns; col++)
                pDistMatrix[row + nOfRows*col] += h;

    /* subtract h from each uncovered column */
    for (col = 0; col < nOfColumns; col++)
        if (!pCoveredColumns[col])
            for (row = 0; row < nOfRows; row++)
                pDistMatrix[row + nOfRows*col] -= h;

    /* move to step 3 */
    Step3(pAssignment, pDistMatrix, pStarMatrix, pNewStarMatrix, pPrimeMatrix, pCoveredColumns, pCoveredRows, nOfRows, nOfColumns, minDim);
}

void HUNGARIAN_ALGORITHM_C::Test()
{
    //please use "-std=c++11" for this initialization of vector.
//  vector<vector<double>> costMatrix = { { 10, 19, 8, 15, 0 },
//                                        { 10, 18, 7, 17, 0 },
//                                        { 13, 16, 9, 14, 0 },
//                                        { 12, 19, 8, 18, 0 } };

    vector<vector<double>> costMatrix = { { 10, 10, 13, 12},
                                          { 19, 18, 16, 19},
                                          {  8,  7,  9,  8},
                                          { 15, 17, 14, 18},
                                          {  0,  0,  0,  0} };

    vector<int> assignment;

    double cost = Munkres(costMatrix, assignment);

    for (unsigned int x = 0; x < costMatrix.size(); x++)
        std::cout << x << "," << assignment[x] << "\t";

    std::cout << "\ncost: " << cost << std::endl;
}
