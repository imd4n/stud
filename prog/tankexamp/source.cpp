#include <windows.h>
#include "Header.h"
#include "interfaces.h"
#include <iostream>
#include <cmath>
#include <string>
using namespace std;

extern HDC hdc;
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

/*----------------------------------------*/
/*        Методы класса Location          */
/*----------------------------------------*/
Location::Location(int InitX, int InitY) { X = InitX; Y = InitY; }
Location::~Location() {}
int Location::GetX() const { return X; }
int Location::GetY() const { return Y; }
void Location::SetX(int NewX) { X = NewX; }
void Location::SetY(int NewY) { Y = NewY; }
void Location::SetPosition(int NewX, int NewY) { X = NewX; Y = NewY; }

/*----------------------------------------*/
/*        Методы класса Point             */
/*----------------------------------------*/
Point::Point(int InitX, int InitY) : Location(InitX, InitY) { Visible = false; }
Point::~Point() {}
bool Point::IsVisible() const { return Visible; }
void Point::Show()
{
    Visible = true;
    HPEN pen = CreatePen(PS_SOLID, 4, RGB(255, 0, 0));
    SelectObject(hdc, pen);
    MoveToEx(hdc, X - 5, Y, NULL); LineTo(hdc, X + 5, Y);
    MoveToEx(hdc, X, Y - 5, NULL); LineTo(hdc, X, Y + 5);
    Ellipse(hdc, X - 8, Y - 8, X + 8, Y + 8);
    DeleteObject(pen);
}
void Point::Hide()
{
    Visible = false;
    HPEN pen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
    HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, pen); SelectObject(hdc, brush);
    Rectangle(hdc, X - 10, Y - 10, X + 10, Y + 10);
    DeleteObject(brush); DeleteObject(pen);
}
void Point::MoveTo(int NewX, int NewY)
{
    Hide(); SetPosition(NewX, NewY); Show();
}
void Point::Drag(int Step)
{
    int FigX = GetX(), FigY = GetY();
    while (1)
    {
        if (KEY_DOWN(VK_ESCAPE)) break;
        if (KEY_DOWN(VK_LEFT)) { FigX -= Step; MoveTo(FigX, FigY); Sleep(200); }
        if (KEY_DOWN(VK_RIGHT)) { FigX += Step; MoveTo(FigX, FigY); Sleep(200); }
        if (KEY_DOWN(VK_DOWN)) { FigY += Step; MoveTo(FigX, FigY); Sleep(200); }
        if (KEY_DOWN(VK_UP)) { FigY -= Step; MoveTo(FigX, FigY); Sleep(200); }
    }
}

void CheckCollisions(Tank* activeTank, IInteractable** objects, int objectCount)
{
    if (activeTank->IsDestroyed()) return;
    for (int i = 0; i < objectCount; i++)
    {
        if (objects[i]->IsActive() && objects[i]->CheckCollision(activeTank))
        {
            objects[i]->OnCollision(activeTank);
        }
    }
}

/*----------------------------------------*/
/*        Методы класса Tank              */
/*----------------------------------------*/
Tank::Tank(int InitX, int InitY, int bW, int bH, int tW, int tH, int gL, int trH, int spd)
    : Point(InitX, InitY)
{
    bodyWidth = bW; bodyHeight = bH; turretWidth = tW; turretHeight = tH;
    gunLength = gL; trackHeight = trH; speed = spd;
    health = 100;
    isDestroyed = false;
    lostGun = false;
    lostTracks = false;
}
Tank::~Tank() {}
void Tank::Drag(int step)
{
    if (isDestroyed) return;
    int figX = GetX(), figY = GetY();
    while (1)
    {
        if (KEY_DOWN(VK_ESCAPE)) break;
        if (KEY_DOWN(VK_LEFT)) { figX -= speed; MoveTo(figX, figY); Sleep(200); }
        if (KEY_DOWN(VK_RIGHT)) { figX += speed; MoveTo(figX, figY); Sleep(200); }
        if (KEY_DOWN(VK_UP)) { figY -= speed; MoveTo(figX, figY); Sleep(200); }
        if (KEY_DOWN(VK_DOWN)) { figY += speed; MoveTo(figX, figY); Sleep(200); }
    }
}
void Tank::Show()
{
    if (isDestroyed) return;
    Visible = true;
    HPEN pen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
    HBRUSH brush = CreateSolidBrush(RGB(0, 128, 0));
    SelectObject(hdc, pen); SelectObject(hdc, brush);
    Rectangle(hdc, X, Y + bodyHeight, X + bodyWidth, Y + bodyHeight + trackHeight);
    DeleteObject(brush);
    brush = CreateSolidBrush(RGB(0, 100, 0)); SelectObject(hdc, brush);
    Rectangle(hdc, X + bodyWidth / 6, Y, X + bodyWidth - bodyWidth / 6, Y + bodyHeight);
    DeleteObject(brush);

    // Башня (если есть)
    if (turretHeight > 0) {
        int turretTop = Y - turretHeight;
        brush = CreateSolidBrush(RGB(0, 80, 0)); SelectObject(hdc, brush);
        Rectangle(hdc, X + (bodyWidth - turretWidth) / 2, turretTop,
                  X + (bodyWidth + turretWidth) / 2, turretTop + turretHeight);
        DeleteObject(brush);

        // Ствол исчезает при подрыве на мине или ударе
        if (gunLength > 0 && !ShouldLoseGun()) {
            int turretCenterX = X + bodyWidth/2;
            int turretCenterY = turretTop + turretHeight/2;
            int barrelWidth = gunLength;
            int barrelHeight = 20;
            brush = CreateSolidBrush(RGB(70, 70, 70)); SelectObject(hdc, brush);
            Rectangle(hdc, turretCenterX, turretCenterY - barrelHeight/2,
                      turretCenterX + barrelWidth, turretCenterY + barrelHeight/2);
            DeleteObject(brush);
        } else {
            // Очистка пушки если она была нарисована ранее
            HPEN blackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
            HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
            SelectObject(hdc, blackPen); SelectObject(hdc, blackBrush);
            int turretCenterX = X + bodyWidth/2;
            int turretCenterY = turretTop + turretHeight/2;
            int barrelWidth = gunLength;
            int barrelHeight = 22;
            Rectangle(hdc, turretCenterX-2, turretCenterY - barrelHeight/2-2,
                      turretCenterX + barrelWidth+2, turretCenterY + barrelHeight/2+2);
            DeleteObject(blackBrush); DeleteObject(blackPen);
        }
    }
    // Гусеницы исчезают при уроне
    if (!ShouldLoseTracks()) {
        brush = CreateSolidBrush(RGB(20,20,20)); SelectObject(hdc, brush);
        Ellipse(hdc, X + bodyWidth/6, Y + bodyHeight + trackHeight/4,
            X + bodyWidth/6 + trackHeight/2, Y + bodyHeight + trackHeight - trackHeight/4);
        Ellipse(hdc, X + bodyWidth/2 - trackHeight/4, Y + bodyHeight + trackHeight/4,
            X + bodyWidth/2 + trackHeight/4, Y + bodyHeight + trackHeight - trackHeight/4);
        Ellipse(hdc, X + bodyWidth - bodyWidth/6 - trackHeight/2, Y + bodyHeight + trackHeight/4,
            X + bodyWidth - bodyWidth/6, Y + bodyHeight + trackHeight - trackHeight/4);
        DeleteObject(brush);
    }
    DeleteObject(pen);

    // ---- Здоровье под танком ----
    HFONT oldFont = (HFONT)SelectObject(hdc, CreateFont(22, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Consolas"));
    SetBkMode(hdc, TRANSPARENT);
    COLORREF clr = (health > 50) ? RGB(0,180,0) : (health > 20) ? RGB(255,140,0) : RGB(255,0,0);
    SetTextColor(hdc, clr);
    wchar_t healthStr[32];
    wsprintfW(healthStr, L"HP: %d", health);
    int textX = X + bodyWidth/2 - 30;
    int textY = Y + bodyHeight + trackHeight + 5;
    TextOutW(hdc, textX, textY, healthStr, lstrlenW(healthStr));
    SelectObject(hdc, oldFont);
    SetTextColor(hdc, RGB(255,255,255));
    SetBkMode(hdc, OPAQUE);
}
void Tank::Hide()
{
    Visible = false;
    HPEN blackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, blackPen); SelectObject(hdc, blackBrush);
    int clearWidth = bodyWidth + gunLength + 30;
    int clearHeight = bodyHeight + trackHeight + (turretHeight > 0 ? turretHeight : 0) + 50;
    int clearX = X - 15;
    int clearY = Y - (turretHeight > 0 ? turretHeight : 0) - 10;
    Rectangle(hdc, clearX, clearY, clearX + clearWidth, clearY + clearHeight);
    DeleteObject(blackBrush); DeleteObject(blackPen);
}
void Tank::TakeDamage(int dmg)
{
    if (isDestroyed) return;
    health -= dmg;
    LosePartOnHit();
    cout << "Tank took " << dmg << " damage! Health: " << health << endl;
    if (health <= 0) {
        health = 0;
        Explode();
    }
}
void Tank::Heal(int amount)
{
    if (isDestroyed) return;
    health += amount;
    if (health > 100) health = 100;
    cout << "Tank healed by " << amount << " HP! Health: " << health << endl;
}
void Tank::Explode()
{
    if (isDestroyed) return;
    cout << "TANK DESTROYED! Explosion at (" << X << ", " << Y << ")" << endl;
    Hide();
    HPEN pen = CreatePen(PS_SOLID, 5, RGB(255, 100, 0));
    HBRUSH brush = CreateSolidBrush(RGB(255, 200, 0));
    SelectObject(hdc, pen); SelectObject(hdc, brush);
    Ellipse(hdc, X - 60, Y - 60, X + bodyWidth + 60, Y + bodyHeight + 60);
    DeleteObject(brush);
    brush = CreateSolidBrush(RGB(255, 150, 0)); SelectObject(hdc, brush);
    Ellipse(hdc, X - 40, Y - 40, X + bodyWidth + 40, Y + bodyHeight + 40);
    DeleteObject(brush);
    brush = CreateSolidBrush(RGB(255, 255, 0)); SelectObject(hdc, brush);
    Ellipse(hdc, X - 20, Y - 20, X + bodyWidth + 20, Y + bodyHeight + 20);
    DeleteObject(brush); DeleteObject(pen);
    Sleep(1000);
    HPEN blackPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
    HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, blackPen); SelectObject(hdc, blackBrush);
    Rectangle(hdc, X - 70, Y - 70, X + bodyWidth + 70, Y + bodyHeight + 70);
    DeleteObject(blackBrush); DeleteObject(blackPen);
    isDestroyed = true;
}
void Tank::OnCollision(Tank* tank)
{
    // При столкновении — деталь отлетает (например, пушка)
    LosePartOnHit();
}
int Tank::GetHealth() const { return health; }
int Tank::GetSpeed() const { return speed; }
int Tank::GetBodyWidth() const { return bodyWidth; }
int Tank::GetBodyHeight() const { return bodyHeight; }
bool Tank::IsDestroyed() const { return isDestroyed; }
void Tank::LosePartOnMine() { lostGun = true; }
void Tank::LosePartOnHit() { lostTracks = true; } // по заданию: у обычного танка — гусеницы
bool Tank::ShouldLoseGun() const { return lostGun; }
bool Tank::ShouldLoseTracks() const { return lostTracks; }
bool Tank::ShouldLoseTurret() const { return false; }

/*-------------------- HEAVY TANK ---------------------*/
HeavyTank::HeavyTank(int InitX, int InitY)
    : Tank(InitX, InitY, 320, 140, 140, 90, 200, 50, 20), lostGun(false)
{
    health = 150;
    targetX = InitX;
    targetY = InitY - 300;
}
HeavyTank::~HeavyTank() {}
void HeavyTank::SetTarget(int x, int y) { targetX = x; targetY = y; }
void HeavyTank::ShowTargetingSystem() {}
void HeavyTank::Show()
{
    // ВСЕГДА очищаем прежний рисунок (это уберёт "след" пушки)
    Hide();

    if (isDestroyed) return;
    HPEN pen = CreatePen(PS_SOLID, 3, RGB(50, 0, 0));
    HBRUSH brush = CreateSolidBrush(RGB(100, 50, 50));
    SelectObject(hdc, pen); SelectObject(hdc, brush);
    Rectangle(hdc, X, Y + bodyHeight, X + bodyWidth, Y + bodyHeight + trackHeight);
    DeleteObject(brush);
    brush = CreateSolidBrush(RGB(90, 30, 30)); SelectObject(hdc, brush);
    Rectangle(hdc, X + bodyWidth / 6, Y, X + bodyWidth - bodyWidth / 6, Y + bodyHeight);
    DeleteObject(brush);
    int turretTop = Y - turretHeight;
    brush = CreateSolidBrush(RGB(80, 20, 20)); SelectObject(hdc, brush);
    Rectangle(hdc, X + (bodyWidth - turretWidth)/2, turretTop,
              X + (bodyWidth + turretWidth)/2, turretTop + turretHeight);
    DeleteObject(brush);
    int turretCenterX = X + bodyWidth/2;
    int turretCenterY = turretTop + turretHeight/2;
    if (!ShouldLoseGun()) {
        double artilleryAngle = -M_PI/4;
        int gunEndX = turretCenterX + (int)(gunLength * cos(artilleryAngle));
        int gunEndY = turretCenterY + (int)(gunLength * sin(artilleryAngle));
        HPEN gunPen = CreatePen(PS_SOLID, 12, RGB(110, 60, 60));
        SelectObject(hdc, gunPen);
        MoveToEx(hdc, turretCenterX, turretCenterY, NULL);
        LineTo(hdc, gunEndX, gunEndY);
        DeleteObject(gunPen);
        HPEN rocketPen = CreatePen(PS_SOLID, 6, RGB(120, 120, 120));
        SelectObject(hdc, rocketPen);
        int offsetY = 25;
        int rocketEndX = turretCenterX + (int)((gunLength - 40) * cos(artilleryAngle));
        int rocketEndY1 = turretCenterY + (int)((gunLength - 40) * sin(artilleryAngle)) - offsetY;
        int rocketEndY2 = turretCenterY + (int)((gunLength - 40) * sin(artilleryAngle)) + offsetY;
        MoveToEx(hdc, turretCenterX, turretCenterY - offsetY, NULL);
        LineTo(hdc, rocketEndX, rocketEndY1);
        MoveToEx(hdc, turretCenterX, turretCenterY + offsetY, NULL);
        LineTo(hdc, rocketEndX, rocketEndY2);
        DeleteObject(rocketPen);
    }
    brush = CreateSolidBrush(RGB(40,20,20)); SelectObject(hdc, brush);
    Ellipse(hdc, X + bodyWidth/6, Y + bodyHeight + trackHeight/4,
        X + bodyWidth/6 + trackHeight/2, Y + bodyHeight + trackHeight - trackHeight/4);
    Ellipse(hdc, X + bodyWidth/2 - trackHeight/4, Y + bodyHeight + trackHeight/4,
        X + bodyWidth/2 + trackHeight/4, Y + bodyHeight + trackHeight - trackHeight/4);
    Ellipse(hdc, X + bodyWidth - bodyWidth/6 - trackHeight/2, Y + bodyHeight + trackHeight/4,
        X + bodyWidth - bodyWidth/6, Y + bodyHeight + trackHeight - trackHeight/4);
    DeleteObject(brush); DeleteObject(pen);

    // Здоровье
    HFONT oldFont = (HFONT)SelectObject(hdc, CreateFont(22, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Consolas"));
    SetBkMode(hdc, TRANSPARENT);
    COLORREF clr = (health > 50) ? RGB(0,180,0) : (health > 20) ? RGB(255,140,0) : RGB(255,0,0);
    SetTextColor(hdc, clr);
    wchar_t healthStr[32];
    wsprintfW(healthStr, L"HP: %d", health);
    int textX = X + bodyWidth/2 - 30;
    int textY = Y + bodyHeight + trackHeight + 5;
    TextOutW(hdc, textX, textY, healthStr, lstrlenW(healthStr));
    SelectObject(hdc, oldFont);
    SetTextColor(hdc, RGB(255,255,255));
    SetBkMode(hdc, OPAQUE);
}

void HeavyTank::Hide()
{
    Visible = false;
    HPEN blackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, blackPen); SelectObject(hdc, blackBrush);

    // 1. Корпус + башня + гусеницы
    int clearWidth = bodyWidth + 20;
    int clearHeight = bodyHeight + trackHeight + turretHeight + 20;
    int clearX = X - 10;
    int clearY = Y - turretHeight - 10;
    Rectangle(hdc, clearX, clearY, clearX + clearWidth, clearY + clearHeight);

    // 2. Диагональная основная пушка (широкий прямоугольник)
    int turretCenterX = X + bodyWidth/2;
    int turretCenterY = (Y - turretHeight) + turretHeight/2;
    double artilleryAngle = -M_PI/4;
    int gunEndX = turretCenterX + (int)(gunLength * cos(artilleryAngle));
    int gunEndY = turretCenterY + (int)(gunLength * sin(artilleryAngle));
    int left = min(turretCenterX, gunEndX) - 20;
    int top = min(turretCenterY, gunEndY) - 20;
    int right = max(turretCenterX, gunEndX) + 20;
    int bottom = max(turretCenterY, gunEndY) + 20;
    Rectangle(hdc, left, top, right, bottom);

    // 3. Очистка области под счет здоровья (чтобы не оставался след)
    // Размеры зависят от того, как ты рисуешь текст здоровья в Show
    int healthTextX = X + bodyWidth / 2 - 40;
    int healthTextY = Y + bodyHeight + trackHeight + 2;
    int healthTextW = 100; // ширина под текст "HP: 100"
    int healthTextH = 40; // высота под шрифт

    Rectangle(hdc, healthTextX, healthTextY, healthTextX + healthTextW, healthTextY + healthTextH);

    DeleteObject(blackBrush); DeleteObject(blackPen);
}
void HeavyTank::TakeDamage(int dmg)
{
    if (isDestroyed) return;
    health -= dmg / 2;
    LosePartOnHit();
    cout << "Heavy Tank took " << (dmg/2) << " damage (armor reduced)! Health: " << health << endl;
    if (health <= 0) {
        health = 0;
        Explode();
    }
}

/*-------------------- LIGHT TANK ---------------------*/
LightTank::LightTank(int InitX, int InitY)
    : Tank(InitX, InitY, 160, 70, 0, 0, 0, 20, 50), lostTracks(false)
{
    health = 60;
}
LightTank::~LightTank() {}
void LightTank::Show()
{
    if (isDestroyed) return;
    HPEN pen = CreatePen(PS_SOLID, 3, RGB(0, 0, 128));
    HBRUSH brush = CreateSolidBrush(RGB(100, 100, 180));
    SelectObject(hdc, pen); SelectObject(hdc, brush);
    Rectangle(hdc, X, Y + bodyHeight, X + bodyWidth, Y + bodyHeight + trackHeight);
    DeleteObject(brush);

    brush = CreateSolidBrush(RGB(80, 80, 180)); SelectObject(hdc, brush);
    Rectangle(hdc, X + bodyWidth / 8, Y, X + bodyWidth - bodyWidth / 8, Y + bodyHeight);
    DeleteObject(brush);

    if (!ShouldLoseTracks()) {
        brush = CreateSolidBrush(RGB(60,60,100)); SelectObject(hdc, brush);
        Ellipse(hdc, X + bodyWidth/4, Y + bodyHeight + trackHeight/4,
            X + bodyWidth/4 + trackHeight/2, Y + bodyHeight + trackHeight - trackHeight/4);
        Ellipse(hdc, X + bodyWidth*3/4 - trackHeight/2, Y + bodyHeight + trackHeight/4,
            X + bodyWidth*3/4, Y + bodyHeight + trackHeight - trackHeight/4);
        DeleteObject(brush);
    } else {
        // Очистка гусениц
        HPEN blackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
        SelectObject(hdc, blackPen); SelectObject(hdc, blackBrush);
        Rectangle(hdc, X + bodyWidth/4 - 5, Y + bodyHeight + trackHeight/4 - 5,
                  X + bodyWidth*3/4 + 5, Y + bodyHeight + trackHeight - trackHeight/4 + 5);
        DeleteObject(blackBrush); DeleteObject(blackPen);
    }
    DeleteObject(pen);

    HFONT oldFont = (HFONT)SelectObject(hdc, CreateFont(22, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Consolas"));
    SetBkMode(hdc, TRANSPARENT);
    COLORREF clr = (health > 30) ? RGB(0,180,0) : (health > 10) ? RGB(255,140,0) : RGB(255,0,0);
    SetTextColor(hdc, clr);
    wchar_t healthStr[32];
    wsprintfW(healthStr, L"HP: %d", health);
    int textX = X + bodyWidth/2 - 30;
    int textY = Y + bodyHeight + trackHeight + 5;
    TextOutW(hdc, textX, textY, healthStr, lstrlenW(healthStr));
    SelectObject(hdc, oldFont);
    SetTextColor(hdc, RGB(255,255,255));
    SetBkMode(hdc, OPAQUE);
}
void LightTank::TakeDamage(int dmg)
{
    if (isDestroyed) return;
    health -= dmg * 2;
    LosePartOnHit();
    cout << "Light Tank took " << (dmg*2) << " damage (weak armor)! Health: " << health << endl;
    if (health <= 0) {
        health = 0;
        Explode();
    }
}

/*-------------------- DAMAGE TANK ---------------------*/
DamageTank::DamageTank(int InitX, int InitY)
    : Tank(InitX, InitY, 240, 100, 100, 60, 120, 35, 35), lostTurret(false)
{
    health = 120;
}
DamageTank::~DamageTank() {}
void DamageTank::Show()
{
    if (isDestroyed) return;
    HPEN pen = CreatePen(PS_SOLID, 3, RGB(100, 100, 0));
    HBRUSH brush = CreateSolidBrush(RGB(180, 180, 80));
    SelectObject(hdc, pen); SelectObject(hdc, brush);
    Rectangle(hdc, X, Y + bodyHeight, X + bodyWidth, Y + bodyHeight + trackHeight);
    DeleteObject(brush);
    brush = CreateSolidBrush(RGB(180, 170, 60)); SelectObject(hdc, brush);
    Rectangle(hdc, X + bodyWidth / 6, Y, X + bodyWidth - bodyWidth / 6, Y + bodyHeight);
    DeleteObject(brush);

    int turretTop = Y - turretHeight;
    if (!ShouldLoseTurret()) {
        brush = CreateSolidBrush(RGB(130, 130, 30)); SelectObject(hdc, brush);
        Rectangle(hdc, X + (bodyWidth - turretWidth)/2, turretTop,
                  X + (bodyWidth + turretWidth)/2, turretTop + turretHeight);
        DeleteObject(brush);
        int turretCenterX = X + bodyWidth/2;
        int turretCenterY = turretTop + turretHeight/2;
        brush = CreateSolidBrush(RGB(220, 200, 40)); SelectObject(hdc, brush);
        Rectangle(hdc, turretCenterX, turretCenterY - 20,
                  turretCenterX + gunLength, turretCenterY - 12);
        Rectangle(hdc, turretCenterX, turretCenterY + 12,
                  turretCenterX + gunLength, turretCenterY + 20);
        DeleteObject(brush);
        brush = CreateSolidBrush(RGB(200, 180, 20)); SelectObject(hdc, brush);
        Rectangle(hdc, turretCenterX, turretCenterY - 6,
                  turretCenterX + gunLength + 20, turretCenterY + 6);
        DeleteObject(brush);
    } else {
        // Очистка башни
        HPEN blackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
        SelectObject(hdc, blackPen); SelectObject(hdc, blackBrush);
        int left = X + (bodyWidth - turretWidth)/2 - 10;
        int right = X + (bodyWidth + turretWidth)/2 + gunLength + 40;
        int top = turretTop - 20;
        int bottom = turretTop + turretHeight + 40;
        Rectangle(hdc, left, top, right, bottom);
        DeleteObject(blackBrush); DeleteObject(blackPen);
    }
    brush = CreateSolidBrush(RGB(100, 100, 0)); SelectObject(hdc, brush);
    Ellipse(hdc, X + bodyWidth/6, Y + bodyHeight + trackHeight/4,
        X + bodyWidth/6 + trackHeight/2, Y + bodyHeight + trackHeight - trackHeight/4);
    Ellipse(hdc, X + bodyWidth/2 - trackHeight/4, Y + bodyHeight + trackHeight/4,
        X + bodyWidth/2 + trackHeight/4, Y + bodyHeight + trackHeight - trackHeight/4);
    Ellipse(hdc, X + bodyWidth - bodyWidth/6 - trackHeight/2, Y + bodyHeight + trackHeight/4,
        X + bodyWidth - bodyWidth/6, Y + bodyHeight + trackHeight - trackHeight/4);
    DeleteObject(brush); DeleteObject(pen);

    HFONT oldFont = (HFONT)SelectObject(hdc, CreateFont(22, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Consolas"));
    SetBkMode(hdc, TRANSPARENT);
    COLORREF clr = (health > 60) ? RGB(0,180,0) : (health > 20) ? RGB(255,140,0) : RGB(255,0,0);
    SetTextColor(hdc, clr);
    wchar_t healthStr[32];
    wsprintfW(healthStr, L"HP: %d", health);
    int textX = X + bodyWidth/2 - 30;
    int textY = Y + bodyHeight + trackHeight + 5;
    TextOutW(hdc, textX, textY, healthStr, lstrlenW(healthStr));
    SelectObject(hdc, oldFont);
    SetTextColor(hdc, RGB(255,255,255));
    SetBkMode(hdc, OPAQUE);
}
void DamageTank::TakeDamage(int dmg)
{
    if (isDestroyed) return;
    health -= dmg;
    LosePartOnHit();
    cout << "Damage Tank took " << dmg << " damage! Health: " << health << endl;
    if (health <= 0) {
        health = 0;
        Explode();
    }
}

/*----------------------------------------*/
/*          Rocket - наследуется от Tank   */
/*----------------------------------------*/
Rocket::Rocket(int InitX, int InitY, int targetX, int targetY, int pwr, int w, int h, int spd)
    : Tank(InitX, InitY, w, h, 0, 0, 0, 0, spd), power(pwr), isActive(true), targetX(targetX), targetY(targetY)
{
    // Вычисляем направление к цели
    int deltaX = targetX - InitX;
    int deltaY = targetY - InitY;
    double distance = sqrt(deltaX * deltaX + deltaY * deltaY);

    if (distance > 0) {
        directionX = (int)(speed * deltaX / distance);
        directionY = (int)(speed * deltaY / distance);
    } else {
        directionX = speed;
        directionY = 0;
    }
}
Rocket::~Rocket() {}

void Rocket::Show()
{
    if (!isActive) return;

    Visible = true;
    HPEN penBody = CreatePen(PS_SOLID, 3, RGB(255, 100, 0));
    HBRUSH brushBody = CreateSolidBrush(RGB(255, 200, 0));
    SelectObject(hdc, penBody); SelectObject(hdc, brushBody);
    Ellipse(hdc, X - bodyWidth/2, Y - bodyHeight/2, X + bodyWidth/2, Y + bodyHeight/2);
    DeleteObject(brushBody); DeleteObject(penBody);

    // Хвост ракеты
    HPEN penTail = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    SelectObject(hdc, penTail);
    MoveToEx(hdc, X - bodyWidth/2, Y, NULL); LineTo(hdc, X - bodyWidth, Y);
    DeleteObject(penTail);
}

void Rocket::Hide()
{
    if (!isActive) return;

    Visible = false;
    HPEN blackPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
    HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, blackPen); SelectObject(hdc, blackBrush);
    Rectangle(hdc, X - bodyWidth, Y - bodyHeight, X + bodyWidth, Y + bodyHeight);
    DeleteObject(blackBrush); DeleteObject(blackPen);
}

void Rocket::MoveTo(int NewX, int NewY)
{
    if (!isActive) return;
    Hide();
    SetPosition(NewX, NewY);
    Show();
}

bool Rocket::CheckCollision(Tank* tank)
{
    if (!isActive || tank->IsDestroyed() || tank == this) return false;

    int tankCenterX = tank->GetX() + tank->GetBodyWidth() / 2;
    int tankCenterY = tank->GetY() + tank->GetBodyHeight() / 2;
    int distance = sqrt((tankCenterX - X) * (tankCenterX - X) + (tankCenterY - Y) * (tankCenterY - Y));
    return distance < 60;
}

void Rocket::OnCollision(Tank* tank)
{
    cout << "ROCKET HIT! Tank took " << power << " damage!" << endl;
    tank->TakeDamage(power);
    Deactivate();

    // Анимация взрыва
    HPEN pen = CreatePen(PS_SOLID, 5, RGB(255, 255, 0));
    HBRUSH brush = CreateSolidBrush(RGB(255, 150, 0));
    SelectObject(hdc, pen); SelectObject(hdc, brush);
    Ellipse(hdc, X - 40, Y - 40, X + 40, Y + 40);
    DeleteObject(brush); DeleteObject(pen);
    Sleep(300);
}

void Rocket::MoveToTarget()
{
    if (!isActive) return;

    int newX = X + directionX;
    int newY = Y + directionY;

    // Проверка границ экрана
    if (newX < 0 || newX > 2000 || newY < 0 || newY > 1200) {
        Deactivate();
        return;
    }

    MoveTo(newX, newY);
}

void Rocket::Deactivate()
{
    isActive = false;
    Hide();
}

int Rocket::GetPower() const { return power; }

/*----------------------------------------*/
/*          Mina - наследуется от Tank     */
/*----------------------------------------*/
Mina::Mina(int InitX, int InitY, int dmg, int w, int h)
    : Tank(InitX, InitY, w, h, 0, 0, 0, 0, 0), damage(dmg), isExploded(false) {}
Mina::~Mina() {}

void Mina::Show()
{
    if (isExploded) return;

    Visible = true;
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(50, 50, 50));
    HBRUSH brush = CreateSolidBrush(RGB(100, 100, 100));
    SelectObject(hdc, pen); SelectObject(hdc, brush);
    Ellipse(hdc, X - bodyWidth/2, Y - bodyHeight/2, X + bodyWidth/2, Y + bodyHeight/2);
    DeleteObject(brush);

    // Шипы мины
    MoveToEx(hdc, X - bodyWidth/2, Y, NULL); LineTo(hdc, X - bodyWidth/2 - 5, Y);
    MoveToEx(hdc, X + bodyWidth/2, Y, NULL); LineTo(hdc, X + bodyWidth/2 + 5, Y);
    MoveToEx(hdc, X, Y - bodyHeight/2, NULL); LineTo(hdc, X, Y - bodyHeight/2 - 5);
    MoveToEx(hdc, X, Y + bodyHeight/2, NULL); LineTo(hdc, X, Y + bodyHeight/2 + 5);

    DeleteObject(pen);
}

void Mina::Hide()
{
    Visible = false;
    HPEN blackPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
    HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, blackPen); SelectObject(hdc, blackBrush);
    Rectangle(hdc, X - bodyWidth, Y - bodyHeight, X + bodyWidth, Y + bodyHeight);
    DeleteObject(blackBrush); DeleteObject(blackPen);
}

bool Mina::CheckCollision(Tank* tank)
{
    if (isExploded || tank->IsDestroyed() || tank == this) return false;

    int tankCenterX = tank->GetX() + tank->GetBodyWidth() / 2;
    int tankCenterY = tank->GetY() + tank->GetBodyHeight() / 2;
    int distance = sqrt((tankCenterX - X) * (tankCenterX - X) + (tankCenterY - Y) * (tankCenterY - Y));
    return distance < 40;
}

void Mina::OnCollision(Tank* tank)
{
    cout << "BOOM! Tank hit a mine!" << endl;
    tank->TakeDamage(damage);
    Explode();
}

void Mina::Explode()
{
    if (isExploded) return;

    Hide();
    // Анимация взрыва
    HPEN pen = CreatePen(PS_SOLID, 3, RGB(255, 255, 0));
    HBRUSH brush = CreateSolidBrush(RGB(255, 100, 0));
    SelectObject(hdc, pen); SelectObject(hdc, brush);
    Ellipse(hdc, X - 30, Y - 30, X + 30, Y + 30);
    DeleteObject(brush); DeleteObject(pen);
    Sleep(500);

    // Очистка взрыва
    HPEN blackPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
    HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, blackPen); SelectObject(hdc, blackBrush);
    Rectangle(hdc, X - 35, Y - 35, X + 35, Y + 35);
    DeleteObject(blackBrush); DeleteObject(blackPen);

    isExploded = true;
}

int Mina::GetDamage() const { return damage; }
bool Mina::IsExploded() const { return isExploded; }

/*----------------------------------------*/
/*        Remont - наследуется от Tank     */
/*----------------------------------------*/
Remont::Remont(int InitX, int InitY, int heal, int w, int h)
    : Tank(InitX, InitY, w, h, 0, 0, 0, 0, 0), healAmount(heal), isUsed(false) {}
Remont::~Remont() {}

void Remont::Show()
{
    if (isUsed) return;

    Visible = true;

    // Зеленый прямоугольник
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 100, 0));
    HBRUSH brush = CreateSolidBrush(RGB(0, 200, 0));
    SelectObject(hdc, pen); SelectObject(hdc, brush);
    Rectangle(hdc, X - bodyWidth/2, Y - bodyHeight/2,
              X + bodyWidth/2, Y + bodyHeight/2);
    DeleteObject(brush);

    // Символ H
    pen = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
    SelectObject(hdc, pen);
    MoveToEx(hdc, X - 10, Y - 10, NULL); LineTo(hdc, X - 10, Y + 10);
    MoveToEx(hdc, X + 10, Y - 10, NULL); LineTo(hdc, X + 10, Y + 10);
    MoveToEx(hdc, X - 10, Y, NULL); LineTo(hdc, X + 10, Y);

    DeleteObject(pen);
}

void Remont::Hide()
{
    Visible = false;
    HPEN blackPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
    HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, blackPen); SelectObject(hdc, blackBrush);
    Rectangle(hdc, X - bodyWidth, Y - bodyHeight,
              X + bodyWidth, Y + bodyHeight);
    DeleteObject(blackBrush); DeleteObject(blackPen);
}

bool Remont::CheckCollision(Tank* tank)
{
    if (isUsed || tank->IsDestroyed() || tank == this) return false;

    int tankCenterX = tank->GetX() + tank->GetBodyWidth() / 2;
    int tankCenterY = tank->GetY() + tank->GetBodyHeight() / 2;
    return (abs(tankCenterX - X) < 50 && abs(tankCenterY - Y) < 40);
}

void Remont::OnCollision(Tank* tank)
{
    tank->Heal(healAmount);
    Use();
}

void Remont::Use()
{
    isUsed = true;
    Hide();
}

int Remont::GetHealAmount() const { return healAmount; }
bool Remont::IsUsed() const { return isUsed; }

// ---------------- RoundHeavyTank ----------------
RoundHeavyTank::RoundHeavyTank(int InitX, int InitY)
    : HeavyTank(InitX, InitY)
{
    // всё остальное настраивается в конструкторе HeavyTank
}

RoundHeavyTank::~RoundHeavyTank() {}

void RoundHeavyTank::Show()
{
    // сначала стираем всё, что рисовали ранее (т.е. и ствол)
    Hide();
    if (isDestroyed) return;

    // — корпус и гусеницы (копируем из HeavyTank)
    HPEN   pen   = CreatePen(PS_SOLID, 3, RGB(50, 0, 0));
    HBRUSH brush = CreateSolidBrush(RGB(100, 50, 50));
    SelectObject(hdc, pen);
    SelectObject(hdc, brush);
    Rectangle(hdc, X, Y + bodyHeight, X + bodyWidth, Y + bodyHeight + trackHeight);
    DeleteObject(brush);

    brush = CreateSolidBrush(RGB(90, 30, 30));
    SelectObject(hdc, brush);
    Rectangle(hdc,
              X + bodyWidth / 6, Y,
              X + bodyWidth - bodyWidth / 6, Y + bodyHeight);
    DeleteObject(brush);

    // — круглая башня
    int turretTop = Y - turretHeight;
    int centerX   = X + bodyWidth / 2;
    int centerY   = turretTop + turretHeight / 2;
    int rX        = turretWidth  / 2;
    int rY        = turretHeight / 2;

    brush = CreateSolidBrush(RGB(80, 20, 20));
    SelectObject(hdc, brush);
    Ellipse(hdc,
            centerX - rX, centerY - rY,
            centerX + rX, centerY + rY);
    DeleteObject(brush);

    // — ствол и ракеты (копируем из HeavyTank)
    if (!ShouldLoseGun()) {
        double angle = -M_PI/4;
        int gEndX = centerX + int(gunLength * cos(angle));
        int gEndY = centerY + int(gunLength * sin(angle));

        HPEN gunPen = CreatePen(PS_SOLID, 12, RGB(110, 60, 60));
        SelectObject(hdc, gunPen);
        MoveToEx(hdc, centerX, centerY, NULL);
        LineTo(hdc, gEndX, gEndY);
        DeleteObject(gunPen);

        HPEN rocketPen = CreatePen(PS_SOLID, 6, RGB(120, 120, 120));
        SelectObject(hdc, rocketPen);
        int off = 25;
        int rEndX = centerX + int((gunLength - 40) * cos(angle));
        int rEndY = centerY + int((gunLength - 40) * sin(angle));
        MoveToEx(hdc, centerX, centerY - off, NULL);
        LineTo(hdc, rEndX, rEndY - off);
        MoveToEx(hdc, centerX, centerY + off, NULL);
        LineTo(hdc, rEndX, rEndY + off);
        DeleteObject(rocketPen);
    }

    // — катки (копируем из HeavyTank)
    brush = CreateSolidBrush(RGB(40,20,20));
    SelectObject(hdc, brush);
    Ellipse(hdc, X + bodyWidth/6,                     Y + bodyHeight + trackHeight/4,
                   X + bodyWidth/6 + trackHeight/2,   Y + bodyHeight + trackHeight - trackHeight/4);
    Ellipse(hdc, X + bodyWidth/2 - trackHeight/4,     Y + bodyHeight + trackHeight/4,
                   X + bodyWidth/2 + trackHeight/4,   Y + bodyHeight + trackHeight - trackHeight/4);
    Ellipse(hdc, X + bodyWidth - bodyWidth/6 - trackHeight/2, Y + bodyHeight + trackHeight/4,
                   X + bodyWidth - bodyWidth/6,       Y + bodyHeight + trackHeight - trackHeight/4);
    DeleteObject(brush);
    DeleteObject(pen);

    // — текст HP (копируем из HeavyTank)
    HFONT oldFont = (HFONT)SelectObject(hdc,
        CreateFont(22,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,
                   RUSSIAN_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
                   DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,L"Consolas"));
    SetBkMode(hdc, TRANSPARENT);
    COLORREF clr = (health > 50) ? RGB(0,180,0)
                  : (health > 20) ? RGB(255,140,0)
                                  : RGB(255,0,0);
    SetTextColor(hdc, clr);
    wchar_t buf[32];
    wsprintfW(buf, L"HP: %d", health);
    TextOutW(hdc,
             X + bodyWidth/2 - 30,
             Y + bodyHeight + trackHeight + 5,
             buf, lstrlenW(buf));
    SelectObject(hdc, oldFont);
    SetTextColor(hdc, RGB(255,255,255));
    SetBkMode(hdc, OPAQUE);
}

void RoundHeavyTank::Hide()
{
    Visible = false;

    // вынос ствола по X и Y
    double angle = -M_PI/4;
    int extX = abs(int(gunLength * cos(angle)));
    int extY = abs(int(gunLength * sin(angle)));

    // высота области, где рисуется текст
    const int textHeight = 22 + 5;

    // минимальный буфер вокруг всех краёв в 2 пикселя
    const int BUF = 2;

    // границы прямоугольника очистки
    int clearLeft   = X - BUF;
    int clearTop    = Y - turretHeight - extY - BUF;
    int clearRight  = X + bodyWidth + extX + BUF;
    int clearBottom = Y + bodyHeight + trackHeight + textHeight + BUF;

    HPEN   blackPen   = CreatePen(PS_SOLID, 1, RGB(0,0,0));
    HBRUSH blackBrush = CreateSolidBrush(RGB(0,0,0));
    SelectObject(hdc, blackPen);
    SelectObject(hdc, blackBrush);

    Rectangle(hdc,
              clearLeft,
              clearTop,
              clearRight,
              clearBottom);

    DeleteObject(blackBrush);
    DeleteObject(blackPen);
}
