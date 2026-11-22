#pragma once

// Interface for objects that can be displayed
class IDisplayable {
public:
    virtual ~IDisplayable() = default;
    virtual void Show() = 0;
    virtual void Hide() = 0;
};

// Interface for objects that can move
class IMovable {
public:
    virtual ~IMovable() = default;
    virtual void MoveTo(int NewX, int NewY) = 0;
};

// Interface for objects that can be damaged
class IDamageable {
public:
    virtual ~IDamageable() = default;
    virtual void TakeDamage(int dmg) = 0;
    virtual bool IsDestroyed() const = 0;
};

// Interface for objects that can interact (already in your code)
class IInteractable {
public:
    virtual ~IInteractable() = default;
    virtual bool CheckCollision(class Tank* tank) = 0;
    virtual void OnCollision(Tank* tank) = 0;
    virtual bool IsActive() const = 0;
};

// Interface for healing objects
class IHealable {
public:
    virtual ~IHealable() = default;
    virtual void Heal(int amount) = 0;
};
