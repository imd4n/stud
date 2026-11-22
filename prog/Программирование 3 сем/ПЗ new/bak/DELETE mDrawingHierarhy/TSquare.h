#ifndef TSQUARE_H
#define TSQUARE_H

#include "TPoint.h"

class Square:public Point {
private:
    int width;
    int height;
public:
    Square(int initX, int initY, int _width, int _height);
    ~Square();

    int getWidth();
    int getHeight();

    void Show();
    void Hide();

    void moveTo(int newX, int newY);
};

#endif