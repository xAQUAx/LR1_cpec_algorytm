#include "Coub.h"
#include "pch.h"

Coub::Coub()
{
	this->x = 500.0;
	this->y = 50.0;
	this->a = 50.0;
	this->v_x = -70.0;
	this->v_y = 45.0;
}

void Coub::Move(int mks)
{
	x = x + v_x * mks / 1000000.0;
	y = y + v_y * mks / 1000000.0;

	if (((x + a/2) > bound_box.right) || ((x - a/2) < bound_box.left))
		v_x = -v_x;
	if (((y + a/2) > bound_box.bottom) || ((y - a/2) < bound_box.top))
		v_y = -v_y;
}

void Coub::Draw(HDC hdc)
{
	HPEN penCoub = CreatePen(PS_SOLID, 3, RGB(0, 0, 150));
	HBRUSH coubBrush = CreateSolidBrush(RGB(100, 0, 0));
	SelectObject(hdc, coubBrush);
	SelectObject(hdc, penCoub);

	Rectangle(hdc, x - a/2, y - a/2, x + a/2, y + a/2);
	DeleteObject(coubBrush);
	DeleteObject(penCoub);

}

void Coub::SetBounds(RECT bbox)
{
	this->bound_box = bbox;
}