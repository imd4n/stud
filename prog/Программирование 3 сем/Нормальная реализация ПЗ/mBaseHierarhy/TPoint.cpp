#include "TPoint.h"
// Реализация методов в классе point
#include <windows.h>
extern HDC hdc;

// Конструктор, используем конструктор предка, но добавляем обработку нового поля
Point::Point(int _x, int _y):Location(_x, _y) {
    Visible = false;
}

// Деструктор
Point::~Point() {
}

void Point::show() {
    Visible = true;

    SetPixel(hdc, x, y, RGB(0, 0, 255));
    SetPixel(hdc, x+1, y, RGB(0, 0, 255));
    SetPixel(hdc, x, y+1, RGB(0, 0, 255));
    SetPixel(hdc, x+1, y+1, RGB(0, 0, 255));
}

void Point::hide() {
    Visible = true;

    SetPixel(hdc, x, y, RGB(255, 0, 0));
    SetPixel(hdc, x+1, y, RGB(255, 0, 0));
    SetPixel(hdc, x, y+1, RGB(255, 0, 0));
    SetPixel(hdc, x+1, y+1, RGB(255, 0, 0));
}

bool Point::getVisible() {
    return Visible;
}

// Переместить обьект
void Point::moveTo(int newX, int newY) {
    hide();     // Скрыли старую точку
    setX(newX);        // Изменили координату Х
    setY(newY);        // Изменили координату У
    show();     // Показали новую точку
}