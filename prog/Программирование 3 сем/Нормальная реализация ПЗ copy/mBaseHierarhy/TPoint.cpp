#include "TPoint.h"
// Реализация методов в классе point
#include <windows.h>
extern HDC hdc;

// Конструктор, используем конструктор предка, но добавляем обработку нового поля
Point::Point(int _x, int _y, int _mapN, int _mapM):Location(_x, _y) {
    Visible = false;
    mapN = _mapN;
    mapM = _mapM;
    // Выделяем память под обьект
    bitMap = new u_char [mapN*mapM/8];
}

// Деструктор
Point::~Point() {
    delete[] bitMap;
}

void Point::drawObj(u_char r, u_char g, u_char b) {
// Проходим по каждому байту
    u_char * ptr = bitMap;
    int thisBit = 0;
    for (int i = 0; i < mapN*mapM/8; ++i) {
        // Разыменовываем указатель, чтобы получить значение байта
        unsigned char current_byte = *ptr; 
        for (int j = 7; j >= 0; --j) {
            thisBit++;
            if(((current_byte >> j) & 1)) {// Выводим биты
                int thisN = thisBit/mapN;
                int thisM = thisBit%mapM;
                
                SetPixel(hdc, x+thisM, y+thisN, RGB(r, g, b));
            }
        }
        
        // Переходим к следующему байту
        ptr++; 
    }
}

void Point::show() {
    drawObj(255, 0, 0);
}

void Point::hide() {
    drawObj(0, 0, 255);
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