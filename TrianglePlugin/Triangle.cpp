#include "Triangle.h"
#include "pch.h"

Triangle::Triangle()
{
	this->x = 50.0;
	this->y = 250.0;
	this->a = 70;
	this->h = 70;
	this->v_x = 20;
	this->v_y = 100;
}

void Triangle::Move(int mks)
{
	x = x + v_x * mks / 1000000.0;
	y = y + v_y * mks / 1000000.0;

	if (((x + a/2) > bound_box.right) || ((x - a/2) < bound_box.left))
		v_x = -v_x;
	if (((y + h/2) > bound_box.bottom) || ((y - h/2) < bound_box.top))
		v_y =-v_y;
}

void Triangle::Draw(HDC hdc)
{
	HPEN penTriang = CreatePen(PS_SOLID, 2, RGB(0, 0, 150));
	SelectObject(hdc, penTriang);
	MoveToEx(hdc, x - a/2, y + h/2, nullptr);
	LineTo(hdc, x, y-h/2);
	LineTo(hdc, x + a / 2, y + h/2);
	LineTo(hdc, x - a / 2, y + h / 2);
	DeleteObject(penTriang);
}

void Triangle::SetBounds(RECT bbox)
{
	this->bound_box = bbox;
}