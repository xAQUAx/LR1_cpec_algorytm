#include "Ball.h"
#include "pch.h"

Ball::Ball()
{
	this->x = 20;
	this->y = 20;
	this->r = 20;
	this->v_x = 30;
	this->v_y = 30;
}

void Ball::Move(int mks)
{
	this->x = this->x + v_x * mks / 1000000.0;
	y = y + v_y * mks / 1000000.0;

	if (((x + r) > bound_box.right) || ((x - r) < bound_box.left))
		v_x = -v_x;
	if (((y + r) > bound_box.bottom) || ((y - r) < bound_box.top))
		v_y = -v_y;
}

void Ball::Draw(HDC hdc)
{
	HPEN penBall = CreatePen(PS_SOLID, 3, RGB(150, 0, 0));
	HBRUSH ballBrush = CreateSolidBrush(RGB(200, 0, 200));
	SelectObject(hdc, penBall);
	SelectObject(hdc, ballBrush);
	Ellipse(hdc, x - r, y - r, x + r, y + r);
	DeleteObject(penBall);
	DeleteObject(ballBrush);
}

void Ball::SetBounds(RECT bbox)
{
	this->bound_box = bbox;
}