#ifndef WEAPON_H
#define WEAPON_H

#include <string>

class Weapon
{
public:
    virtual ~Weapon() {}
    virtual std::string getDescription() const = 0;
    virtual int getDamage() const = 0;
};

class Sword : public Weapon
{
public:
    std::string getDescription() const override
    {
        return "Sword";
    }
    int getDamage() const override
    {
        return 10;
    }
};

class Bow : public Weapon
{
public:
    std::string getDescription() const override
    {
        return "Bow";
    }
    int getDamage() const override
    {
        return 8;
    }
};

class Dagger : public Weapon
{
public:
    std::string getDescription() const override
    {
        return "Dagger";
    }
    int getDamage() const override
    {
        return 6;
    }
};

class WeaponDecorator : public Weapon
{
protected:
    Weapon *wrappedWeapon;

public:
    WeaponDecorator(Weapon *weapon) : wrappedWeapon(weapon) {}
    virtual ~WeaponDecorator()
    {
        delete wrappedWeapon;
    }
};

class AwakenedWeapon : public WeaponDecorator
{
public:
    AwakenedWeapon(Weapon *weapon) : WeaponDecorator(weapon) {}
    std::string getDescription() const override
    {
        return wrappedWeapon->getDescription() + " (Awakened)";
    }
    int getDamage() const override
    {
        return wrappedWeapon->getDamage() + 5;
    }
};

#endif // WEAPON_H
