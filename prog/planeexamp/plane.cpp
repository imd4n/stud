/********************************************************************
 *          ЛАБОРАТОРНАЯ РАБОТА ПО ООП                               *
 *------------------------------------------------------------------*
 * Проект        : Flappy Plane (вариант "Самолёт")                 *
 * Файл          : plane.cpp                                        *
 * Комментарий   : Реализация Location, Point, Plane, SkyScraper,   *
 *                 его подклассов и игровой сцены Game.             *
 *******************************************************************/

#include "plane.h"

#include <cmath>
#include <cstdio>
#include <cwchar>

/*------------------------------------------------------------------*/
/*                     ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ                      */
/*------------------------------------------------------------------*/
namespace {

const double PI = 3.14159265358979323846;

int ClampInt(int Value, int MinValue, int MaxValue)
{
    if (Value < MinValue) return MinValue;
    if (Value > MaxValue) return MaxValue;
    return Value;
}

bool IsKeyDown(int VirtualKey)
{
    return (GetAsyncKeyState(VirtualKey) & 0x8000) != 0;
}

} // namespace

/*------------------------------------------------------------------*/
/*                  РЕАЛИЗАЦИЯ GDI-ХЕЛПЕРОВ                         */
/*------------------------------------------------------------------*/
namespace Gfx {

void FillRect(HDC Dc, int Left, int Top, int Right, int Bottom,
              COLORREF Fill, COLORREF Border, int BorderWidth)
{
    HPEN Pen = (Border != CLR_INVALID)
                   ? CreatePen(PS_SOLID, BorderWidth, Border)
                   : static_cast<HPEN>(GetStockObject(NULL_PEN));
    HBRUSH Brush = (Fill != CLR_INVALID)
                       ? CreateSolidBrush(Fill)
                       : static_cast<HBRUSH>(GetStockObject(NULL_BRUSH));

    HGDIOBJ OldPen = SelectObject(Dc, Pen);
    HGDIOBJ OldBrush = SelectObject(Dc, Brush);

    Rectangle(Dc, Left, Top, Right, Bottom);

    SelectObject(Dc, OldPen);
    SelectObject(Dc, OldBrush);

    if (Border != CLR_INVALID) DeleteObject(Pen);
    if (Fill != CLR_INVALID) DeleteObject(Brush);
}

void FrameRect(HDC Dc, int Left, int Top, int Right, int Bottom,
               COLORREF Border, int BorderWidth)
{
    HPEN Pen = CreatePen(PS_SOLID, BorderWidth, Border);
    HGDIOBJ OldPen = SelectObject(Dc, Pen);
    HGDIOBJ OldBrush = SelectObject(Dc, GetStockObject(NULL_BRUSH));

    Rectangle(Dc, Left, Top, Right, Bottom);

    SelectObject(Dc, OldPen);
    SelectObject(Dc, OldBrush);
    DeleteObject(Pen);
}

void FillEllipse(HDC Dc, int Left, int Top, int Right, int Bottom,
                 COLORREF Fill, COLORREF Border, int BorderWidth)
{
    HPEN Pen = (Border != CLR_INVALID)
                   ? CreatePen(PS_SOLID, BorderWidth, Border)
                   : static_cast<HPEN>(GetStockObject(NULL_PEN));
    HBRUSH Brush = (Fill != CLR_INVALID)
                       ? CreateSolidBrush(Fill)
                       : static_cast<HBRUSH>(GetStockObject(NULL_BRUSH));

    HGDIOBJ OldPen = SelectObject(Dc, Pen);
    HGDIOBJ OldBrush = SelectObject(Dc, Brush);

    Ellipse(Dc, Left, Top, Right, Bottom);

    SelectObject(Dc, OldPen);
    SelectObject(Dc, OldBrush);

    if (Border != CLR_INVALID) DeleteObject(Pen);
    if (Fill != CLR_INVALID) DeleteObject(Brush);
}

void FillRoundRect(HDC Dc, int Left, int Top, int Right, int Bottom,
                   int Radius, COLORREF Fill, COLORREF Border, int BorderWidth)
{
    HPEN Pen = (Border != CLR_INVALID)
                   ? CreatePen(PS_SOLID, BorderWidth, Border)
                   : static_cast<HPEN>(GetStockObject(NULL_PEN));
    HBRUSH Brush = (Fill != CLR_INVALID)
                       ? CreateSolidBrush(Fill)
                       : static_cast<HBRUSH>(GetStockObject(NULL_BRUSH));

    HGDIOBJ OldPen = SelectObject(Dc, Pen);
    HGDIOBJ OldBrush = SelectObject(Dc, Brush);

    RoundRect(Dc, Left, Top, Right, Bottom, Radius, Radius);

    SelectObject(Dc, OldPen);
    SelectObject(Dc, OldBrush);

    if (Border != CLR_INVALID) DeleteObject(Pen);
    if (Fill != CLR_INVALID) DeleteObject(Brush);
}

void FillPolygon(HDC Dc, const POINT* Points, int Count,
                 COLORREF Fill, COLORREF Border, int BorderWidth)
{
    HPEN Pen = (Border != CLR_INVALID)
                   ? CreatePen(PS_SOLID, BorderWidth, Border)
                   : static_cast<HPEN>(GetStockObject(NULL_PEN));
    HBRUSH Brush = (Fill != CLR_INVALID)
                       ? CreateSolidBrush(Fill)
                       : static_cast<HBRUSH>(GetStockObject(NULL_BRUSH));

    HGDIOBJ OldPen = SelectObject(Dc, Pen);
    HGDIOBJ OldBrush = SelectObject(Dc, Brush);

    Polygon(Dc, Points, Count);

    SelectObject(Dc, OldPen);
    SelectObject(Dc, OldBrush);

    if (Border != CLR_INVALID) DeleteObject(Pen);
    if (Fill != CLR_INVALID) DeleteObject(Brush);
}

void Line(HDC Dc, int X1, int Y1, int X2, int Y2,
          COLORREF Color, int Width)
{
    HPEN Pen = CreatePen(PS_SOLID, Width, Color);
    HGDIOBJ OldPen = SelectObject(Dc, Pen);

    MoveToEx(Dc, X1, Y1, nullptr);
    LineTo(Dc, X2, Y2);

    SelectObject(Dc, OldPen);
    DeleteObject(Pen);
}

void Text(HDC Dc, int X, int Y, const wchar_t* Str,
          int Size, COLORREF Color, bool Bold)
{
    if (Str == nullptr || *Str == L'\0') return;

    HFONT Font = CreateFontW(-Size, 0, 0, 0,
                             Bold ? FW_BOLD : FW_NORMAL,
                             FALSE, FALSE, FALSE,
                             DEFAULT_CHARSET,
                             OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                             ANTIALIASED_QUALITY,
                             DEFAULT_PITCH | FF_DONTCARE,
                             L"Consolas");
    HGDIOBJ OldFont = SelectObject(Dc, Font);

    int OldBkMode = SetBkMode(Dc, TRANSPARENT);
    COLORREF OldColor = SetTextColor(Dc, Color);

    TextOutW(Dc, X, Y, Str, static_cast<int>(wcslen(Str)));

    SetTextColor(Dc, OldColor);
    SetBkMode(Dc, OldBkMode);
    SelectObject(Dc, OldFont);
    DeleteObject(Font);
}

void TextShadow(HDC Dc, int X, int Y, const wchar_t* Str,
                int Size, COLORREF Color, COLORREF Shadow, bool Bold)
{
    Text(Dc, X + 2, Y + 2, Str, Size, Shadow, Bold);
    Text(Dc, X, Y, Str, Size, Color, Bold);
}

void CenteredText(HDC Dc, int CenterX, int Y, const wchar_t* Str,
                  int Size, COLORREF Color, bool Bold, COLORREF Shadow)
{
    int Width = TextWidth(Dc, Str, Size, Bold);
    int X = CenterX - Width / 2;

    if (Shadow != CLR_INVALID) {
        Text(Dc, X + 2, Y + 2, Str, Size, Shadow, Bold);
    }
    Text(Dc, X, Y, Str, Size, Color, Bold);
}

int TextWidth(HDC Dc, const wchar_t* Str, int Size, bool Bold)
{
    if (Str == nullptr || *Str == L'\0') return 0;

    HFONT Font = CreateFontW(-Size, 0, 0, 0,
                             Bold ? FW_BOLD : FW_NORMAL,
                             FALSE, FALSE, FALSE,
                             DEFAULT_CHARSET,
                             OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                             ANTIALIASED_QUALITY,
                             DEFAULT_PITCH | FF_DONTCARE,
                             L"Consolas");
    HGDIOBJ OldFont = SelectObject(Dc, Font);

    SIZE Extent = {0, 0};
    GetTextExtentPoint32W(Dc, Str, static_cast<int>(wcslen(Str)), &Extent);

    SelectObject(Dc, OldFont);
    DeleteObject(Font);

    return static_cast<int>(Extent.cx);
}

} // namespace Gfx

/*------------------------------------------------------------------*/
/*                     МЕТОДЫ КЛАССА LOCATION                       */
/*------------------------------------------------------------------*/
Location::Location(int InitX, int InitY)
{
    X = InitX;
    Y = InitY;
}

Location::~Location() = default;

int Location::GetX() const { return X; }
int Location::GetY() const { return Y; }

void Location::SetX(int NewX) { X = NewX; }
void Location::SetY(int NewY) { Y = NewY; }

void Location::SetPosition(int NewX, int NewY)
{
    X = NewX;
    Y = NewY;
}

/*------------------------------------------------------------------*/
/*                      МЕТОДЫ КЛАССА POINT                         */
/*------------------------------------------------------------------*/
Point::Point(int InitX, int InitY)
    : Location(InitX, InitY)
{
    Visible = false;
}

Point::~Point() = default;

bool Point::IsVisible() const { return Visible; }

void Point::Show()
{
    Visible = true;

    const COLORREF Color = RGB(230, 40, 40);
    Gfx::Line(hdc, X - 7, Y, X + 7, Y, Color, 2);
    Gfx::Line(hdc, X, Y - 7, X, Y + 7, Color, 2);
    Gfx::FillEllipse(hdc, X - 8, Y - 8, X + 8, Y + 8,
                     CLR_INVALID, Color, 2);
}

void Point::Hide()
{
    Visible = false;
    Gfx::FillRect(hdc, X - 10, Y - 10, X + 10, Y + 10, COLOR_SKY);
}

void Point::MoveTo(int NewX, int NewY)
{
    if (X == NewX && Y == NewY) return;

    Hide();
    SetPosition(NewX, NewY);
    Show();
}

void Point::MoveBy(int Dx, int Dy)
{
    MoveTo(X + Dx, Y + Dy);
}

void Point::Drag(int Step)
{
    int NewX = GetX();
    int NewY = GetY();

    while (!IsKeyDown(VK_ESCAPE)) {
        bool Moved = false;

        if (IsKeyDown(VK_LEFT))  { NewX -= Step; Moved = true; }
        if (IsKeyDown(VK_RIGHT)) { NewX += Step; Moved = true; }
        if (IsKeyDown(VK_UP))    { NewY -= Step; Moved = true; }
        if (IsKeyDown(VK_DOWN))  { NewY += Step; Moved = true; }

        if (Moved) {
            MoveTo(NewX, NewY);
            Sleep(20);
        }
        Sleep(10);
    }
}

/*------------------------------------------------------------------*/
/*                      МЕТОДЫ КЛАССА PLANE                         */
/*------------------------------------------------------------------*/
Plane::Plane(int InitX, int InitY, int NewGroundY)
    : Point(InitX, InitY)
{
    BodyWidth = 96;
    BodyHeight = 64;
    GroundY = NewGroundY;

    PreciseY = InitY;
    VelocityY = 0.0;
    Gravity = 0.55;
    FlapImpulse = -9.0;
    MaxFallSpeed = 11.0;
    PropPhase = 0.0;
    FlapFlash = 0;
    Crashed = false;
}

Plane::~Plane() = default;

void Plane::Reset(int InitX, int InitY, int NewGroundY)
{
    SetPosition(InitX, InitY);
    GroundY = NewGroundY;

    PreciseY = InitY;
    VelocityY = 0.0;
    PropPhase = 0.0;
    FlapFlash = 0;
    Crashed = false;
    Visible = false;
}

bool Plane::IsCrashed() const { return Crashed; }
int Plane::GetBodyWidth() const { return BodyWidth; }
int Plane::GetBodyHeight() const { return BodyHeight; }
double Plane::GetVelocityY() const { return VelocityY; }

void Plane::Animate()
{
    PropPhase += 0.55;
    if (FlapFlash > 0) FlapFlash--;
}

void Plane::Flap()
{
    if (Crashed) return;

    PreciseY = Y;
    VelocityY = FlapImpulse;
    FlapFlash = 9;
    PropPhase += 0.8;
}

void Plane::Crash()
{
    if (Crashed) return;

    Crashed = true;
    VelocityY = 0.0;
    Hide();
}

void Plane::Update()
{
    if (Crashed) return;

    Animate();

    VelocityY += Gravity;
    if (VelocityY > MaxFallSpeed) VelocityY = MaxFallSpeed;

    PreciseY += VelocityY;
    int NewY = static_cast<int>(std::floor(PreciseY + 0.5));

    // Потолок: выше окна не улетаем.
    if (NewY < 0) {
        NewY = 0;
        PreciseY = 0.0;
        VelocityY = 0.0;
    }

    // Касание земли — авария.
    if (NewY + BodyHeight >= GroundY) {
        NewY = GroundY - BodyHeight;
        SetPosition(GetX(), NewY);
        PreciseY = NewY;
        Crash();
        return;
    }

    MoveTo(GetX(), NewY);
}

HitBox Plane::GetHitBox() const
{
    // Немного уменьшаем хитбокс, чтобы игра прощала касания краями.
    HitBox Box;
    Box.Left = X + 16;
    Box.Top = Y + 14;
    Box.Right = X + BodyWidth - 14;
    Box.Bottom = Y + BodyHeight - 6;
    return Box;
}

bool Plane::IsActive() const { return !Crashed; }

void Plane::Show()
{
    if (Crashed) return;

    Visible = true;

    const int CenterY = Y + BodyHeight / 2;
    const int Tilt = ClampInt(static_cast<int>(VelocityY * 1.4), -11, 11);
    const int NoseY = CenterY + Tilt;
    const int NoseX = X + BodyWidth - 1;

    // Короткий след от прыжка.
    if (FlapFlash > 0) {
        int Len = FlapFlash * 3;
        Gfx::Line(hdc, X - 12 - Len, CenterY - 12, X - 4, CenterY - 12,
                  RGB(240, 250, 255), 2);
        Gfx::Line(hdc, X - 8 - Len, CenterY, X - 2, CenterY,
                  RGB(220, 240, 255), 2);
        Gfx::Line(hdc, X - 14 - Len, CenterY + 12, X - 6, CenterY + 12,
                  RGB(240, 250, 255), 2);
    }

    DrawWing(CenterY);
    DrawTail(CenterY);
    DrawFuselage(CenterY, NoseY);
    DrawPropeller(NoseX, NoseY);
}

void Plane::Hide()
{
    Visible = false;
    Gfx::FillRect(hdc, X - 18, Y - 16,
                  X + BodyWidth + 12, Y + BodyHeight + 10,
                  COLOR_SKY);
}

void Plane::DrawWing(int CenterY)
{
    // Дальнее крыло (уходит вверх-назад).
    POINT FarWing[4] = {
        {X + 34, CenterY - 2},
        {X + 46, CenterY - 18},
        {X + 66, CenterY - 14},
        {X + 60, CenterY - 2}
    };
    Gfx::FillPolygon(hdc, FarWing, 4,
                     COLOR_PLANE_WING2, COLOR_PLANE_EDGE, 2);

    // Ближнее крыло (вниз-назад).
    POINT Wing[4] = {
        {X + 28, CenterY + 3},
        {X + 42, CenterY + 26},
        {X + 70, CenterY + 26},
        {X + 64, CenterY + 3}
    };
    Gfx::FillPolygon(hdc, Wing, 4,
                     COLOR_PLANE_WING, COLOR_PLANE_EDGE, 2);

    // Красная полоса на законцовке крыла.
    Gfx::Line(hdc, X + 45, CenterY + 24, X + 69, CenterY + 24,
              COLOR_PLANE_RED, 3);
}

void Plane::DrawTail(int CenterY)
{
    // Горизонтальный стабилизатор.
    POINT HStab[4] = {
        {X + 2, CenterY + 2},
        {X + 24, CenterY},
        {X + 28, CenterY + 9},
        {X + 4, CenterY + 11}
    };
    Gfx::FillPolygon(hdc, HStab, 4,
                     COLOR_PLANE_RED, COLOR_PLANE_DARK, 2);

    // Вертикальный киль.
    POINT Fin[3] = {
        {X + 4, CenterY - 2},
        {X + 10, CenterY - 28},
        {X + 32, CenterY - 8}
    };
    Gfx::FillPolygon(hdc, Fin, 3,
                     COLOR_PLANE_RED, COLOR_PLANE_DARK, 2);
}

void Plane::DrawFuselage(int CenterY, int NoseY)
{
    // Фюзеляж.
    Gfx::FillRoundRect(hdc, X + 4, CenterY - 16,
                       X + BodyWidth - 18, CenterY + 16,
                       30, COLOR_PLANE_BODY, COLOR_PLANE_EDGE, 2);

    // Нос (треугольник, чуть поворачивается по скорости).
    POINT Nose[3] = {
        {X + BodyWidth - 36, CenterY - 16},
        {X + BodyWidth - 1, NoseY},
        {X + BodyWidth - 36, CenterY + 16}
    };
    Gfx::FillPolygon(hdc, Nose, 3,
                     COLOR_PLANE_BODY, COLOR_PLANE_EDGE, 2);

    // Красная линия по борту.
    Gfx::FillRect(hdc, X + 12, CenterY + 4,
                  X + BodyWidth - 36, CenterY + 9,
                  COLOR_PLANE_RED);

    // Кабина.
    Gfx::FillEllipse(hdc, X + BodyWidth - 52, CenterY - 11,
                     X + BodyWidth - 26, CenterY - 1,
                     COLOR_PLANE_GLASS, COLOR_PLANE_EDGE, 1);

    // Иллюминаторы.
    for (int Index = 0; Index < 3; Index++) {
        int Wx = X + 26 + Index * 13;
        Gfx::FillEllipse(hdc, Wx, CenterY - 6,
                         Wx + 9, CenterY + 3,
                         COLOR_PLANE_GLASS, COLOR_PLANE_EDGE, 1);
    }

    // Дверь.
    Gfx::FillRect(hdc, X + 17, CenterY - 11,
                  X + 21, CenterY + 10,
                  RGB(214, 222, 234));

    // Шасси.
    Gfx::Line(hdc, X + 46, CenterY + 15,
              X + 46, CenterY + 25, COLOR_PLANE_EDGE, 3);
    Gfx::FillEllipse(hdc, X + 40, CenterY + 23,
                     X + 52, CenterY + 35,
                     RGB(38, 42, 52), COLOR_PLANE_EDGE, 1);
}

void Plane::DrawPropeller(int NoseX, int NoseY)
{
    int PropLen = 5 + static_cast<int>(20.0 * std::fabs(std::cos(PropPhase)));

    Gfx::FillEllipse(hdc, NoseX - 3, NoseY - PropLen,
                     NoseX + 3, NoseY + PropLen,
                     RGB(226, 232, 242), RGB(90, 100, 120), 1);

    Gfx::FillEllipse(hdc, NoseX - 5, NoseY - 5,
                     NoseX + 5, NoseY + 5,
                     RGB(45, 55, 75), COLOR_PLANE_EDGE, 1);
}

/*------------------------------------------------------------------*/
/*                    МЕТОДЫ КЛАССА SKYSCRAPER                      */
/*------------------------------------------------------------------*/
SkyScraper::SkyScraper(int InitX, int InitGroundY,
                       int InitWidth, int InitHeight)
    : Point(InitX, InitGroundY - InitHeight)
{
    GroundY = InitGroundY;
    BodyWidth = InitWidth;
    BodyHeight = InitHeight;

    if (BodyWidth < 50) BodyWidth = 50;
    if (BodyHeight < 60) BodyHeight = 60;
    if (BodyHeight > GroundY - 60) BodyHeight = GroundY - 60;
    if (BodyHeight < 60) BodyHeight = 60;

    Y = GroundY - BodyHeight;

    AnimTick = 0;
    WindowSeed = 5;
    Scored = false;
}

SkyScraper::~SkyScraper() = default;

int SkyScraper::GetWidth() const { return BodyWidth; }
int SkyScraper::GetHeight() const { return BodyHeight; }
int SkyScraper::GetRight() const { return X + BodyWidth; }
bool SkyScraper::IsScored() const { return Scored; }
void SkyScraper::MarkScored() { Scored = true; }

bool SkyScraper::IsOffScreen() const
{
    return X + BodyWidth < -20;
}

void SkyScraper::AdvanceAnimation()
{
    AnimTick++;
}

void SkyScraper::Scroll(int Dx)
{
    MoveTo(X + Dx, Y);
}

void SkyScraper::Recycle(int NewX, int NewHeight)
{
    BodyHeight = NewHeight;
    if (BodyHeight < 60) BodyHeight = 60;
    if (BodyHeight > GroundY - 60) BodyHeight = GroundY - 60;
    if (BodyHeight < 60) BodyHeight = 60;

    X = NewX;
    Y = GroundY - BodyHeight;
    Scored = false;
    Visible = true;
}

HitBox SkyScraper::GetHitBox() const
{
    HitBox Box;
    Box.Left = X;
    Box.Top = Y;
    Box.Right = X + BodyWidth;
    Box.Bottom = GroundY;
    return Box;
}

COLORREF SkyScraper::GetBodyColor() const { return COLOR_TOWER_BODY; }
COLORREF SkyScraper::GetShadowColor() const { return COLOR_TOWER_SHADOW; }
COLORREF SkyScraper::GetRoofColor() const { return COLOR_TOWER_ROOF; }
COLORREF SkyScraper::GetWindowColor() const { return COLOR_TOWER_WINDOW; }
COLORREF SkyScraper::GetLitWindowColor() const { return COLOR_TOWER_LIT; }
COLORREF SkyScraper::GetOutlineColor() const { return COLOR_TOWER_EDGE; }

void SkyScraper::Show()
{
    Visible = true;

    const int Left = X;
    const int Right = X + BodyWidth;
    const int Top = Y;
    const int Bottom = GroundY;

    // Тень сбоку.
    Gfx::FillRect(hdc, Right - 8, Top + 4,
                  Right + 2, Bottom,
                  GetShadowColor());

    // Корпус.
    Gfx::FillRect(hdc, Left, Top,
                  Right, Bottom,
                  GetBodyColor(), GetOutlineColor(), 2);

    // Затемнённая правая грань.
    Gfx::FillRect(hdc, Right - 12, Top + 4,
                  Right - 2, Bottom - 2,
                  GetShadowColor());

    // Окна.
    DrawWindows();

    // Вход.
    const int DoorW = 26;
    const int DoorH = 36;
    const int DoorX = Left + BodyWidth / 2 - DoorW / 2;
    Gfx::FillRect(hdc, DoorX, Bottom - DoorH,
                  DoorX + DoorW, Bottom,
                  RGB(44, 49, 60), GetOutlineColor(), 2);
    Gfx::FillRect(hdc, DoorX + DoorW / 2 - 2, Bottom - DoorH + 8,
                  DoorX + DoorW / 2 + 2, Bottom - 10,
                  RGB(190, 200, 210));

    // Крыша, антенна, мелкие детали.
    DrawRoof();
    DrawExtra();
}

void SkyScraper::Hide()
{
    Visible = false;

    // С запасом: антенна уходит выше крыши, тень — правее корпуса.
    Gfx::FillRect(hdc, X - 12, Y - 70,
                  X + BodyWidth + 12, GroundY,
                  COLOR_SKY);
}

void SkyScraper::DrawRoof()
{
    const int Left = X;
    const int Right = X + BodyWidth;
    const int Top = Y;

    // Козырёк крыши.
    Gfx::FillRect(hdc, Left - 4, Top - 8,
                  Right + 4, Top + 2,
                  GetRoofColor(), GetOutlineColor(), 2);

    // Вентиляция.
    Gfx::FillRect(hdc, Left + 10, Top - 18,
                  Left + 34, Top - 6,
                  GetShadowColor(), GetOutlineColor(), 1);
    Gfx::FillRect(hdc, Left + 42, Top - 14,
                  Left + 60, Top - 6,
                  GetShadowColor(), GetOutlineColor(), 1);

    // Антенна с красным огоньком.
    const int AntennaX = Left + BodyWidth - 18;
    Gfx::Line(hdc, AntennaX, Top - 8,
              AntennaX, Top - 48,
              GetOutlineColor(), 2);
    Gfx::FillEllipse(hdc, AntennaX - 3, Top - 54,
                     AntennaX + 3, Top - 48,
                     RGB(230, 60, 60), GetOutlineColor(), 1);
}

void SkyScraper::DrawWindows()
{
    const int CellW = 18;
    const int CellH = 22;
    const int WinW = 10;
    const int WinH = 13;

    int Columns = (BodyWidth - 24) / CellW;
    if (Columns < 1) Columns = 1;

    const int GridW = Columns * CellW - (CellW - WinW);
    const int StartX = X + (BodyWidth - GridW) / 2;
    const int StartY = Y + 24;
    const int BottomLimit = GroundY - 40;

    for (int Row = 0; ; Row++) {
        int Wy = StartY + Row * CellH;
        if (Wy + WinH > BottomLimit) break;

        for (int Col = 0; Col < Columns; Col++) {
            int Wx = StartX + Col * CellW + (CellW - WinW) / 2;

            bool Lit = ((Row * 7 + Col * 13 + WindowSeed) % 17 == 0);
            Gfx::FillRect(hdc, Wx, Wy,
                          Wx + WinW, Wy + WinH,
                          Lit ? GetLitWindowColor() : GetWindowColor());
        }
    }
}

void SkyScraper::DrawExtra()
{
    // Базовый небоскрёб не имеет дополнительных деталей.
}

/*------------------------------------------------------------------*/
/*                     МЕТОДЫ КЛАССА GLASSTOWER                     */
/*------------------------------------------------------------------*/
GlassTower::GlassTower(int InitX, int InitGroundY,
                       int InitWidth, int InitHeight)
    : SkyScraper(InitX, InitGroundY, InitWidth, InitHeight)
{
    WindowSeed = 11;
}

GlassTower::~GlassTower() = default;

COLORREF GlassTower::GetBodyColor() const { return RGB(138, 170, 196); }
COLORREF GlassTower::GetShadowColor() const { return RGB(108, 140, 168); }
COLORREF GlassTower::GetRoofColor() const { return RGB(96, 120, 145); }
COLORREF GlassTower::GetWindowColor() const { return RGB(82, 128, 165); }
COLORREF GlassTower::GetLitWindowColor() const { return RGB(228, 242, 252); }
COLORREF GlassTower::GetOutlineColor() const { return RGB(62, 82, 104); }

void GlassTower::DrawRoof()
{
    SkyScraper::DrawRoof();

    const int CenterX = X + BodyWidth / 2;
    Gfx::Line(hdc, CenterX, Y - 6,
              CenterX, Y - 60,
              RGB(76, 96, 116), 3);
    Gfx::FillEllipse(hdc, CenterX - 5, Y - 66,
                     CenterX + 5, Y - 56,
                     RGB(230, 90, 90), RGB(90, 40, 40), 1);
}

void GlassTower::DrawWindows()
{
    const int Left = X + 10;
    const int Right = X + BodyWidth - 12;
    const int Top = Y + 18;
    const int Bottom = GroundY - 38;

    for (int PanelX = Left; PanelX + 12 <= Right; PanelX += 16) {
        Gfx::FillRect(hdc, PanelX, Top,
                      PanelX + 12, Bottom,
                      GetWindowColor(), RGB(70, 95, 120), 1);

        for (int Wy = Top + 4; Wy + 8 < Bottom; Wy += 16) {
            bool Lit = ((PanelX * 3 + Wy * 5 + WindowSeed) % 9 == 0);
            COLORREF Cell = Lit ? GetLitWindowColor() : RGB(104, 150, 184);
            Gfx::FillRect(hdc, PanelX + 2, Wy,
                          PanelX + 10, Wy + 8,
                          Cell);
        }
    }
}

/*------------------------------------------------------------------*/
/*                      МЕТОДЫ КЛАССА NEONTOWER                     */
/*------------------------------------------------------------------*/
NeonTower::NeonTower(int InitX, int InitGroundY,
                     int InitWidth, int InitHeight)
    : SkyScraper(InitX, InitGroundY, InitWidth, InitHeight)
{
    WindowSeed = 2;
}

NeonTower::~NeonTower() = default;

COLORREF NeonTower::GetBodyColor() const { return RGB(100, 100, 112); }
COLORREF NeonTower::GetShadowColor() const { return RGB(78, 78, 90); }
COLORREF NeonTower::GetRoofColor() const { return RGB(70, 70, 82); }
COLORREF NeonTower::GetWindowColor() const { return RGB(55, 60, 75); }
COLORREF NeonTower::GetLitWindowColor() const { return RGB(255, 140, 80); }
COLORREF NeonTower::GetOutlineColor() const { return RGB(44, 44, 54); }

void NeonTower::DrawRoof()
{
    SkyScraper::DrawRoof();

    const int AntennaX = X + 20;
    Gfx::Line(hdc, AntennaX, Y - 8,
              AntennaX, Y - 54,
              GetOutlineColor(), 2);

    COLORREF Light = ((AnimTick / 12) % 2 == 0)
                         ? RGB(255, 80, 90)
                         : RGB(120, 35, 45);
    Gfx::FillEllipse(hdc, AntennaX - 4, Y - 60,
                     AntennaX + 4, Y - 52,
                     Light, GetOutlineColor(), 1);
}

void NeonTower::DrawExtra()
{
    const int SignX = X + BodyWidth - 42;
    const int SignY = Y + 48;

    COLORREF Neon = ((AnimTick / 15) % 2 == 0)
                        ? RGB(255, 80, 150)
                        : RGB(170, 45, 110);

    // Корпус вывески.
    Gfx::FillRect(hdc, SignX - 4, SignY - 4,
                  SignX + 38, SignY + 96,
                  RGB(30, 30, 40), Neon, 2);

    // Неоновые полосы.
    for (int Index = 0; Index < 4; Index++) {
        int LineY = SignY + 8 + Index * 21;
        Gfx::FillRect(hdc, SignX + 3, LineY,
                      SignX + 29, LineY + 8,
                      Neon);
    }
}

/*------------------------------------------------------------------*/
/*                    ИГРОВАЯ СЦЕНА (Game)                          */
/*------------------------------------------------------------------*/

/*                      КОНСТРУКТОР / ДЕСТРУКТОР                    */
/*------------------------------------------------------------------*/
Game::Game(int W, int H)
    : Width(W),
      Height(H),
      GroundY(H - 72),
      Player(W * 27 / 100, H - 72 - 300, H - 72),
      State(GameState::Ready),
      Score(0),
      BestScore(0),
      CrashTimer(0),
      CrashX(-1),
      CrashY(-1),
      ScrollSpeed(4),
      BobPhase(0.0),
      ShowHitBoxes(false),
      Rng(2025)
{
    Towers[0] = new SkyScraper(0, GroundY, 100, 280);
    Towers[1] = new GlassTower(0, GroundY, 86, 300);
    Towers[2] = new NeonTower(0, GroundY, 110, 260);
    Towers[3] = new GlassTower(0, GroundY, 86, 320);

    Reset();
}

Game::~Game()
{
    for (int Index = 0; Index < TowerCount; Index++) {
        delete Towers[Index];
        Towers[Index] = nullptr;
    }
}

/*------------------------------------------------------------------*/
/*                           СБРОС ИГРЫ                             */
/*------------------------------------------------------------------*/
void Game::Reset()
{
    State = GameState::Ready;
    Score = 0;
    CrashTimer = 0;
    CrashX = -1;
    CrashY = -1;
    ScrollSpeed = 4;
    BobPhase = 0.0;

    const int StartX = Width * 27 / 100;
    const int StartY = GroundY - 300;
    Player.Reset(StartX, StartY, GroundY);

    // Первые дома ставим за правым краем экрана.
    int TowerX = Width + 80;
    for (int Index = 0; Index < TowerCount; Index++) {
        Towers[Index]->Recycle(TowerX, RandomHeight());
        TowerX += Towers[Index]->GetWidth() + RandomGap();
    }

    // Облака.
    int MaxCloudY = GroundY - 320;
    if (MaxCloudY < 140) MaxCloudY = 140;

    for (int Index = 0; Index < CloudCount; Index++) {
        Clouds[Index].X =
            static_cast<double>(Rng() % static_cast<unsigned>(Width + 200)) - 100.0;
        Clouds[Index].Y =
            60.0 + static_cast<double>(Rng() %
                   static_cast<unsigned>(MaxCloudY - 60 + 1));
        Clouds[Index].Scale =
            0.60 + static_cast<double>(Rng() % 80) / 100.0;
        Clouds[Index].Speed =
            0.25 + static_cast<double>(Rng() % 50) / 100.0;
    }
}

/*------------------------------------------------------------------*/
/*                         ИГРОВОЙ ЦИКЛ                             */
/*------------------------------------------------------------------*/
void Game::Update()
{
    switch (State) {
    case GameState::Ready:
        BobPhase += 0.08;
        Player.SetPosition(Width * 27 / 100,
                           GroundY - 300 +
                           static_cast<int>(std::sin(BobPhase) * 9.0));
        Player.Animate();
        UpdateClouds();
        break;

    case GameState::Playing:
        UpdateClouds();

        Player.Update();

        for (int Index = 0; Index < TowerCount; Index++) {
            Towers[Index]->Scroll(-ScrollSpeed);
            Towers[Index]->AdvanceAnimation();
        }

        CheckScore();

        if (!Player.IsCrashed()) {
            CheckCollisions();
        }

        if (Player.IsCrashed()) {
            State = GameState::Crashed;
            CrashTimer = 0;

            // Если самолёт врезался в землю, точку взрыва задаём здесь.
            if (CrashX < 0) {
                CrashX = Player.GetX() + Player.GetBodyWidth() / 2;
                CrashY = Player.GetY() + Player.GetBodyHeight();
            }
        } else {
            RecycleTowers();
        }
        break;

    case GameState::Crashed:
        UpdateClouds();
        for (int Index = 0; Index < TowerCount; Index++) {
            Towers[Index]->AdvanceAnimation();
        }

        CrashTimer++;
        if (CrashTimer > 48) {
            State = GameState::GameOver;
        }
        break;

    case GameState::GameOver:
        UpdateClouds();
        break;
    }
}

void Game::PressFlap()
{
    if (State == GameState::Crashed) return;

    if (State == GameState::GameOver) {
        Reset();
        State = GameState::Playing;
        Player.Flap();
        return;
    }

    if (State == GameState::Ready) {
        State = GameState::Playing;
    }

    Player.Flap();
}

void Game::ToggleHitBoxes()
{
    ShowHitBoxes = !ShowHitBoxes;
}

GameState Game::GetState() const { return State; }
int Game::GetScore() const { return Score; }
int Game::GetBestScore() const { return BestScore; }

/*------------------------------------------------------------------*/
/*                     ВСПОМОГАТЕЛЬНЫЕ МЕТОДЫ                       */
/*------------------------------------------------------------------*/
void Game::UpdateClouds()
{
    int MaxCloudY = GroundY - 320;
    if (MaxCloudY < 140) MaxCloudY = 140;

    for (int Index = 0; Index < CloudCount; Index++) {
        Clouds[Index].X -= Clouds[Index].Speed;

        if (Clouds[Index].X + 180.0 * Clouds[Index].Scale < 0.0) {
            Clouds[Index].X = static_cast<double>(Width) + 60.0;
            Clouds[Index].Y =
                60.0 + static_cast<double>(Rng() %
                       static_cast<unsigned>(MaxCloudY - 60 + 1));
            Clouds[Index].Scale =
                0.60 + static_cast<double>(Rng() % 80) / 100.0;
            Clouds[Index].Speed =
                0.25 + static_cast<double>(Rng() % 50) / 100.0;
        }
    }
}

int Game::RandomHeight()
{
    int MaxHeight = GroundY - 190;
    if (MaxHeight > 470) MaxHeight = 470;
    if (MaxHeight < 120) MaxHeight = 120;

    std::uniform_int_distribution<int> Dist(120, MaxHeight);
    return Dist(Rng);
}

int Game::RandomGap()
{
    std::uniform_int_distribution<int> Dist(250, 340);
    return Dist(Rng);
}

void Game::RecycleTowers()
{
    int Rightmost = -100000;

    for (int Index = 0; Index < TowerCount; Index++) {
        if (Towers[Index]->GetRight() > Rightmost) {
            Rightmost = Towers[Index]->GetRight();
        }
    }

    for (int Index = 0; Index < TowerCount; Index++) {
        if (Towers[Index]->IsOffScreen()) {
            Towers[Index]->Recycle(Rightmost + RandomGap(), RandomHeight());
            Rightmost = Towers[Index]->GetRight();
        }
    }
}

void Game::CheckScore()
{
    const int PlaneLeft = Player.GetHitBox().Left;

    for (int Index = 0; Index < TowerCount; Index++) {
        if (!Towers[Index]->IsScored() &&
            Towers[Index]->GetRight() < PlaneLeft) {
            Towers[Index]->MarkScored();
            Score++;

            if (Score > BestScore) BestScore = Score;

            ScrollSpeed = 4 + Score / 6;
            if (ScrollSpeed > 7) ScrollSpeed = 7;
        }
    }
}

void Game::CheckCollisions()
{
    HitBox PlaneBox = Player.GetHitBox();

    for (int Index = 0; Index < TowerCount; Index++) {
        HitBox TowerBox = Towers[Index]->GetHitBox();

        if (!Intersects(PlaneBox, TowerBox)) continue;

        // Центр пересечения — там и рисуем взрыв.
        int Left = PlaneBox.Left > TowerBox.Left
                       ? PlaneBox.Left : TowerBox.Left;
        int Right = PlaneBox.Right < TowerBox.Right
                        ? PlaneBox.Right : TowerBox.Right;
        int Top = PlaneBox.Top > TowerBox.Top
                      ? PlaneBox.Top : TowerBox.Top;
        int Bottom = PlaneBox.Bottom < TowerBox.Bottom
                         ? PlaneBox.Bottom : TowerBox.Bottom;

        CrashX = (Left + Right) / 2;
        CrashY = (Top + Bottom) / 2;

        Player.Crash();
        State = GameState::Crashed;
        CrashTimer = 0;
        return;
    }
}

/*------------------------------------------------------------------*/
/*                          ОТРИСОВКА                               */
/*------------------------------------------------------------------*/
void Game::Render(HDC Target)
{
    hdc = Target;

    Gfx::FillRect(Target, 0, 0, Width, Height, COLOR_SKY);

    DrawSky();
    DrawClouds();
    DrawGround();

    for (int Index = 0; Index < TowerCount; Index++) {
        Towers[Index]->Show();
    }

    Player.Show();

    DrawHud();

    if (ShowHitBoxes) {
        DrawHitBoxes();
    }

    if (State == GameState::Crashed) {
        DrawExplosion();
    }

    if (State == GameState::Ready) {
        DrawReadyOverlay();
    }

    if (State == GameState::GameOver) {
        DrawGameOverOverlay();
    }
}

void Game::DrawSky()
{
    const int Bands = 24;

    for (int Index = 0; Index < Bands; Index++) {
        int Y0 = GroundY * Index / Bands;
        int Y1 = GroundY * (Index + 1) / Bands + 1;
        int T = Index * 255 / (Bands - 1);

        COLORREF Color = RGB(110 + T * 72 / 255,
                             180 + T * 44 / 255,
                             228 + T * 16 / 255);
        Gfx::FillRect(hdc, 0, Y0, Width, Y1, Color);
    }

    // Солнце.
    const int SunX = Width - 140;
    const int SunY = 105;

    Gfx::FillEllipse(hdc, SunX - 76, SunY - 76,
                     SunX + 76, SunY + 76, RGB(255, 247, 208));
    Gfx::FillEllipse(hdc, SunX - 52, SunY - 52,
                     SunX + 52, SunY + 52, RGB(255, 228, 96));
    Gfx::FillEllipse(hdc, SunX - 42, SunY - 42,
                     SunX + 42, SunY + 42, RGB(255, 239, 140));
}

void Game::DrawClouds()
{
    for (int Index = 0; Index < CloudCount; Index++) {
        int X = static_cast<int>(Clouds[Index].X);
        int Y = static_cast<int>(Clouds[Index].Y);
        int W = static_cast<int>(110.0 * Clouds[Index].Scale);
        int H = static_cast<int>(36.0 * Clouds[Index].Scale);

        if (W < 30) W = 30;
        if (H < 12) H = 12;

        Gfx::FillEllipse(hdc, X, Y,
                         X + W, Y + H, RGB(236, 246, 253));
        Gfx::FillEllipse(hdc, X + W / 5, Y - H / 3,
                         X + W * 4 / 5, Y + H * 2 / 3, RGB(248, 252, 255));
        Gfx::FillEllipse(hdc, X + W / 2, Y - H / 5,
                         X + W + W / 4, Y + H, RGB(248, 252, 255));
        Gfx::FillEllipse(hdc, X - W / 6, Y + H / 4,
                         X + W / 4, Y + H + H / 3, RGB(242, 249, 255));
    }
}

void Game::DrawGround()
{
    Gfx::FillRect(hdc, 0, GroundY, Width, Height, COLOR_GROUND);
    Gfx::FillRect(hdc, 0, GroundY, Width, GroundY + 8, COLOR_GROUND_TOP);
    Gfx::FillRect(hdc, 0, GroundY + 8, Width, GroundY + 12,
                  RGB(78, 82, 94));

    // Разметка.
    for (int X = 0; X < Width; X += 72) {
        Gfx::FillRect(hdc, X, GroundY + 34,
                      X + 38, GroundY + 40, COLOR_GROUND_LINE);
    }

    // Швы покрытия.
    for (int X = 0; X < Width; X += 144) {
        Gfx::Line(hdc, X, GroundY + 12, X, GroundY + 64,
                  RGB(48, 50, 58), 1);
    }
}

void Game::DrawHud()
{
    wchar_t Buffer[64];

    std::swprintf(Buffer, 64, L"SCORE  %d", Score);
    Gfx::TextShadow(hdc, 26, 16, Buffer, 38,
                    RGB(255, 255, 255), RGB(20, 50, 80), true);

    std::swprintf(Buffer, 64, L"BEST  %d", BestScore);
    int BestWidth = Gfx::TextWidth(hdc, Buffer, 24, true);
    Gfx::TextShadow(hdc, Width - BestWidth - 26, 24, Buffer, 24,
                    RGB(255, 255, 255), RGB(20, 50, 80), true);

    Gfx::Text(hdc, 24, Height - 26, L"F1 - HITBOXES    ESC - EXIT",
              14, RGB(210, 220, 230));
}

void Game::DrawExplosion()
{
    int Radius = 22 + CrashTimer * 3;
    if (Radius > 170) Radius = 170;

    // Лучи.
    for (int Index = 0; Index < 12; Index++) {
        double Angle = Index * 2.0 * PI / 12.0 + CrashTimer * 0.07;
        int X2 = CrashX + static_cast<int>(std::cos(Angle) * Radius * 1.3);
        int Y2 = CrashY + static_cast<int>(std::sin(Angle) * Radius * 1.3);

        Gfx::Line(hdc, CrashX, CrashY, X2, Y2,
                  (Index % 2 == 0) ? RGB(255, 190, 60) : RGB(255, 90, 30),
                  3);
    }

    Gfx::FillEllipse(hdc, CrashX - Radius, CrashY - Radius,
                     CrashX + Radius, CrashY + Radius, RGB(205, 55, 20));
    Gfx::FillEllipse(hdc, CrashX - Radius * 3 / 4, CrashY - Radius * 3 / 4,
                     CrashX + Radius * 3 / 4, CrashY + Radius * 3 / 4,
                     RGB(255, 125, 30));
    Gfx::FillEllipse(hdc, CrashX - Radius / 2, CrashY - Radius / 2,
                     CrashX + Radius / 2, CrashY + Radius / 2,
                     RGB(255, 220, 90));
    Gfx::FillEllipse(hdc, CrashX - Radius / 4, CrashY - Radius / 4,
                     CrashX + Radius / 4, CrashY + Radius / 4,
                     RGB(255, 255, 235));
}

void Game::DrawReadyOverlay()
{
    const int PanelW = 660;
    const int PanelH = 350;
    const int PanelX = (Width - PanelW) / 2;
    int PanelY = (GroundY - PanelH) / 2;
    if (PanelY < 8) PanelY = 8;

    Gfx::FillRoundRect(hdc, PanelX, PanelY,
                       PanelX + PanelW, PanelY + PanelH,
                       28, RGB(18, 42, 70), RGB(255, 255, 255), 3);
    Gfx::FillRoundRect(hdc, PanelX + 7, PanelY + 7,
                       PanelX + PanelW - 7, PanelY + PanelH - 7,
                       23, CLR_INVALID, RGB(90, 130, 165), 1);

    Gfx::CenteredText(hdc, Width / 2, PanelY + 30,
                      L"FLAPPY PLANE", 56,
                      RGB(255, 226, 100), true, RGB(0, 0, 0));
    Gfx::CenteredText(hdc, Width / 2, PanelY + 102,
                      L"OOP LAB  -  LOCATION -> POINT -> PLANE", 18,
                      RGB(175, 215, 245), false, RGB(0, 0, 0));

    Gfx::Line(hdc, PanelX + 42, PanelY + 138,
              PanelX + PanelW - 42, PanelY + 138,
              RGB(90, 130, 165), 1);

    Gfx::CenteredText(hdc, Width / 2, PanelY + 158,
                      L"SPACE / UP / W   -   FLAP", 24,
                      RGB(255, 255, 255), true, RGB(0, 0, 0));
    Gfx::CenteredText(hdc, Width / 2, PanelY + 202,
                      L"F1 - HITBOXES        ESC - EXIT", 17,
                      RGB(185, 215, 240), false, RGB(0, 0, 0));

    if ((GetTickCount() / 450) % 2 == 0) {
        Gfx::CenteredText(hdc, Width / 2, PanelY + 262,
                          L"PRESS SPACE TO START", 26,
                          RGB(255, 214, 90), true, RGB(0, 0, 0));
    }
}

void Game::DrawGameOverOverlay()
{
    const int PanelW = 580;
    const int PanelH = 320;
    const int PanelX = (Width - PanelW) / 2;
    int PanelY = (GroundY - PanelH) / 2;
    if (PanelY < 8) PanelY = 8;

    Gfx::FillRoundRect(hdc, PanelX, PanelY,
                       PanelX + PanelW, PanelY + PanelH,
                       28, RGB(20, 32, 52), RGB(255, 255, 255), 3);
    Gfx::FillRoundRect(hdc, PanelX + 7, PanelY + 7,
                       PanelX + PanelW - 7, PanelY + PanelH - 7,
                       23, CLR_INVALID, RGB(120, 90, 90), 1);

    Gfx::CenteredText(hdc, Width / 2, PanelY + 28,
                      L"GAME OVER", 54,
                      RGB(255, 112, 90), true, RGB(0, 0, 0));

    Gfx::Line(hdc, PanelX + 42, PanelY + 104,
              PanelX + PanelW - 42, PanelY + 104,
              RGB(120, 90, 90), 1);

    wchar_t Buffer[64];

    std::swprintf(Buffer, 64, L"SCORE   %d", Score);
    Gfx::CenteredText(hdc, Width / 2, PanelY + 126,
                      Buffer, 36, RGB(255, 255, 255),
                      true, RGB(0, 0, 0));

    std::swprintf(Buffer, 64, L"BEST   %d", BestScore);
    Gfx::CenteredText(hdc, Width / 2, PanelY + 176,
                      Buffer, 22, RGB(185, 215, 240),
                      false, RGB(0, 0, 0));

    Gfx::CenteredText(hdc, Width / 2, PanelY + 222,
                      L"SPACE - FLY AGAIN", 24,
                      RGB(255, 214, 90), true, RGB(0, 0, 0));
    Gfx::CenteredText(hdc, Width / 2, PanelY + 268,
                      L"ESC - EXIT", 18,
                      RGB(185, 215, 240), false, RGB(0, 0, 0));
}

void Game::DrawHitBoxes()
{
    HitBox PlayerBox = Player.GetHitBox();
    Gfx::FrameRect(hdc, PlayerBox.Left, PlayerBox.Top,
                   PlayerBox.Right, PlayerBox.Bottom,
                   RGB(255, 40, 40), 2);

    for (int Index = 0; Index < TowerCount; Index++) {
        HitBox TowerBox = Towers[Index]->GetHitBox();
        Gfx::FrameRect(hdc, TowerBox.Left, TowerBox.Top,
                       TowerBox.Right, TowerBox.Bottom,
                       RGB(255, 160, 40), 2);
    }
}
