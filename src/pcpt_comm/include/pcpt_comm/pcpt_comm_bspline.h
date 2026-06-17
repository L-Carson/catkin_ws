#pragma once

struct BSPLINE_POS_STRU
{
    double  x;
    double  y;
    BSPLINE_POS_STRU(double _x, double _y) { x = _x; y = _y;}
    BSPLINE_POS_STRU() {};
    bool operator == (const BSPLINE_POS_STRU& pt) {return (x == pt.x && y == pt.y);}
};

class BSPLINE_C
{
public:
    BSPLINE_C(void){};
    ~BSPLINE_C(void){};

    void TwoOrderBSplineSmooth(BSPLINE_POS_STRU *pt, int Num);
    void TwoOrderBSplineInterpolatePt(BSPLINE_POS_STRU *&pt, int &Num, int *InsertNum);
    double F02(double t);
    double F12(double t);
    double F22(double t);

    void ThreeOrderBSplineSmooth(BSPLINE_POS_STRU *pt, int Num);
    void ThreeOrderBSplineInterpolatePt(BSPLINE_POS_STRU *&pt, int &Num, int *InsertNum);
    double F03(double t);
    double F13(double t);
    double F23(double t);
    double F33(double t);
};
