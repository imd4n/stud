#pragma once

#include "interfaces.h"

class Location {
protected:
    int X, Y;
public:
    Location(int InitX = 0, int InitY = 0);
    virtual ~Location();
    int GetX() const;
    int GetY() const;
    void SetX(int NewX);
    void SetY(int NewY);
    void SetPosition(int NewX, int NewY);
};

class Point : public Location, public IDisplayable, public IMovable {
protected:
    bool Visible;
public:
    Point(int InitX = 0, int InitY = 0);
    virtual ~Point();
    bool IsVisible() const;
    virtual void Show();
    virtual void Hide();
    virtual void MoveTo(int NewX, int NewY);
    virtual void Drag(int Step);
};

// Tank now implements IDamageable and IHealable
class Tank : public Point, public IInteractable, public IDamageable, public IHealable {
protected:
    int bodyWidth, bodyHeight, turretWidth, turretHeight;
    int gunLength, trackHeight;
    int health;
    int speed;
    bool isDestroyed;
    // Для исчезновения пушки, гусениц
    bool lostGun;
    bool lostTracks;
public:
    Tank(int InitX, int InitY, int bW, int bH, int tW, int tH, int gL, int trH, int spd = 30);
    virtual ~Tank();
    virtual void Show();
    virtual void Hide();
    virtual void Drag(int step);
    virtual void TakeDamage(int dmg);
    virtual void Heal(int amount);
    void Explode();

    virtual bool CheckCollision(Tank* tank) { return false; }
    virtual void OnCollision(Tank* tank);
    virtual bool IsActive() const { return !isDestroyed; }
    virtual bool IsDestroyed() const;

    int GetHealth() const;
    int GetSpeed() const;
    int GetBodyWidth() const;
    int GetBodyHeight() const;

    // Для исчезновения деталей
    virtual void LosePartOnMine();        // исчезает пушка
    virtual void LosePartOnHit();         // исчезают гусеницы
    virtual bool ShouldLoseGun() const;   // исчезает пушка
    virtual bool ShouldLoseTracks() const;// исчезают гусеницы
    virtual bool ShouldLoseTurret() const;// для DamageTank
};

class HeavyTank : public Tank {
private:
    int targetX, targetY;
    bool lostGun;
public:
    HeavyTank(int InitX, int InitY);
    ~HeavyTank();
    void Show();
    void Hide();
    virtual void TakeDamage(int dmg);
    void SetTarget(int x, int y);
    void ShowTargetingSystem();
    void LosePartOnMine() override { lostGun = true; }
    void LosePartOnHit() override { lostGun = true; }
    bool ShouldLoseGun() const override { return lostGun; }
};
// НОВЫЙ ТАНК
class RoundHeavyTank : public HeavyTank {
public:
    RoundHeavyTank(int InitX, int InitY);
    ~RoundHeavyTank();
    virtual void Show() override;
    virtual void Hide() override;
};

class LightTank : public Tank {
private:
    bool lostTracks;
public:
    LightTank(int InitX, int InitY);
    ~LightTank();
    virtual void Show();
    virtual void TakeDamage(int dmg);
    void LosePartOnMine() override { lostTracks = true; }
    void LosePartOnHit() override { lostTracks = true; }
    bool ShouldLoseTracks() const override { return lostTracks; }
};

class DamageTank : public Tank {
private:
    bool lostTurret;
public:
    DamageTank(int InitX, int InitY);
    ~DamageTank();
    virtual void Show();
    virtual void TakeDamage(int dmg);
    void LosePartOnMine() override { lostTurret = true; }
    void LosePartOnHit() override { lostTurret = true; }
    bool ShouldLoseTurret() const override { return lostTurret; }
};

class Rocket : public Tank {
private:
    int power;
    int directionX, directionY;
    bool isActive;
    int targetX, targetY;
public:
    Rocket(int InitX, int InitY, int targetX, int targetY, int pwr, int w = 36, int h = 12, int spd = 15);
    ~Rocket();
    virtual void Show();
    virtual void Hide();
    virtual void MoveTo(int NewX, int NewY);

    virtual bool CheckCollision(Tank* tank);
    virtual void OnCollision(Tank* tank);
    virtual bool IsActive() const { return isActive; }

    void MoveToTarget();
    int GetPower() const;
    void Deactivate();
};

class Mina : public Tank {
private:
    int damage;
    bool isExploded;
public:
    Mina(int InitX, int InitY, int dmg = 50, int w = 20, int h = 20);
    ~Mina();
    virtual void Show();
    virtual void Hide();

    virtual bool CheckCollision(Tank* tank);
    virtual void OnCollision(Tank* tank);
    virtual bool IsActive() const { return !isExploded; }

    void Explode();
    int GetDamage() const;
    bool IsExploded() const;
};

class Remont : public Tank {
private:
    int healAmount;
    bool isUsed;
public:
    Remont(int InitX, int InitY, int heal = 30, int w = 40, int h = 30);
    ~Remont();
    virtual void Show();
    virtual void Hide();

    virtual bool CheckCollision(Tank* tank);
    virtual void OnCollision(Tank* tank);
    virtual bool IsActive() const { return !isUsed; }

    void Use();
    int GetHealAmount() const;
    bool IsUsed() const;
};

void CheckCollisions(Tank* activeTank, IInteractable** objects, int objectCount);
