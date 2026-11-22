#include "TLine.h"
#include <cmath>
#include "windows.h"

extern HDC hdc;

Line::Line(int initX1, int initY1, int initX2, int initY2):Point(initX1, initY1) {
    angle = atan2(double(initY2-initY1),double(initX2-initX1));
    length = sqrt(pow(initX2-initX1,2)+pow(initY2-initY1, 2));
}

Line::Line(int initX, int initY, double _angle, double _length):Point(initX, initY) {
    angle = _angle;
    length = _length;
}

Line::~Line() {}

void Line::Show() {
    int x2 = x + length*cos(angle);
    int y2 = y + length*sin(angle);

    Visible = true;
	HPEN Pen = CreatePen(PS_SOLID, 2, RGB(0,0,255));
	SelectObject(hdc, Pen);
	MoveToEx(hdc, x, y, NULL);
    LineTo(hdc, x2, y2);
	DeleteObject(Pen);
}

void Line::Hide() {
    int x2 = x + length*cos(angle);
    int y2 = y + length*sin(angle);

    Visible = true;
	HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255,0,0));
	SelectObject(hdc, Pen);
	MoveToEx(hdc, x, y, NULL);
    LineTo(hdc, x2, y2);
	DeleteObject(Pen);
}

void Line::moveTo(int newX, int newY) {
    Hide();
    x = newX;
    y = newY;
    Show();
}