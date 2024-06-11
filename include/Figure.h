#pragma once
#include "Windows.h"

class Figure
{
public:
	virtual void Move(int mks) = 0;
	virtual void Draw(HDC hdc) = 0;
	virtual void SetBounds(RECT bbox) = 0;
	virtual ~Figure() {}
};