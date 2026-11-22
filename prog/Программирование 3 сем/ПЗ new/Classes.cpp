// Реализация методов в классе Location
#include "Classes.h"
#include <cmath>

// Конструктор
Location::Location(int initX, int initY) {
    x = initX;
    y = initY;
} // Location::Location();

// Деструктор
Location::~Location() {

}

// Геттеры
int Location::getX() {
    return x;
}
int Location::getY() {
    return y;
}

// Сеттеры
void Location::setX(int newX) {
    x = newX;
}
void Location::setY(int newY) {
    y = newY;
}

// Реализация методов в классе point
#include <windows.h>

// Удалите:
// extern HDC hdc;
extern HDC hdc;

// Конструктор, используем конструктор предка, но добавляем обработку нового поля
Point::Point(int initX, int initY):Location(initX, initY) {
    Visible = false;
}

// Деструктор
Point::~Point() {

}

// Показать обьект
void Point::Show() {
    Visible = true;

SetPixel(hdc, x, y, RGB(255, 0, 0));
    SetPixel(hdc, x+1, y, RGB(255, 0, 0));
    SetPixel(hdc, x+1, y+1, RGB(255, 0, 0));
    SetPixel(hdc, x, y+1, RGB(255, 0, 0));

SetPixel(hdc, x+2, y, RGB(255, 0, 0));
    SetPixel(hdc, x+2, y+1, RGB(255, 0, 0));
    SetPixel(hdc, x+2, y+2, RGB(255, 0, 0));
    SetPixel(hdc, x, y+1, RGB(255, 0, 0));

    SetPixel(hdc, x, y, RGB(255, 0, 0));
    SetPixel(hdc, x+1, y, RGB(255, 0, 0));
    SetPixel(hdc, x+1, y+1, RGB(255, 0, 0));
    SetPixel(hdc, x, y+1, RGB(255, 0, 0));
} // Show

// Скрыть обьект
 void Point::Hide() {
    Visible = false;
    SetPixel(hdc, x, y, RGB(0, 0, 255));
} // Hide

// Переместить обьект
void Point::moveTo(int newX, int newY) {
    Hide();     // Скрыли старую точку
    setX(newX); // Изменили координату Х
    setY(newY); // Изменили координату У
    Show();     // Показали новую точку
}

Circle::Circle(int InitX, int InitY, int InitRadius):Point(InitX, InitY)
{
	Radius = InitRadius;
}

Circle::~Circle() {}

void Circle::Show() {
	Visible = true;
	HPEN Pen = CreatePen(PS_SOLID, 2, RGB(0,0,255));
	SelectObject(hdc, Pen);
	Ellipse(hdc, x-Radius, y-Radius, x+Radius, y+Radius);
	DeleteObject(Pen);
} // Circle::Sow()

void Circle::Hide() {
	Visible = false;
	HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255,0,0));
	SelectObject(hdc, Pen);
	Ellipse(hdc, x-Radius, y-Radius, x+Radius, y+Radius);
	DeleteObject(Pen);
} // Circle::Sow()

void Circle::Expand(int DeltaRad) {
 Hide();
 Radius+=DeltaRad;
 if (Radius<2) {
	 Radius = 2;
	 Show();
	}
} // Circle::Expand()

void Circle::Reduce(int DeltaRad) {
	Expand(-DeltaRad);
} // Circle::Reduce()

/*void Circle::moveTo(int newX, int newY) {
    Hide();
    x = newX;
    y = newY;
    Show();
} */

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

Plane::Plane(int initX, int initY):Point(initX, initY) {
    // Зададим обьект коллизии
    CollisionObj = new Square(initX-45, initY, 90, 103);
    // Выделили память и задали линии для отрисокви
    lineArr = new Line*[17];
    int x0 = x;
    int y0 = y;
    lineArr[0] = new Line(x0, y0, x0 + 10, y0 + 10);
    lineArr[1] = new Line(x0, y0, x0 - 10, y0 + 10);
    lineArr[2] = new Line(x0+10, y0+10, x0+10, y0+50);
    lineArr[3] = new Line(x0-10, y0+10, x0-10, y0+50);
    lineArr[4] = new Line(x0+10, y0+50, x0+45, y0+65);
    lineArr[5] = new Line(x0-10, y0+50, x0-45, y0+65);
    lineArr[6] = new Line(x0+45, y0+65, x0+45, y0+75);
    lineArr[7] = new Line(x0-45, y0+65, x0-45, y0+75);
    lineArr[8] = new Line(x0+45, y0+75, x0+10, y0+75);
    lineArr[9] = new Line(x0-45, y0+75, x0-10, y0+75);
    lineArr[10] = new Line(x0+10, y0+75, x0+10, y0+95);
    lineArr[11] = new Line(x0-10, y0+75, x0-10, y0+95);
    lineArr[12] = new Line(x0+10, y0+95, x0+20, y0+100);
    lineArr[13] = new Line(x0-10, y0+95, x0-20, y0+100);
    lineArr[14] = new Line(x0+20, y0+100, x0+20, y0+103);
    lineArr[15] = new Line(x0-20, y0+100, x0-20, y0+103);
    lineArr[16] = new Line(x0+20, y0+103, x0-20, y0+103);
}

Plane::~Plane() {
    // Очистка динамической памяти
    for (int i = 0; i < 17; i++) {
        delete lineArr[i];
    }
    delete [] lineArr;
}

void Plane::Show() {
    // Показываем каждую линию через ее функцию
    for (int i = 0; i < 17; i++) {
        lineArr[i]->Show();
    }
}

void Plane::Hide() {
    // Прячем каждую линию через ее функцию
    for (int i = 0; i < 17; i++) {
        lineArr[i]->Hide();
    }
}

void Plane::moveTo(int newX, int newY) {
    x = newX;
    y = newY;
    // Находим сдвиг
    int deltaX = newX - this->getX();
    int deltaY = newY - this->getY();
    // Двигаем каждую линию
    for (int i = 0; i < 17; i++) {
        lineArr[i]->moveTo(lineArr[i]->getX()+deltaX, lineArr[i]->getY()+deltaY);
    }

    // Двигаем обьект коллизии

    //CollisionObj->Hide();
    CollisionObj->setX(CollisionObj->getX()+deltaX);
    CollisionObj->setY(CollisionObj->getY()+deltaY);
    //CollisionObj->Show();
}

Square *Plane::getCollisionObj() {
    return CollisionObj;
}

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
