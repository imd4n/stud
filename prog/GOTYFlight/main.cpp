/******************************************************************
 * О Б Ъ Е К Т Н О - О Р И Е Н Т И Р О В А Н Н О Е ПРОГРАММИРОВАНИЕ*
 *-----------------------------------------------------------------*
 * Project Type  : Window Application                              *
 * Project Name  : GOTYFLIGHT                                      *
 * File Name     : main.cpp                                        *
 * Language      : C++21                                           *
 * Comment(s)    : Простой сайдскроллер. Самолёт падает и «прыгает» *
 *                 над башнями, которые едут справа налево.        *
 *                 Считаем удачные перелёты, а не столкновения.    *
 *                 Кодовая база: "Игра", механика: planeexamp.     *
 ******************************************************************/

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>

#include <windows.h>

#include "./mPlaneHierarhy/TBrokenPlane.h"
#include "./mPlaneHierarhy/TPlane.h"
#include "./mTowerHierarhy/TMagicTower.h"
#include "./mTowerHierarhy/TTower.h"
#include "gameSettings.h"

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

HDC hdc; // глобальный контекст (переключается на буферный)

// --------------------- НАСТРОЙКИ ПОЛЁТА ---------------------
// Физика позаимствована у flappy-механики из planeexamp.
const int FLAP_START_Y = 300;   // стартовая высота самолёта
const int SCROLL_SPEED = 5;     // скорость движения башен влево
// Зазор считаем в ширинах дома: от одного до четырёх домов подряд.
const int MIN_HOUSES_GAP = 1;
const int MAX_HOUSES_GAP = 4;

const double GRAVITY = 0.55;
const double FLAP_IMPULSE = -9.0;
const double MAX_FALL_SPEED = 11.0;

// --------------------- ГЛОБАЛЬНОЕ СОСТОЯНИЕ ---------------------
bool gameRunning = true;
int flightsOver = 0; // сколько раз пролетели сверху
Plane *wreck = nullptr; // сломанный самолёт, появляется после удара

// --------------------- САМОЛЁТ С «ПРЫЖКОМ» ---------------------
// Наследник спрайта из базовой кодовой базы: рисование и хитбокс
// остаются в Plane, здесь добавляется только физика полёта.
class FlappyPlane : public Plane {
private:
  double preciseY; // дробная координата для плавной гравитации
  double velocityY;
  bool crashed = false;

public:
  FlappyPlane(int x, int y)
      : Plane(x, y), preciseY(y), velocityY(0.0) {}

  void flap() {
    if (crashed)
      return;
    velocityY = FLAP_IMPULSE;
  }

  void update(int groundY) {
    if (crashed)
      return;

    velocityY += GRAVITY;
    if (velocityY > MAX_FALL_SPEED)
      velocityY = MAX_FALL_SPEED;

    preciseY += velocityY;
    int newY = static_cast<int>(std::floor(preciseY + 0.5));

    // Потолок: выше поля не улетаем.
    if (newY < playAreaTop) {
      newY = playAreaTop;
      preciseY = newY;
      velocityY = 0.0;
    }

    // Касание земли — конец залёта. Самолёт повёрнут, поэтому его
    // экранная «высота» — это половина высоты спрайта плюс половина ширины.
    const int halfOnScreen = getHeight() / 2 + getWidth() / 2 - 5;
    if (newY + halfOnScreen >= groundY) {
      newY = groundY - halfOnScreen;
      setY(newY);
      preciseY = newY;
      crashed = true;
      return;
    }

    setY(newY);
  }

  void crash() { crashed = true; }
  bool isCrashed() const { return crashed; }
};

// --------------------- БАШНИ ---------------------
Tower *towers[TOWERS_NUM] = {0};
bool towerScored[TOWERS_NUM] = {false};

// Расстояние между домами — случайное, но от MIN_HOUSES_GAP до
// MAX_HOUSES_GAP ширин спрайта дома (то есть 1..4 дома).
int randomGap(int houseWidth) {
  int houses = MIN_HOUSES_GAP +
               rand() % (MAX_HOUSES_GAP - MIN_HOUSES_GAP + 1);
  return houseWidth * houses;
}

// Ставим башни за правым краем: первая — сразу за экраном, остальные
// через случайные промежутки.
void initializeTowers() {
  int x = playAreaRight + 80;
  for (int i = 0; i < TOWERS_NUM; i++) {
    switch (rand() % 3) {
    case 0:
      towers[i] = new Tower(x, playAreaBottom);
      break;
    case 1:
      towers[i] = new MagicTower(x, playAreaBottom);
      break;
    default:
      towers[i] = new SmallTower(x, playAreaBottom);
      break;
    }
    towerScored[i] = false;
    x += towers[i]->getWidth() + randomGap(towers[i]->getWidth());
  }
}

int rightmostTowerRight() {
  int rightmost = -100000;
  for (int i = 0; i < TOWERS_NUM; i++) {
    int r = towers[i]->getX() + towers[i]->getWidth();
    if (r > rightmost)
      rightmost = r;
  }
  return rightmost;
}

// Ушедшую за экран башню возвращаем справа и сбрасываем ей «пролёт».
void recycleTower(int index) {
  towers[index]->setX(rightmostTowerRight() +
                      randomGap(towers[index]->getWidth()));
  towers[index]->setY(playAreaBottom); // основание всегда стоит на земле
  towerScored[index] = false;
}

// --------------------- ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ---------------------

// Хитбоксы в базовых классах заданы «на вырост»: у башни он на 50 px выше
// нарисованного корпуса, у самолёта — для неповёрнутого спрайта. Считаем
// хитбоксы по тому, что реально видно на экране, и чуть увеличиваем запас.
struct HitBox {
  int left, top, right, bottom;
};

bool intersect(const HitBox &a, const HitBox &b) {
  return a.left < b.right && a.right > b.left && a.top < b.bottom &&
         a.bottom > b.top;
}

// После поворота экранная ширина самолёта равна высоте его спрайта,
// а экранная высота — ширине. Небольшой отступ внутрь прощает касания краёв.
HitBox planeHitBox(Plane *p) {
  const int centerX = p->getX();
  const int centerY = p->getY() + p->getHeight() / 2;
  const int halfW = p->getHeight() / 2 - 5;
  const int halfH = p->getWidth() / 2 - 5;
  return {centerX - halfW, centerY - halfH, centerX + halfW, centerY + halfH};
}

// Корпус обычной башни рисуется от y-400 до y, магической — от y-450.
// Тонкие шпиль и кристалл в хитбокс не включаем.
HitBox towerHitBox(Tower *t) {
  const bool magic = (t->getID() == 1);
  const int bodyWidth = magic ? 100 : 80;
  const int bodyHeight = magic ? 450 : 400;
  return {t->getX(), t->getY() - bodyHeight, t->getX() + bodyWidth, t->getY()};
}

// Поворачиваем спрайт на 90° по часовой стрелке: нос самолёта смотрит
// вправо, в сторону полёта. Сами спрайты не трогаем — только матрица GDI.
void drawRotated(Plane *obj) {
  const int centerX = obj->getX();
  const int centerY = obj->getY() + obj->getHeight() / 2;

  int oldMode = GetGraphicsMode(hdc);
  SetGraphicsMode(hdc, GM_ADVANCED);

  XFORM oldForm;
  GetWorldTransform(hdc, &oldForm);

  XFORM rotate = {};
  rotate.eM11 = 0.0f;
  rotate.eM12 = 1.0f;
  rotate.eM21 = -1.0f;
  rotate.eM22 = 0.0f;
  rotate.eDx = static_cast<float>(centerX + centerY);
  rotate.eDy = static_cast<float>(centerY - centerX);
  SetWorldTransform(hdc, &rotate);

  obj->show();

  SetWorldTransform(hdc, &oldForm);
  SetGraphicsMode(hdc, oldMode);
}

// --------------------- HUD ---------------------
void drawHUD(HDC context, int flights, bool crashed) {
  SetBkMode(context, TRANSPARENT);
  SetTextColor(context, RGB(255, 255, 255));

  char buf[128];
  sprintf(buf, "FLIGHTS OVER: %d", flights);
  TextOut(context, 10, 10, buf, strlen(buf));

  if (crashed) {
    sprintf(buf, "CRASHED - ESC TO EXIT");
    TextOut(context, 10, 40, buf, strlen(buf));
  }
}

// --------------------- ОКНО ИГРЫ ---------------------
LRESULT CALLBACK GameWndProc(HWND hwnd, UINT msg, WPARAM wParam,
                             LPARAM lParam) {
  switch (msg) {
  case WM_CLOSE:
    gameRunning = false;
    return 0;
  case WM_DESTROY:
    gameRunning = false;
    return 0;
  default:
    return DefWindowProc(hwnd, msg, wParam, lParam);
  }
}

// --------------------- ГЛАВНАЯ ФУНКЦИЯ ---------------------

int main() {
  srand(static_cast<unsigned int>(time(nullptr)));

  HINSTANCE hInst = GetModuleHandle(nullptr);
  const char *const className = "GOTYFlightWindow";

  WNDCLASSEXA wc = {};
  wc.cbSize = sizeof(wc);
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = GameWndProc;
  wc.hInstance = hInst;
  wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
  wc.lpszClassName = className;
  if (!RegisterClassExA(&wc)) {
    std::cout << "RegisterClassEx FAILED\n";
    return 1;
  }

  RECT winRect = {0, 0, windowW, windowH};
  AdjustWindowRectEx(&winRect, WS_OVERLAPPEDWINDOW, FALSE, 0);
  HWND hwnd = CreateWindowExA(0, className, "GOTYFlight - Flappy Flight",
                              WS_OVERLAPPEDWINDOW, windowX, windowY,
                              winRect.right - winRect.left,
                              winRect.bottom - winRect.top,
                              nullptr, nullptr, hInst, nullptr);
  if (hwnd == NULL) {
    std::cout << "CreateWindowEx FAILED\n";
    return 1;
  }
  ShowWindow(hwnd, SW_SHOW);
  UpdateWindow(hwnd);

  HDC hdcScreen = GetDC(hwnd);
  if (hdcScreen == 0) {
    std::cout << "HDC NOT FOUND\n";
    return 2;
  }
  hdc = hdcScreen;

  // --------------------- ДВОЙНАЯ БУФЕРИЗАЦИЯ ---------------------
  int bufW = windowW;
  int bufH = windowH;

  HDC hdcBack = CreateCompatibleDC(hdcScreen);
  HBITMAP backBmp = CreateCompatibleBitmap(hdcScreen, bufW, bufH);
  HBITMAP oldBackBmp = (HBITMAP)SelectObject(hdcBack, backBmp);

  HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
  RECT fullRect = {0, 0, bufW, bufH};
  FillRect(hdcBack, &fullRect, blackBrush);
  DeleteObject(blackBrush);

  HPEN borderPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
  SelectObject(hdcBack, borderPen);
  SelectObject(hdcBack, (HBRUSH)GetStockObject(NULL_BRUSH));
  Rectangle(hdcBack, playAreaLeft, playAreaTop, playAreaRight, playAreaBottom);
  DeleteObject(borderPen);

  HDC hdcFrame = CreateCompatibleDC(hdcScreen);
  HBITMAP frameBmp = CreateCompatibleBitmap(hdcScreen, bufW, bufH);
  HBITMAP oldFrameBmp = (HBITMAP)SelectObject(hdcFrame, frameBmp);

  // --------------------- ИНИЦИАЛИЗАЦИЯ ИГРЫ ---------------------
  FlappyPlane *bird = new FlappyPlane(planeX0, FLAP_START_Y);
  initializeTowers();

  bool prevFlapDown = false;
  const std::chrono::milliseconds frameDuration(1000 / GAME_FPS);
  auto lastTime = std::chrono::steady_clock::now();

  // --------------------- ГЛАВНЫЙ ЦИКЛ ---------------------
  MSG msg;
  while (gameRunning) {
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
      if (msg.message == WM_QUIT) {
        gameRunning = false;
        break;
      }
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    if (!gameRunning)
      break;

    auto now = std::chrono::steady_clock::now();
    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTime);

    if (KEY_DOWN(VK_ESCAPE))
      gameRunning = false;

    if (elapsed >= frameDuration) {
      lastTime = now;

      // Переключаем глобальный hdc на буфер и копируем статический фон.
      hdc = hdcFrame;
      BitBlt(hdcFrame, 0, 0, bufW, bufH, hdcBack, 0, 0, SRCCOPY);

      // ---------- УПРАВЛЕНИЕ: прыжок по нажатию ----------
      bool flapDown = KEY_DOWN(VK_SPACE) || KEY_DOWN(VK_UP) || KEY_DOWN('W');
      if (flapDown && !prevFlapDown && !bird->isCrashed())
        bird->flap();
      prevFlapDown = flapDown;

      // ---------- ДВИЖЕНИЕ ----------
      if (!bird->isCrashed()) {
        bird->update(playAreaBottom);
        const HitBox birdBox = planeHitBox(bird);

        for (int i = 0; i < TOWERS_NUM; i++) {
          // Башни едут справа налево (не сверху вниз, как в старой игре).
          towers[i]->setX(towers[i]->getX() - SCROLL_SPEED);

          const HitBox towerBox = towerHitBox(towers[i]);

          if (towerBox.right < playAreaLeft)
            recycleTower(i);

          // Пролетели сверху: башня целиком осталась позади самолёта.
          if (!towerScored[i] && towerBox.right < birdBox.left) {
            towerScored[i] = true;
            flightsOver++;
          }

          // Врезались — залёт закончен.
          if (intersect(birdBox, towerBox))
            bird->crash();
        }
      }

      // После удара вместо целого самолёта показываем обломки.
      if (bird->isCrashed() && wreck == nullptr)
        wreck = new BrokenPlane(bird->getX(), bird->getY());

      // ---------- ОТРИСОВКА ----------
      for (int i = 0; i < TOWERS_NUM; i++)
        towers[i]->show();

      if (wreck)
        drawRotated(wreck);
      else
        drawRotated(bird);
      drawHUD(hdcFrame, flightsOver, bird->isCrashed());

      hdc = hdcScreen;
      BitBlt(hdcScreen, 0, 0, bufW, bufH, hdcFrame, 0, 0, SRCCOPY);
      hdc = hdcFrame;
    } else {
      Sleep(1);
    }
  }

  // --------------------- ОСВОБОЖДЕНИЕ РЕСУРСОВ ---------------------
  delete bird;
  delete wreck;
  for (int i = 0; i < TOWERS_NUM; i++)
    delete towers[i];

  SelectObject(hdcBack, oldBackBmp);
  DeleteObject(backBmp);
  DeleteDC(hdcBack);

  SelectObject(hdcFrame, oldFrameBmp);
  DeleteObject(frameBmp);
  DeleteDC(hdcFrame);

  ReleaseDC(hwnd, hdcScreen);
  DestroyWindow(hwnd);
  UnregisterClassA(className, hInst);
  return 0;
}
