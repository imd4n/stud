/******************************************************************
 * О Б Ъ Е К Т Н О - О Р И Е Н Т И Р О В А Н Н О Е ПРОГРАММИРОВАНИЕ*
 *-----------------------------------------------------------------*
 * Project Type  : Console Application                             *
 * Project Name  : PLANE_GAMAE                                     *
 * File Name     : main.cpp                                        *
 * Language      : C++21                                           *
 * Programmer(s) : Кручинин Н.М.                                   *
 * Modified By   : Кручинин Н.М.                                   *
 * Created       : 26/04/04                                        *
 * Last Revision : 25/01/25                                        *
 * Comment(s)    :                                                 *
 *                                                                 *
 ******************************************************************/
/*
#include <cstdlib>
#include <ctime>
#include <synchapi.h>
#include <windef.h>
#include <windows.h>

#include "./mObjects/TRepairZone.h"
#include "./mObjects/TRockets.h"
#include "./mPlaneHierarhy/TBrokenPlane.h"
#include "./mPlaneHierarhy/TMagicPlane.h"
#include "./mPlaneHierarhy/TPlane.h"
#include "./mTowerHierarhy/TMagicTower.h"
#include "./mTowerHierarhy/TTower.h"
#include "mObjects/TRocketsFill.h"
#include "mObjects/TStarBonus.h"
#include "mPlaneHierarhy/TRepairedPlane.h"

#include "gameSettings.h"

#include <chrono>
#include <iostream>
#include <wingdi.h>
#include <winnt.h>
#include <winscard.h>

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

HDC hdc; // глобальный контекст (переключается на буферный)

// Глобальные переменные для башен
Tower *towers[TOWERS_NUM] = {0};
int activeTowerCount = 0;
auto lastTowerSpawnTime = std::chrono::steady_clock::now();
int nextSpawnInterval = MIN_SPAWN_INTERVAL;
const std::chrono::milliseconds rocketDelay(1000);

// Массив самолётов (все типы)
Plane *planes[] = {new Plane(), new AttackPlane(), new MagicPlane(),
                   new BrokenPlane(), new RepairedPlane()};

int planeHealth = PLANE_HEALTH;
int score = 0;
bool gameRunning = true;

// Глобальные переменные для звёздочки и волшебства
StarBonus *star = nullptr;
auto starSpawnTime = std::chrono::steady_clock::now();
bool magicActive = false;
std::chrono::steady_clock::time_point magicEndTime;

// ------------------------ ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ------------------------

template <typename T1, typename T2> bool isCollision(T1 *obj1, T2 *obj2) {
  double x1 = obj1->getCollisionX();
  double y1 = obj1->getCollisionY();
  double w1 = obj1->getWidth();
  double h1 = obj1->getHeight();
  double x2 = obj2->getCollisionX();
  double y2 = obj2->getCollisionY();
  double w2 = obj2->getWidth();
  double h2 = obj2->getHeight();
  return (x1 + w1 >= x2) && (x2 + w2 >= x1) && (y1 + h1 >= y2) &&
         (y2 + h2 >= y1);
}

template <typename T> bool isOutbound(T *obj) {
  double x = obj->getX();
  double y = obj->getY();
  double w = obj->getWidth();
  double h = obj->getHeight();
  return (x < playAreaLeft || x + w > playAreaRight || y < playAreaTop ||
          y + h > playAreaBottom);
}

enum directions { up = 38, down = 40, left = 37, right = 39, none = 0 };

directions keyHandler() {
  if (KEY_DOWN(up))
    return up;
  if (KEY_DOWN(down))
    return down;
  if (KEY_DOWN(left))
    return left;
  if (KEY_DOWN(right))
    return right;
  return none;
}

void moveObject(Point *obj, directions direction, int step) {
  if (direction == up)
    obj->moveTo(obj->getX(), obj->getY() - step);
  else if (direction == down)
    obj->moveTo(obj->getX(), obj->getY() + step);
  else if (direction == left)
    obj->moveTo(obj->getX() - step, obj->getY());
  else if (direction == right)
    obj->moveTo(obj->getX() + step, obj->getY());
}

void planeOutbound(Plane *currentPlane) {
  double x = currentPlane->getCollisionX();
  double y = currentPlane->getCollisionY();
  double w = currentPlane->getWidth();
  double h = currentPlane->getHeight();
  double dx = currentPlane->getX() - x;
  double dy = currentPlane->getY() - y;

  if (x < playAreaLeft)
    x = playAreaLeft;
  if (x + w > playAreaRight)
    x = playAreaRight - w;
  if (y < playAreaTop)
    y = playAreaTop;
  if (y + h > playAreaBottom)
    y = playAreaBottom - h;

  currentPlane->moveTo(x + dx, y + dy);
}

void initializeTowers() {
  for (int i = 0; i < TOWERS_NUM; i++) {
    if (towers[i]) {
      delete towers[i];
      towers[i] = nullptr;
    }
  }
  activeTowerCount = 0;
}

int findFreeTowerSlot() {
  for (int i = 0; i < TOWERS_NUM; i++)
    if (towers[i] == nullptr)
      return i;
  return -1;
}

bool spawnNewTower() {
  if (activeTowerCount >= TOWERS_NUM)
    return false;
  int slot = findFreeTowerSlot();
  if (slot == -1)
    return false;

  int x = playAreaLeft + rand() % (playAreaRight - playAreaLeft - 50);
  int y = playAreaTop - 100;
  int type = rand() % 3;
  switch (type) {
  case 0:
    towers[slot] = new Tower(x, y);
    break;
  case 1:
    towers[slot] = new MagicTower(x, y);
    break;
  case 2:
    towers[slot] = new SmallTower(x, y);
    break;
  default:
    towers[slot] = new Tower(x, y);
  }
  activeTowerCount++;
  nextSpawnInterval = MIN_SPAWN_INTERVAL +
                      rand() % (MAX_SPAWN_INTERVAL - MIN_SPAWN_INTERVAL + 1);
  lastTowerSpawnTime = std::chrono::steady_clock::now();
  return true;
}

void updateTowers() {
  for (int i = 0; i < TOWERS_NUM; i++) {
    if (towers[i]) {
      double oldY = towers[i]->getY();
      towers[i]->move();
      if (oldY > playAreaBottom) {
        towers[i]->hide();
        delete towers[i];
        towers[i] = nullptr;
        activeTowerCount--;
      }
    }
  }
}

void checkTowerSpawn() {
  auto now = std::chrono::steady_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                     now - lastTowerSpawnTime)
                     .count();
  if (elapsed >= nextSpawnInterval)
    spawnNewTower();
}

void updateRockets(Plane *pl) {
  int plID = pl->getID();
  for (int i = 0; i < 2; i++) {
    Rockets *r = pl->getr(i);
    if (r && r->getVisible()) {
      for (int j = 0; j < TOWERS_NUM; j++) {
        if (towers[j] && isCollision(r, towers[j])) {
          // Если ракета от AttackPlane (ID=1) и башня волшебная (ID=1) –
          // пропустить
          if (plID == 1 && towers[j]->getID() == 1) {
            continue;
          }
          // Иначе уничтожаем башню и ракету
          towers[j]->hide();
          delete towers[j];
          towers[j] = nullptr;
          activeTowerCount--;
          r->hide();
        }
      }
      if (isOutbound(r))
        r->hide();
      if (r->getVisible())
        r->move();
    }
  }
}
/*
void updateRockets(Plane *pl) {
  for (int i = 0; i < 2; i++) {
    Rockets *r = pl->getr(i);
    if (r && r->getVisible()) {
      for (int j = 0; j < TOWERS_NUM; j++) {
        if (towers[j] && isCollision(r, towers[j])) {
          towers[j]->hide();
          delete towers[j];
          towers[j] = nullptr;
          activeTowerCount--;
          r->hide();
        }
      }
      if (isOutbound(r))
        r->hide();
      if (r->getVisible())
        r->move();
    }
  }
}*/
/*
int checkPlaneTowerCollisions(Plane *plane, bool &collisionOccurred) {
  int newID = plane->getID();
  collisionOccurred = false;

  // Волшебный самолёт неуязвим для башен
  if (plane->getID() == 2) {
    return newID;
  }

  for (int i = 0; i < TOWERS_NUM; i++) {
    if (towers[i] && isCollision(plane, towers[i])) {
      collisionOccurred = true;
      planeHealth -= towers[i]->getDamage();

      // Логика смены типа
      if (plane->getID() == 1) { // AttackPlane -> BrokenPlane
        newID = 3;
      }
      if (plane->getID() == 2) {
        newID = 1;
      }
      // если уже BrokenPlane (ID=3) – остаётся сломанным

      delete towers[i];
      towers[i] = nullptr;
      activeTowerCount--;
    }
  }
  return newID;
}

void drawHUD(int health, int maxHealth, HDC context) {
  int barWidth = 200;
  int barHeight = 20;
  int barX = 150;
  int barY = 30;

  HBRUSH bgBrush = CreateSolidBrush(RGB(0, 0, 0));
  RECT bgRect = {barX, barY, barX + barWidth, barY + barHeight};
  FillRect(context, &bgRect, bgBrush);
  DeleteObject(bgBrush);

  int currentWidth = (health * barWidth) / maxHealth;
  if (currentWidth > 0) {
    HBRUSH healthBrush = CreateSolidBrush(RGB(0, 255, 0));
    RECT healthRect = {barX, barY, barX + currentWidth, barY + barHeight};
    FillRect(context, &healthRect, healthBrush);
    DeleteObject(healthBrush);
  }
  if (currentWidth < barWidth) {
    HBRUSH lostBrush = CreateSolidBrush(RGB(255, 0, 0));
    RECT lostRect = {barX + currentWidth, barY, barX + barWidth,
                     barY + barHeight};
    FillRect(context, &lostRect, lostBrush);
    DeleteObject(lostBrush);
  }

  HPEN borderPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
  SelectObject(context, borderPen);
  SelectObject(context, (HBRUSH)GetStockObject(NULL_BRUSH));
  Rectangle(context, barX, barY, barX + barWidth, barY + barHeight);
  DeleteObject(borderPen);
}

// ----------------------------------------------------------------------

int main() {
  srand(static_cast<unsigned int>(time(nullptr)));

  HWND hwnd = GetConsoleWindow();
  if (hwnd == NULL) {
    std::cout << "Console Window NOT FOUND\n";
    return 1;
  }

  HDC hdcScreen = GetWindowDC(hwnd);
  if (hdcScreen == 0) {
    std::cout << "HDC NOT FOUND\n";
    return 2;
  }
  hdc = hdcScreen;

  SetWindowPos(hwnd, NULL, windowX, windowY, windowW, windowH, 0);
  Sleep(100);

  // ----------------------------------------------------------------
  //  ДВОЙНАЯ БУФЕРИЗАЦИЯ
  // ----------------------------------------------------------------
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

  // ----------------------------------------------------------------
  //  ИНИЦИАЛИЗАЦИЯ ИГРЫ
  // ----------------------------------------------------------------
  int step = 5;
  auto lastTime = std::chrono::steady_clock::now();
  auto rocketLastTime = std::chrono::steady_clock::now();
  const std::chrono::milliseconds frameDuration(1000 / GAME_FPS);

  Plane *currentPlane = planes[1];
  initializeTowers();

  // Зона починки – левый верхний угол
  RepairZone *repair = new RepairZone(playAreaRight-130, 30);
  // Зона пополнения ракет – правый верхний угол
  RocketsFill *rockets = new RocketsFill(30, playAreaBottom-100);

  RECT playAreaRect = {playAreaLeft, playAreaTop, playAreaRight,
                       playAreaBottom};

  // ----------------------------------------------------------------
  //  ГЛАВНЫЙ ЦИКЛ
  // ----------------------------------------------------------------
  while (gameRunning) {
    auto now = std::chrono::steady_clock::now();
    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTime);
    auto rocketElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - rocketLastTime);
    auto starElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                           now - starSpawnTime)
                           .count();

    if (KEY_DOWN(VK_ESCAPE))
      gameRunning = false;

    if (star == nullptr && starElapsed >= STAR_SPAWN_INTERVAL) {
      int x = playAreaLeft + rand() % (playAreaRight - playAreaLeft - 30);
      star = new StarBonus(x, playAreaTop);
      star->show();
      starSpawnTime = now;
    }

    if (elapsed >= frameDuration) {
      lastTime = now;

      // 1. Переключаем глобальный hdc на буфер
      hdc = hdcFrame;

      // 2. Копируем статический фон
      BitBlt(hdcFrame, 0, 0, bufW, bufH, hdcBack, 0, 0, SRCCOPY);

      // 3. Логика движения и ввода
      directions pressed = keyHandler();
      moveObject(currentPlane, pressed, step);
      planeOutbound(currentPlane);

      // 4. Ракеты
      // Выстрел
      if (rocketElapsed >= rocketDelay && KEY_DOWN(VK_SPACE) &&
          (currentPlane->getID() == 1 || currentPlane->getID() == 2)) {
        currentPlane->shoot();
        rocketLastTime = now;
      }

      // Обновление ракет
      if (currentPlane->getID() == 1 || currentPlane->getID() == 2)
        updateRockets(currentPlane);

      // 5. Башни
      checkTowerSpawn();
      updateTowers();

      if (star) {
        star->moveDown();
        if (star->getY() > playAreaBottom) {
          star->hide();
          delete star;
          star = nullptr;
        } else if (isCollision(currentPlane, star)) {
          star->hide();
          delete star;
          star = nullptr;
          if (currentPlane->getID() != 2) { // если не волшебный
            int lastX = currentPlane->getX();
            int lastY = currentPlane->getY();
            currentPlane->hide();
            currentPlane = planes[2]; // MagicPlane
            currentPlane->moveTo(lastX, lastY);
            magicActive = true;
            magicEndTime = now + std::chrono::milliseconds(MAGIC_DURATION_MS);
          }
        }
      }

      if (magicActive && currentPlane->getID() == 2) {
        if (now >= magicEndTime) {
          int lastX = currentPlane->getX();
          int lastY = currentPlane->getY();
          currentPlane->hide();
          currentPlane = planes[1]; // обратно на AttackPlane
          currentPlane->moveTo(lastX, lastY);
          magicActive = false;
        }
      }

      // 6. Столкновения с башнями
      bool collisionThisFrame = false;
      int lastX = currentPlane->getX();
      int lastY = currentPlane->getY();
      int newID = checkPlaneTowerCollisions(currentPlane, collisionThisFrame);
      if (newID != currentPlane->getID()) {
        currentPlane->hide();
        currentPlane = planes[newID];
        currentPlane->moveTo(lastX, lastY);
      }

      // 7. Зона ремонта
      // Зона починки – чинит только сломанный самолёт (ID=3)
      if (currentPlane->getID() == 3 && isCollision(currentPlane, repair)) {
        int lx = currentPlane->getX();
        int ly = currentPlane->getY();
        currentPlane->hide();
        currentPlane = planes[1]; // AttackPlane
        currentPlane->moveTo(lx, ly);
        magicActive = false; // сброс волшебства, если было
      }

      // 8. Подбор ракет
      if ((currentPlane->getID() == 1 || currentPlane->getID() == 2) && isCollision(currentPlane, rockets)) {
        currentPlane->setRockets(1);
      }

      // ----------------------------------------------------------
      // 9. ОТРИСОВКА ВСЕХ ОБЪЕКТОВ
      // ----------------------------------------------------------
      repair->show();
      rockets->show();

      // Башни – рисуем все существующие, без проверки видимости
      for (int i = 0; i < TOWERS_NUM; i++) {
        if (towers[i]) {
          towers[i]->show();
        }
      }

      // Ракеты – для AttackPlane и MagicPlane
      if (currentPlane->getID() == 1 || currentPlane->getID() == 2) {
        for (int i = 0; i < 2; i++) {
          Rockets *r = currentPlane->getr(i);
          if (r && r->getVisible())
            r->show();
        }
      }

      // Самолёт
      currentPlane->show();

      // HUD
      drawHUD(planeHealth, PLANE_HEALTH, hdcFrame);

      // ----------------------------------------------------------
      // 10. ЭФФЕКТ СТОЛКНОВЕНИЯ (если было)
      // ----------------------------------------------------------
      if (collisionThisFrame) {
        HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
        FillRect(hdcFrame, &playAreaRect, redBrush);
        DeleteObject(redBrush);
      }

      // 11. Копируем кадр на экран
      hdc = hdcScreen;
      BitBlt(hdcScreen, 0, 0, bufW, bufH, hdcFrame, 0, 0, SRCCOPY);

      // 12. Если столкновение – задержка для эффекта
      if (collisionThisFrame) {
        Sleep(500);
      }

      // Возвращаем hdc на буфер
      hdc = hdcFrame;
    } else {
      Sleep(1);
    }
  }

  // ----------------------------------------------------------------
  //  ОСВОБОЖДЕНИЕ РЕСУРСОВ
  // ----------------------------------------------------------------
  delete repair;
  delete rockets;
  for (int i = 0; i < TOWERS_NUM; i++)
    delete towers[i];
  for (int i = 0; i < sizeof(planes) / sizeof(planes[0]); i++)
    delete planes[i];

  SelectObject(hdcBack, oldBackBmp);
  DeleteObject(backBmp);
  DeleteDC(hdcBack);

  SelectObject(hdcFrame, oldFrameBmp);
  DeleteObject(frameBmp);
  DeleteDC(hdcFrame);

  ReleaseDC(hwnd, hdcScreen);
  return 0;
} */

/******************************************************************
 * О Б Ъ Е К Т Н О - О Р И Е Н Т И Р О В А Н Н О Е ПРОГРАММИРОВАНИЕ*
 *-----------------------------------------------------------------*
 * Project Type  : Console Application                             *
 * Project Name  : PLANE_GAMAE                                     *
 * File Name     : main.cpp                                        *
 * Language      : C++21                                           *
 * Programmer(s) : Кручинин Н.М.                                   *
 * Modified By   : Кручинин Н.М.                                   *
 * Created       : 26/04/04                                        *
 * Last Revision : 25/01/25                                        *
 * Comment(s)    :                                                 *
 *                                                                 *
 ******************************************************************/

#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <synchapi.h>
#include <windef.h>
#include <windows.h>

#include "./mObjects/TRepairZone.h"
#include "./mObjects/TRockets.h"
#include "./mPlaneHierarhy/TBrokenPlane.h"
#include "./mPlaneHierarhy/TMagicPlane.h"
#include "./mPlaneHierarhy/TPlane.h"
#include "./mTowerHierarhy/TMagicTower.h"
#include "./mTowerHierarhy/TTower.h"
#include "mObjects/TRocketsFill.h"
#include "mPlaneHierarhy/TRepairedPlane.h"

#include "gameSettings.h"

#include <chrono>
#include <iostream>
#include <wingdi.h>
#include <winnt.h>
#include <winscard.h>

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

HDC hdc;

// --------------------- ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ ---------------------

Tower *towers[TOWERS_NUM] = {0};
int activeTowerCount = 0;
auto lastTowerSpawnTime = std::chrono::steady_clock::now();
int nextSpawnInterval = MIN_SPAWN_INTERVAL;
const std::chrono::milliseconds rocketDelay(1000);

Plane *planes[] = {new Plane(), new AttackPlane(), new MagicPlane(),
                   new BrokenPlane(), new RepairedPlane()};

int planeHealth = PLANE_HEALTH;
int score = 0;
bool gameRunning = true;

// --------------------- ПЕРЕМЕННЫЕ УРОВНЕЙ ---------------------
int currentLevel = 1;
int killedNormal = 0;
int killedMagic = 0;
const int targetNormalLvl1 = 5;
const int targetNormalLvl2 = 5;
const int targetMagicLvl2 = 3;
bool bonusesEnabled = false; // бонусы (звёзды) доступны начиная со 2 уровня

// --------------------- БОНУСЫ ---------------------
class StarBonus : public Point {
private:
  int width = 30;
  int height = 30;

public:
  StarBonus(int x, int y) : Point(x, y) {}
  void show() override {
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 0));
    HBRUSH brush = CreateSolidBrush(RGB(255, 255, 0));
    SelectObject(hdc, pen);
    SelectObject(hdc, brush);
    POINT pts[4] = {{x, y - 15}, {x + 15, y}, {x, y + 15}, {x - 15, y}};
    Polygon(hdc, pts, 4);
    DeleteObject(pen);
    DeleteObject(brush);
  }
  void hide() override {
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
    HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, pen);
    SelectObject(hdc, brush);
    POINT pts[4] = {{x, y - 15}, {x + 15, y}, {x, y + 15}, {x - 15, y}};
    Polygon(hdc, pts, 4);
    DeleteObject(pen);
    DeleteObject(brush);
  }
  void moveDown() {
    hide();
    y += 3;
    show();
  }
  int getCollisionX() { return x; }
  int getCollisionY() { return y; }
  int getWidth() { return width; }
  int getHeight() { return height; }
};

StarBonus *star = nullptr;
auto starSpawnTime = std::chrono::steady_clock::now();
bool magicActive = false;
std::chrono::steady_clock::time_point magicEndTime;

// --------------------- ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ---------------------

template <typename T1, typename T2> bool isCollision(T1 *obj1, T2 *obj2) {
  double x1 = obj1->getCollisionX();
  double y1 = obj1->getCollisionY();
  double w1 = obj1->getWidth();
  double h1 = obj1->getHeight();
  double x2 = obj2->getCollisionX();
  double y2 = obj2->getCollisionY();
  double w2 = obj2->getWidth();
  double h2 = obj2->getHeight();
  return (x1 + w1 >= x2) && (x2 + w2 >= x1) && (y1 + h1 >= y2) &&
         (y2 + h2 >= y1);
}

template <typename T> bool isOutbound(T *obj) {
  double x = obj->getX();
  double y = obj->getY();
  double w = obj->getWidth();
  double h = obj->getHeight();
  return (x < playAreaLeft || x + w > playAreaRight || y < playAreaTop ||
          y + h > playAreaBottom);
}

enum directions { up = 38, down = 40, left = 37, right = 39, none = 0 };

directions keyHandler() {
  if (KEY_DOWN(up))
    return up;
  if (KEY_DOWN(down))
    return down;
  if (KEY_DOWN(left))
    return left;
  if (KEY_DOWN(right))
    return right;
  return none;
}

void moveObject(Point *obj, directions direction, int step) {
  if (direction == up)
    obj->moveTo(obj->getX(), obj->getY() - step);
  else if (direction == down)
    obj->moveTo(obj->getX(), obj->getY() + step);
  else if (direction == left)
    obj->moveTo(obj->getX() - step, obj->getY());
  else if (direction == right)
    obj->moveTo(obj->getX() + step, obj->getY());
}

void planeOutbound(Plane *currentPlane) {
  double x = currentPlane->getCollisionX();
  double y = currentPlane->getCollisionY();
  double w = currentPlane->getWidth();
  double h = currentPlane->getHeight();
  double dx = currentPlane->getX() - x;
  double dy = currentPlane->getY() - y;

  if (x < playAreaLeft)
    x = playAreaLeft;
  if (x + w > playAreaRight)
    x = playAreaRight - w;
  if (y < playAreaTop)
    y = playAreaTop;
  if (y + h > playAreaBottom)
    y = playAreaBottom - h;

  currentPlane->moveTo(x + dx, y + dy);
}

void initializeTowers() {
  for (int i = 0; i < TOWERS_NUM; i++) {
    if (towers[i]) {
      delete towers[i];
      towers[i] = nullptr;
    }
  }
  activeTowerCount = 0;
}

int findFreeTowerSlot() {
  for (int i = 0; i < TOWERS_NUM; i++)
    if (towers[i] == nullptr)
      return i;
  return -1;
}

// --------------------- СПАВН БАШЕН С УЧЁТОМ УРОВНЯ ---------------------
bool spawnNewTower() {
  if (activeTowerCount >= TOWERS_NUM)
    return false;
  int slot = findFreeTowerSlot();
  if (slot == -1)
    return false;

  int x = playAreaLeft + rand() % (playAreaRight - playAreaLeft - 100);
  int y = playAreaTop - 100;

  int type;
  if (currentLevel == 1) {
    type = 0; // только обычные
  } else if (currentLevel == 2) {
    type = (rand() % 2 == 0) ? 0 : 1; // 50/50 обычные/магические
  } else {                            // level 3
    type = rand() % 3;                // все три типа
  }

  switch (type) {
  case 0:
    towers[slot] = new Tower(x, y);
    break;
  case 1:
    towers[slot] = new MagicTower(x, y);
    break;
  case 2:
    towers[slot] = new SmallTower(x, y);
    break;
  default:
    towers[slot] = new Tower(x, y);
  }
  activeTowerCount++;
  nextSpawnInterval = MIN_SPAWN_INTERVAL +
                      rand() % (MAX_SPAWN_INTERVAL - MIN_SPAWN_INTERVAL + 1);
  lastTowerSpawnTime = std::chrono::steady_clock::now();
  return true;
}

// --------------------- УНИЧТОЖЕНИЕ БАШНИ С УЧЁТОМ ПРОГРЕССА
// ---------------------
void destroyTower(int index) {
  if (towers[index] == nullptr)
    return;
  int id = towers[index]->getID();
  if (id == 0)
    killedNormal++;
  else if (id == 1)
    killedMagic++;
  // SmallTower (id==2) не влияет на прогресс

  delete towers[index];
  towers[index] = nullptr;
  activeTowerCount--;

  // Проверка перехода уровня
  if (currentLevel == 1 && killedNormal >= targetNormalLvl1) {
    currentLevel = 2;
    bonusesEnabled = true;
    killedNormal = 0;
    killedMagic = 0;
    // очистка поля
    for (int i = 0; i < TOWERS_NUM; i++) {
      if (towers[i]) {
        delete towers[i];
        towers[i] = nullptr;
        activeTowerCount--;
      }
    }
    lastTowerSpawnTime = std::chrono::steady_clock::now();
    nextSpawnInterval = MIN_SPAWN_INTERVAL;
    if (star) {
      star->hide();
      delete star;
      star = nullptr;
    }
    starSpawnTime = std::chrono::steady_clock::now();
  } else if (currentLevel == 2 && killedNormal >= targetNormalLvl2 &&
             killedMagic >= targetMagicLvl2) {
    currentLevel = 3;
    bonusesEnabled = true;
    killedNormal = 0;
    killedMagic = 0;
    for (int i = 0; i < TOWERS_NUM; i++) {
      if (towers[i]) {
        delete towers[i];
        towers[i] = nullptr;
        activeTowerCount--;
      }
    }
    lastTowerSpawnTime = std::chrono::steady_clock::now();
    nextSpawnInterval = MIN_SPAWN_INTERVAL;
    if (star) {
      star->hide();
      delete star;
      star = nullptr;
    }
    starSpawnTime = std::chrono::steady_clock::now();
  }
  // Уровень 3 – бесконечный
}

void updateTowers() {
  for (int i = 0; i < TOWERS_NUM; i++) {
    if (towers[i]) {
      double oldY = towers[i]->getY();
      towers[i]->move();
      if (oldY > playAreaBottom) {
        towers[i]->hide();
        delete towers[i];
        towers[i] = nullptr;
        activeTowerCount--;
      }
    }
  }
}

void checkTowerSpawn() {
  auto now = std::chrono::steady_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                     now - lastTowerSpawnTime)
                     .count();
  if (elapsed >= nextSpawnInterval)
    spawnNewTower();
}

void updateRockets(Plane *pl) {
  int plID = pl->getID();
  for (int i = 0; i < 2; i++) {
    Rockets *r = pl->getr(i);
    if (r && r->getVisible()) {
      for (int j = 0; j < TOWERS_NUM; j++) {
        if (towers[j] && isCollision(r, towers[j])) {
          // Обычная ракета (от AttackPlane) не уничтожает магические башни
          if (plID == 1 && towers[j]->getID() == 1) {
            continue;
          }
          // Иначе уничтожаем башню
          destroyTower(j);
          r->hide();
        }
      }
      if (isOutbound(r))
        r->hide();
      if (r->getVisible())
        r->move();
    }
  }
}

int checkPlaneTowerCollisions(Plane *plane, bool &collisionOccurred) {
  int newID = plane->getID();
  collisionOccurred = false;

  // Волшебный самолёт неуязвим
  if (plane->getID() == 2) {
    return newID;
  }

  for (int i = 0; i < TOWERS_NUM; i++) {
    if (towers[i] && isCollision(plane, towers[i])) {
      collisionOccurred = true;
      planeHealth -= towers[i]->getDamage();

      // Логика смены типа
      if (plane->getID() == 1) { // AttackPlane -> BrokenPlane
        newID = 3;
      }
      // если уже BrokenPlane (ID=3) – остаётся сломанным

      destroyTower(i);
    }
  }
  return newID;
}

// --------------------- ОТРИСОВКА HUD (ТЕКСТ И ПОЛОСКА) ---------------------
void drawHUD(HDC context, int health, int maxHealth, int level,
             int normalKilled, int magicKilled) {
  // Полоска здоровья
  int barWidth = 200;
  int barHeight = 20;
  int barX = 150;
  int barY = 30;

  HBRUSH bgBrush = CreateSolidBrush(RGB(0, 0, 0));
  RECT bgRect = {barX, barY, barX + barWidth, barY + barHeight};
  FillRect(context, &bgRect, bgBrush);
  DeleteObject(bgBrush);

  int currentWidth = (health * barWidth) / maxHealth;
  if (currentWidth > 0) {
    HBRUSH healthBrush = CreateSolidBrush(RGB(0, 255, 0));
    RECT healthRect = {barX, barY, barX + currentWidth, barY + barHeight};
    FillRect(context, &healthRect, healthBrush);
    DeleteObject(healthBrush);
  }
  if (currentWidth < barWidth) {
    HBRUSH lostBrush = CreateSolidBrush(RGB(255, 0, 0));
    RECT lostRect = {barX + currentWidth, barY, barX + barWidth,
                     barY + barHeight};
    FillRect(context, &lostRect, lostBrush);
    DeleteObject(lostBrush);
  }

  HPEN borderPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
  SelectObject(context, borderPen);
  SelectObject(context, (HBRUSH)GetStockObject(NULL_BRUSH));
  Rectangle(context, barX, barY, barX + barWidth, barY + barHeight);
  DeleteObject(borderPen);

  // Текстовая информация об уровне и прогрессе
  // Устанавливаем белый цвет текста и прозрачный фон
  SetBkMode(context, TRANSPARENT);
  SetTextColor(context, RGB(255, 255, 255));

  char buf[256];
  if (level == 1) {
    sprintf(buf, "Level 1: Kill %d normal towers. Killed: %d", targetNormalLvl1,
            normalKilled);
  } else if (level == 2) {
    sprintf(buf,
            "Level 2: Kill %d normal & %d magic towers. Normal: %d, Magic: %d",
            targetNormalLvl2, targetMagicLvl2, normalKilled, magicKilled);
  } else {
    sprintf(buf, "Level 3: Infinite mode! Keep going!");
  }
  TextOut(context, 10, 10, buf, strlen(buf));
}

// --------------------- ГЛАВНАЯ ФУНКЦИЯ ---------------------

int main() {
  srand(static_cast<unsigned int>(time(nullptr)));

  HWND hwnd = GetConsoleWindow();
  if (hwnd == NULL) {
    std::cout << "Console Window NOT FOUND\n";
    return 1;
  }

  HDC hdcScreen = GetWindowDC(hwnd);
  if (hdcScreen == 0) {
    std::cout << "HDC NOT FOUND\n";
    return 2;
  }
  hdc = hdcScreen;

  SetWindowPos(hwnd, NULL, windowX, windowY, windowW, windowH, 0);
  Sleep(100);

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
  int step = 5;
  auto lastTime = std::chrono::steady_clock::now();
  auto rocketLastTime = std::chrono::steady_clock::now();
  const std::chrono::milliseconds frameDuration(1000 / GAME_FPS);

  Plane *currentPlane = planes[1]; // AttackPlane
  initializeTowers();

  // Зоны – координаты подобраны так, чтобы не перекрывать HUD и границы
  RepairZone *repair = new RepairZone(30, 30);
  RocketsFill *rocketsBonus =
      new RocketsFill(playAreaRight - 130, 80); // смещена вниз

  RECT playAreaRect = {playAreaLeft, playAreaTop, playAreaRight,
                       playAreaBottom};

  // Сброс состояния уровней
  currentLevel = 1;
  killedNormal = 0;
  killedMagic = 0;
  bonusesEnabled = false;
  if (star) {
    delete star;
    star = nullptr;
  }
  magicActive = false;

  // --------------------- ГЛАВНЫЙ ЦИКЛ ---------------------
  while (gameRunning) {
    auto now = std::chrono::steady_clock::now();
    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTime);
    auto rocketElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - rocketLastTime);

    if (KEY_DOWN(VK_ESCAPE))
      gameRunning = false;

    if (elapsed >= frameDuration) {
      lastTime = now;

      // Переключаем глобальный hdc на буфер
      hdc = hdcFrame;

      // Копируем статический фон
      BitBlt(hdcFrame, 0, 0, bufW, bufH, hdcBack, 0, 0, SRCCOPY);

      // ---------- ЛОГИКА ----------
      // Движение самолёта
      directions pressed = keyHandler();
      moveObject(currentPlane, pressed, step);
      planeOutbound(currentPlane);

      // Ракеты
      if (rocketElapsed >= rocketDelay && KEY_DOWN(VK_SPACE) &&
          (currentPlane->getID() == 1 || currentPlane->getID() == 2)) {
        currentPlane->shoot();
        rocketLastTime = now;
      }
      if (currentPlane->getID() == 1 || currentPlane->getID() == 2)
        updateRockets(currentPlane);

      // Башни
      checkTowerSpawn();
      updateTowers();

      // Столкновения с башнями
      bool collisionThisFrame = false;
      int lastX = currentPlane->getX();
      int lastY = currentPlane->getY();
      int newID = checkPlaneTowerCollisions(currentPlane, collisionThisFrame);
      if (newID != currentPlane->getID()) {
        currentPlane->hide();
        currentPlane = planes[newID];
        currentPlane->moveTo(lastX, lastY);
      }

      // Зона починки – чинит только сломанный (ID=3)
      if (currentPlane->getID() == 3 && isCollision(currentPlane, repair)) {
        int lx = currentPlane->getX();
        int ly = currentPlane->getY();
        currentPlane->hide();
        currentPlane = planes[1]; // AttackPlane
        currentPlane->moveTo(lx, ly);
        magicActive = false;
      }

      // Бонус пополнения ракет – ДОСТУПЕН НА ВСЕХ УРОВНЯХ
      if ((currentPlane->getID() == 1 || currentPlane->getID() == 2) &&
          isCollision(currentPlane, rocketsBonus)) {
        currentPlane->setRockets(1);
      }

      // Бонус – звезда (только на уровнях 2 и 3)
      if (bonusesEnabled) {
        auto starElapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(now -
                                                                  starSpawnTime)
                .count();
        if (star == nullptr && starElapsed >= STAR_SPAWN_INTERVAL) {
          int x = playAreaLeft + rand() % (playAreaRight - playAreaLeft - 30);
          star = new StarBonus(x, playAreaTop);
          star->show();
          starSpawnTime = now;
        }
        if (star) {
          star->moveDown();
          if (star->getY() > playAreaBottom) {
            star->hide();
            delete star;
            star = nullptr;
          } else if (isCollision(currentPlane, star)) {
            star->hide();
            delete star;
            star = nullptr;
            if (currentPlane->getID() != 2) {
              int lx = currentPlane->getX();
              int ly = currentPlane->getY();
              currentPlane->hide();
              currentPlane = planes[2]; // MagicPlane
              currentPlane->moveTo(lx, ly);
              magicActive = true;
              magicEndTime = now + std::chrono::milliseconds(MAGIC_DURATION_MS);
            }
          }
        }

        // Проверка окончания волшебства
        if (magicActive && currentPlane->getID() == 2) {
          if (now >= magicEndTime) {
            int lx = currentPlane->getX();
            int ly = currentPlane->getY();
            currentPlane->hide();
            currentPlane = planes[1]; // обратно на AttackPlane
            currentPlane->moveTo(lx, ly);
            magicActive = false;
          }
        }
      }

      // ---------- ОТРИСОВКА ----------
      // Зоны – всегда рисуем, даже если бонусы отключены (ракетный бонус всегда
      // виден)
      repair->show();
      rocketsBonus->show(); // теперь всегда показываем

      // Башни
      for (int i = 0; i < TOWERS_NUM; i++) {
        if (towers[i])
          towers[i]->show();
      }

      // Ракеты (только видимые)
      if (currentPlane->getID() == 1 || currentPlane->getID() == 2) {
        for (int i = 0; i < 2; i++) {
          Rockets *r = currentPlane->getr(i);
          if (r && r->getVisible())
            r->show();
        }
      }

      // Самолёт
      currentPlane->show();

      // HUD (текст и полоска)
      drawHUD(hdcFrame, planeHealth, PLANE_HEALTH, currentLevel, killedNormal,
              killedMagic);

      // Эффект столкновения
      if (collisionThisFrame) {
        HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
        FillRect(hdcFrame, &playAreaRect, redBrush);
        DeleteObject(redBrush);
      }

      // Копируем кадр на экран
      hdc = hdcScreen;
      BitBlt(hdcScreen, 0, 0, bufW, bufH, hdcFrame, 0, 0, SRCCOPY);

      if (collisionThisFrame) {
        Sleep(500);
      }

      hdc = hdcFrame; // возвращаем на буфер
    } else {
      Sleep(1);
    }
  }

  // --------------------- ОСВОБОЖДЕНИЕ РЕСУРСОВ ---------------------
  delete repair;
  delete rocketsBonus;
  if (star)
    delete star;
  for (int i = 0; i < TOWERS_NUM; i++)
    delete towers[i];
  for (int i = 0; i < sizeof(planes) / sizeof(planes[0]); i++)
    delete planes[i];

  SelectObject(hdcBack, oldBackBmp);
  DeleteObject(backBmp);
  DeleteDC(hdcBack);

  SelectObject(hdcFrame, oldFrameBmp);
  DeleteObject(frameBmp);
  DeleteDC(hdcFrame);

  ReleaseDC(hwnd, hdcScreen);
  return 0;
}