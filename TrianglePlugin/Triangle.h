#pragma once
#include "Figure.h"
class __declspec(dllexport) Triangle :
    public Figure
{
private:
    double x, y, a, h, v_x, v_y;
    RECT bound_box;
public:
    Triangle();
    void Move(int mks);
    void Draw(HDC hdc);
    void SetBounds(RECT bbox);
};

