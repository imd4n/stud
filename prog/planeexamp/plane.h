#pragma once

/********************************************************************
 *          ЛАБОРАТОРНАЯ РАБОТА ПО ООП                               *
 *------------------------------------------------------------------*
 * Проект        : Flappy Plane (вариант "Самолёт")                 *
 * Файл          : plane.h                                          *
 * Комментарий   : Иерархия классов Location -> Point -> Plane,     *
 *                 конкурирующий объект SkyScraper и игровая сцена. *
 *******************************************************************/

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <random>

/*------------------------------------------------------------------*/
/*                      ИНТЕРФЕЙСЫ И ХИТБОКС                        */
/*------------------------------------------------------------------*/
// Хитбокс хранится в экранных координатах. Right/Bottom не включаются,
// как принято в WinAPI (Rectangle рисует до правой/нижней границы).
struct HitBox {
    int Left;
    int Top;
    int Right;
    int Bottom;
};

// Проверка пересечения двух прямоугольников (AABB).
inline bool Intersects(const HitBox& A, const HitBox& B)
{
    return A.Left < B.Right && A.Right > B.Left &&
           A.Top < B.Bottom && A.Bottom > B.Top;
}

// Объект, который умеет показывать и прятать себя.
class IDisplayable {
public:
    virtual ~IDisplayable() = default;
    virtual void Show() = 0;
    virtual void Hide() = 0;
};

// Объект, который умеет перемещаться.
class IMovable {
public:
    virtual ~IMovable() = default;
    virtual void MoveTo(int NewX, int NewY) = 0;
    virtual void MoveBy(int Dx, int Dy) = 0;
};

// Объект, с которым можно столкнуться.
class ICollidable {
public:
    virtual ~ICollidable() = default;
    virtual HitBox GetHitBox() const = 0;
    virtual bool IsActive() const = 0;
};

/*------------------------------------------------------------------*/
/*                         ПАЛИТРА СЦЕНЫ                            */
/*------------------------------------------------------------------*/
const COLORREF COLOR_SKY          = RGB(126, 192, 232);
const COLORREF COLOR_SKY_TOP      = RGB(110, 180, 228);
const COLORREF COLOR_SKY_BOTTOM   = RGB(190, 226, 246);
const COLORREF COLOR_GROUND       = RGB(56, 60, 68);
const COLORREF COLOR_GROUND_TOP   = RGB(98, 103, 115);
const COLORREF COLOR_GROUND_LINE  = RGB(220, 185, 70);

const COLORREF COLOR_PLANE_BODY   = RGB(246, 249, 253);
const COLORREF COLOR_PLANE_EDGE   = RGB(28, 44, 76);
const COLORREF COLOR_PLANE_RED    = RGB(220, 62, 62);
const COLORREF COLOR_PLANE_DARK   = RGB(150, 28, 34);
const COLORREF COLOR_PLANE_GLASS  = RGB(36, 88, 138);
const COLORREF COLOR_PLANE_WING   = RGB(206, 214, 226);
const COLORREF COLOR_PLANE_WING2  = RGB(178, 188, 204);

const COLORREF COLOR_TOWER_BODY   = RGB(150, 152, 158);
const COLORREF COLOR_TOWER_SHADOW = RGB(120, 122, 130);
const COLORREF COLOR_TOWER_ROOF   = RGB(105, 108, 116);
const COLORREF COLOR_TOWER_WINDOW = RGB(55, 75, 95);
const COLORREF COLOR_TOWER_LIT    = RGB(255, 214, 120);
const COLORREF COLOR_TOWER_EDGE   = RGB(60, 62, 70);

/*------------------------------------------------------------------*/
/*                    ГЛОБАЛЬНЫЙ КОНТЕКСТ УСТРОЙСТВА                */
/*------------------------------------------------------------------*/
// Определяется в planeMain.cpp. Все объекты рисуют через него.
extern HDC hdc;

/*------------------------------------------------------------------*/
/*                    МИНИ-БИБЛИОТЕКА РИСОВАНИЯ                     */
/*------------------------------------------------------------------*/
// Обёртки над GDI: сами создают/выбирают/удаляют перья и кисти,
// поэтому в коде объектов не нужно каждый раз помнить про DeleteObject.
namespace Gfx {

void FillRect(HDC Dc, int Left, int Top, int Right, int Bottom,
              COLORREF Fill, COLORREF Border = CLR_INVALID, int BorderWidth = 1);

void FrameRect(HDC Dc, int Left, int Top, int Right, int Bottom,
               COLORREF Border, int BorderWidth = 1);

void FillEllipse(HDC Dc, int Left, int Top, int Right, int Bottom,
                 COLORREF Fill, COLORREF Border = CLR_INVALID, int BorderWidth = 1);

void FillRoundRect(HDC Dc, int Left, int Top, int Right, int Bottom,
                   int Radius, COLORREF Fill,
                   COLORREF Border = CLR_INVALID, int BorderWidth = 1);

void FillPolygon(HDC Dc, const POINT* Points, int Count,
                 COLORREF Fill, COLORREF Border = CLR_INVALID, int BorderWidth = 1);

void Line(HDC Dc, int X1, int Y1, int X2, int Y2,
          COLORREF Color, int Width = 1);

void Text(HDC Dc, int X, int Y, const wchar_t* Text,
          int Size, COLORREF Color, bool Bold = false);

void TextShadow(HDC Dc, int X, int Y, const wchar_t* Text,
                int Size, COLORREF Color, COLORREF Shadow, bool Bold = false);

void CenteredText(HDC Dc, int CenterX, int Y, const wchar_t* Text,
                  int Size, COLORREF Color, bool Bold = false,
                  COLORREF Shadow = CLR_INVALID);

int TextWidth(HDC Dc, const wchar_t* Text, int Size, bool Bold = false);

} // namespace Gfx

/*------------------------------------------------------------------*/
/*                        КЛАСС LOCATION                            */
/*------------------------------------------------------------------*/
// Базовая точка-координата. Ничего не рисует и не двигается сама.
class Location {
protected:
    int X;
    int Y;

public:
    Location(int InitX = 0, int InitY = 0);
    virtual ~Location();

    int GetX() const;
    int GetY() const;

    void SetX(int NewX);
    void SetY(int NewY);
    void SetPosition(int NewX, int NewY);
};

/*------------------------------------------------------------------*/
/*                         КЛАСС POINT                              */
/*------------------------------------------------------------------*/
// Точка уже умеет показываться, прятаться и перемещаться.
// Это база для всех графических объектов сцены.
class Point : public Location, public IDisplayable, public IMovable {
protected:
    bool Visible;

public:
    Point(int InitX = 0, int InitY = 0);
    ~Point() override;

    bool IsVisible() const;

    void Show() override;
    void Hide() override;
    void MoveTo(int NewX, int NewY) override;
    void MoveBy(int Dx, int Dy) override;

    // Буксировка стрелками (демонстрация из методички).
    virtual void Drag(int Step);
};

/*------------------------------------------------------------------*/
/*                         КЛАСС PLANE                              */
/*------------------------------------------------------------------*/
// Самолёт: графический объект варианта + физика "прыжка" (flap).
class Plane : public Point, public ICollidable {
private:
    int BodyWidth;
    int BodyHeight;
    int GroundY;

    double PreciseY;    // дробная координата для плавной гравитации
    double VelocityY;
    double Gravity;
    double FlapImpulse;
    double MaxFallSpeed;
    double PropPhase;   // фаза вращения винта
    int FlapFlash;      // короткий след от "прыжка"
    bool Crashed;

public:
    Plane(int InitX, int InitY, int NewGroundY);
    ~Plane() override;

    void Show() override;
    void Hide() override;

    void Reset(int InitX, int InitY, int NewGroundY);
    void Update();          // гравитация + перемещение
    void Animate();         // анимация винта (работает и на стартовом экране)
    void Flap();            // прыжок вверх
    void Crash();           // авария

    bool IsCrashed() const;
    int GetBodyWidth() const;
    int GetBodyHeight() const;
    double GetVelocityY() const;

    HitBox GetHitBox() const override;
    bool IsActive() const override;

private:
    void DrawTail(int CenterY);
    void DrawWing(int CenterY);
    void DrawFuselage(int CenterY, int NoseY);
    void DrawPropeller(int NoseX, int NoseY);
};

/*------------------------------------------------------------------*/
/*                       КЛАСС SKYSCRAPER                           */
/*------------------------------------------------------------------*/
// Конкурирующий объект: серый небоскрёб.
// Умеет рисоваться, ехать справа налево и отдавать свой хитбокс.
class SkyScraper : public Point, public ICollidable {
protected:
    int GroundY;
    int BodyWidth;
    int BodyHeight;
    int AnimTick;
    int WindowSeed;
    bool Scored;

public:
    SkyScraper(int InitX, int InitGroundY,
               int InitWidth = 100, int InitHeight = 280);
    ~SkyScraper() override;

    void Show() override;
    void Hide() override;

    void Scroll(int Dx);
    void Recycle(int NewX, int NewHeight);
    void AdvanceAnimation();

    bool IsOffScreen() const;
    bool IsScored() const;
    void MarkScored();

    int GetWidth() const;
    int GetHeight() const;
    int GetRight() const;

    HitBox GetHitBox() const override;
    bool IsActive() const override { return true; }

protected:
    // Цвета и детали вынесены в virtual-методы, чтобы подклассы
    // меняли внешний вид, не копируя весь Show().
    virtual COLORREF GetBodyColor() const;
    virtual COLORREF GetShadowColor() const;
    virtual COLORREF GetRoofColor() const;
    virtual COLORREF GetWindowColor() const;
    virtual COLORREF GetLitWindowColor() const;
    virtual COLORREF GetOutlineColor() const;

    virtual void DrawRoof();
    virtual void DrawWindows();
    virtual void DrawExtra();
};

/*------------------------------------------------------------------*/
/*                      КЛАСС GLASSTOWER                            */
/*------------------------------------------------------------------*/
// Стеклянная башня — подкласс небоскрёба (другой рисунок окон).
class GlassTower : public SkyScraper {
public:
    GlassTower(int InitX, int InitGroundY,
               int InitWidth = 86, int InitHeight = 300);
    ~GlassTower() override;

protected:
    COLORREF GetBodyColor() const override;
    COLORREF GetShadowColor() const override;
    COLORREF GetRoofColor() const override;
    COLORREF GetWindowColor() const override;
    COLORREF GetLitWindowColor() const override;
    COLORREF GetOutlineColor() const override;

    void DrawRoof() override;
    void DrawWindows() override;
};

/*------------------------------------------------------------------*/
/*                       КЛАСС NEONTOWER                            */
/*------------------------------------------------------------------*/
// Тёмная башня с неоновой вывеской — ещё один подкласс.
class NeonTower : public SkyScraper {
public:
    NeonTower(int InitX, int InitGroundY,
              int InitWidth = 110, int InitHeight = 260);
    ~NeonTower() override;

protected:
    COLORREF GetBodyColor() const override;
    COLORREF GetShadowColor() const override;
    COLORREF GetRoofColor() const override;
    COLORREF GetWindowColor() const override;
    COLORREF GetLitWindowColor() const override;
    COLORREF GetOutlineColor() const override;

    void DrawRoof() override;
    void DrawExtra() override;
};

/*------------------------------------------------------------------*/
/*                          ОБЛАКО                                  */
/*------------------------------------------------------------------*/
struct Cloud {
    double X;
    double Y;
    double Scale;
    double Speed;
};

/*------------------------------------------------------------------*/
/*                       ИГРА / СЦЕНА                               */
/*------------------------------------------------------------------*/
enum class GameState {
    Ready,      // стартовый экран, ждём SPACE
    Playing,    // полёт
    Crashed,    // анимация взрыва
    GameOver    // итоговый экран
};

class Game {
public:
    enum {
        TowerCount = 4,
        CloudCount = 5
    };

    Game(int Width, int Height);
    ~Game();

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    void Reset();
    void Update();
    void Render(HDC Target);
    void PressFlap();
    void ToggleHitBoxes();

    GameState GetState() const;
    int GetScore() const;
    int GetBestScore() const;

private:
    int Width;
    int Height;
    int GroundY;

    Plane Player;
    SkyScraper* Towers[TowerCount];
    Cloud Clouds[CloudCount];

    GameState State;
    int Score;
    int BestScore;
    int CrashTimer;
    int CrashX;
    int CrashY;
    int ScrollSpeed;
    double BobPhase;
    bool ShowHitBoxes;

    std::mt19937 Rng;

    void UpdateClouds();
    void RecycleTowers();
    void CheckScore();
    void CheckCollisions();

    int RandomHeight();
    int RandomGap();

    void DrawSky();
    void DrawClouds();
    void DrawGround();
    void DrawHud();
    void DrawExplosion();
    void DrawReadyOverlay();
    void DrawGameOverOverlay();
    void DrawHitBoxes();
};
