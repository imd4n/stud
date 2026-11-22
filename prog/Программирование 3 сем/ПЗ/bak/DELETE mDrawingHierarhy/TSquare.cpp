#include "TSquare.h"
#include "windows.h"

extern HDC hdc;

Square::Square(int initX, int initY, int _width, int _height):Point(initX, initY) {
    // Простая установка переменных
    width = _width;
    height = _height;
}

Square::~Square() {}

void Square::Show() {
    Visible = true;
	HPEN Pen = CreatePen(PS_SOLID, 2, RGB(0,0,255));
	SelectObject(hdc, Pen);
	Rectangle(hdc, x, y, x+width, y+height);
	DeleteObject(Pen);
}

void Square::Hide() {
    Visible = true;
	HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255,0,0));
	SelectObject(hdc, Pen);
	Rectangle(hdc, x, y, x+width, y+height);
	DeleteObject(Pen);
}

void Square::moveTo(int newX, int newY) {
    Hide();
    x = newX;
    y = newY;
    Show();
}

int Square::getWidth() {
    return width;
}

int Square::getHeight() {
    return height;
}
