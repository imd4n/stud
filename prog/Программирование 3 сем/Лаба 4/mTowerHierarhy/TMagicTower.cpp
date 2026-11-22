#include "TMagicTower.h"
#include <wingdi.h>
#include <cmath>

extern HDC hdc;

MagicTower::MagicTower(int _x, int _y) : Tower(_x, _y) {
  /* Создание коллизии */
  setWidth(100);
  setHeight(450);
  setCollisionX(-10);
  setCollisionY(-450);
}

void MagicTower::show() {
  // Задали видимость
  Visible = true;

// Установка пера для контура башни - градиент от фиолетового к синему
HPEN towerPen = CreatePen(PS_SOLID, 3, RGB(100, 60, 180));
HPEN oldPen = (HPEN)SelectObject(hdc, towerPen);

int towerWidth = 100;
int towerHeight = 450;
int baseHeight = 40;

// Фундамент башни с магическими рунами
MoveToEx(hdc, x, y, NULL);
LineTo(hdc, x + towerWidth, y);
LineTo(hdc, x + towerWidth, y - baseHeight);
LineTo(hdc, x, y - baseHeight);
LineTo(hdc, x, y);

// Основная башня (изогнутая, волшебная форма)
int topWidth = towerWidth - 30;
POINT towerPoints[] = {
    {x, y - baseHeight},
    {x + towerWidth, y - baseHeight},
    {x + towerWidth + 10, y - towerHeight/3},
    {x + towerWidth - 5, y - towerHeight*2/3},
    {x + towerWidth/2 + topWidth/2, y - towerHeight},
    {x + towerWidth/2 - topWidth/2, y - towerHeight},
    {x + 5, y - towerHeight*2/3},
    {x - 10, y - towerHeight/3},
    {x, y - baseHeight}
};
Polyline(hdc, towerPoints, 9);

// Магические вихри на башне
HPEN swirlPen = CreatePen(PS_SOLID, 1, RGB(180, 140, 255));
SelectObject(hdc, swirlPen);
for (int i = 0; i < 4; i++) {
    int swirlX = x + 20 + i * 20;
    int swirlY = y - baseHeight - 80;
    for (int j = 0; j < 3; j++) {
        // Спиралевидные элементы
        Ellipse(hdc, swirlX - 8, swirlY - 8 - j*60, swirlX + 8, swirlY + 8 - j*60);
    }
}

// Верхняя часть с магическим кристаллом вместо шпиля
HPEN crystalPen = CreatePen(PS_SOLID, 2, RGB(200, 180, 255));
SelectObject(hdc, crystalPen);

int crystalHeight = 80;
POINT crystalPoints[] = {
    {x + towerWidth/2 - 8, y - towerHeight},
    {x + towerWidth/2, y - towerHeight - crystalHeight},
    {x + towerWidth/2 + 8, y - towerHeight},
    {x + towerWidth/2 - 8, y - towerHeight}
};
Polyline(hdc, crystalPoints, 4);

// Внутренние грани кристалла
MoveToEx(hdc, x + towerWidth/2, y - towerHeight - crystalHeight, NULL);
LineTo(hdc, x + towerWidth/2 - 4, y - towerHeight - crystalHeight/2);
MoveToEx(hdc, x + towerWidth/2, y - towerHeight - crystalHeight, NULL);
LineTo(hdc, x + towerWidth/2 + 4, y - towerHeight - crystalHeight/2);

// Парящие магические сферы вокруг башни
HPEN spherePen = CreatePen(PS_SOLID, 1, RGB(255, 220, 100));
SelectObject(hdc, spherePen);
for (int i = 0; i < 5; i++) {
    int sphereX = x - 30 + (i % 3) * 60;
    int sphereY = y - baseHeight - 150 - (i / 3) * 120;
    Ellipse(hdc, sphereX - 6, sphereY - 6, sphereX + 6, sphereY + 6);
    
    // Соединительные линии между сферами
    if (i > 0) {
        MoveToEx(hdc, sphereX, sphereY, NULL);
        LineTo(hdc, x - 30 + ((i-1) % 3) * 60, y - baseHeight - 150 - ((i-1) / 3) * 120);
    }
}

// Окна с магическим свечением
HPEN windowPen = CreatePen(PS_SOLID, 1, RGB(180, 220, 255));
SelectObject(hdc, windowPen);

for (int col = 0; col < 3; col++) {
    for (int floor = 0; floor < 10; floor++) {
        int windowX = x + 20 + col * 25;
        int windowY = y - baseHeight - 60 - floor * 35;
        
        // Арочные окна
        MoveToEx(hdc, windowX, windowY, NULL);
        LineTo(hdc, windowX, windowY - 20);
        MoveToEx(hdc, windowX + 12, windowY, NULL);
        LineTo(hdc, windowX + 12, windowY - 20);
        
        // Арка над окном
        Arc(hdc, windowX - 2, windowY - 25, windowX + 14, windowY - 15, 
            windowX + 14, windowY - 20, windowX - 2, windowY - 20);
        
        // Вертикальные разделители
        MoveToEx(hdc, windowX + 4, windowY, NULL);
        LineTo(hdc, windowX + 4, windowY - 20);
        MoveToEx(hdc, windowX + 8, windowY, NULL);
        LineTo(hdc, windowX + 8, windowY - 20);
        
        // Горизонтальные перемычки
        MoveToEx(hdc, windowX, windowY - 7, NULL);
        LineTo(hdc, windowX + 12, windowY - 7);
        MoveToEx(hdc, windowX, windowY - 14, NULL);
        LineTo(hdc, windowX + 12, windowY - 14);
    }
}

// Магические руны и символы на стенах
HPEN runePen = CreatePen(PS_SOLID, 2, RGB(255, 215, 0));
SelectObject(hdc, runePen);

// Рисуем различные магические символы
POINT symbols[] = {
    {x + 15, y - 120}, {x + 80, y - 180}, {x + 25, y - 250},
    {x + 70, y - 320}, {x + 20, y - 390}
};

for (int i = 0; i < 5; i++) {
    int symX = symbols[i].x;
    int symY = symbols[i].y;
    
    // Каждый символ - уникальная комбинация линий
    switch(i % 3) {
        case 0: // Круг с точкой
            Ellipse(hdc, symX - 8, symY - 8, symX + 8, symY + 8);
            Ellipse(hdc, symX - 2, symY - 2, symX + 2, symY + 2);
            break;
        case 1: // Треугольник
            MoveToEx(hdc, symX, symY - 8, NULL);
            LineTo(hdc, symX + 8, symY + 8);
            LineTo(hdc, symX - 8, symY + 8);
            LineTo(hdc, symX, symY - 8);
            break;
        case 2: // Пентаграмма
            for (int j = 0; j < 5; j++) {
                double angle1 = 2 * 3.14159 * j / 5 - 3.14159/2;
                double angle2 = 2 * 3.14159 * (j + 2) / 5 - 3.14159/2;
                
                int x1 = symX + 10 * cos(angle1);
                int y1 = symY + 10 * sin(angle1);
                int x2 = symX + 10 * cos(angle2);
                int y2 = symY + 10 * sin(angle2);
                
                MoveToEx(hdc, x1, y1, NULL);
                LineTo(hdc, x2, y2);
            }
            break;
    }
}

// Сияющие окна с магической энергией
HPEN glowPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 150));
SelectObject(hdc, glowPen);

POINT glowingWindows[] = {
    {x + 32, y - 95}, {x + 57, y - 165}, {x + 82, y - 235},
    {x + 32, y - 305}, {x + 82, y - 375}, {x + 57, y - 130},
    {x + 32, y - 200}, {x + 82, y - 270}
};

for (int i = 0; i < 8; i++) {
    int winX = glowingWindows[i].x;
    int winY = glowingWindows[i].y;
    
    // Лучи света из окон
    for (int ray = 0; ray < 4; ray++) {
        double angle = 2 * 3.14159 * ray / 4;
        int rayLength = 15;
        MoveToEx(hdc, winX + 6, winY - 10, NULL);
        LineTo(hdc, winX + 6 + rayLength * cos(angle), 
                 winY - 10 + rayLength * sin(angle));
    }
    
    // Яркая центральная точка
    Ellipse(hdc, winX + 4, winY - 12, winX + 8, winY - 8);
}

// Магический туман у основания
HPEN mistPen = CreatePen(PS_SOLID, 1, RGB(200, 180, 230));
SelectObject(hdc, mistPen);

for (int i = 0; i < 8; i++) {
    int mistX = x + i * 15;
    int mistY = y - baseHeight + 5;
    
    // Волнистые линии тумана
    for (int j = 0; j < 3; j++) {
        MoveToEx(hdc, mistX, mistY + j * 8, NULL);
        LineTo(hdc, mistX + 10, mistY + 5 + j * 8);
        LineTo(hdc, mistX + 20, mistY - 2 + j * 8);
        LineTo(hdc, mistX + 30, mistY + 3 + j * 8);
    }
}

// Восстановление старого пера
SelectObject(hdc, oldPen);

// Очистка объектов GDI
DeleteObject(towerPen);
DeleteObject(swirlPen);
DeleteObject(crystalPen);
DeleteObject(spherePen);
DeleteObject(windowPen);
DeleteObject(runePen);
DeleteObject(glowPen);
DeleteObject(mistPen);
}


void MagicTower::hide() {
  // Задали видимость
  Visible = false;


}