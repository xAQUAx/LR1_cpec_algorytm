#pragma once
#include "Figure.h"
class __declspec(dllexport) Coub :
    public Figure
{
private:
    double x, y, a, v_x, v_y;
    RECT bound_box;
public:
    Coub();
    void Move(int mks);
    void Draw(HDC hdc);
    void SetBounds(RECT bbox);
};