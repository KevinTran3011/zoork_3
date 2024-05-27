#ifndef PLAYER_H
#define PLAYER_H

#include "Observer.h"
#include "Item.h"
#include "Weapon.h"
#include "Enemy.h"
#include <vector>
#include <iostream>

class Player : public Observer
{
private:
    int level;
    int health;
    int xp;
    std::vector<Item *> inventory;
    Weapon *equippedWeapon;

public:
    Player() : level(1), health(100), xp(0), equippedWeapon(nullptr) {}

    void gainXP(int amount)
    {
        xp += amount;
        if (xp >= 10)
        {
            levelUp();
        }
    }

    void levelUp()
    {
        level++;
        xp = 0;
        update();
    }

    void update() override
    {
        if (level >= 5 && equippedWeapon)
        {
            equippedWeapon = new AwakenedWeapon(equippedWeapon);
        }
    }

    void equipWeapon(Weapon *weapon)
    {
        equippedWeapon = weapon;
    }

    void addItem(Item *item)
    {
        inventory.push_back(item);
    }

    int getLevel() const
    {
        return level;
    }

    int getHealth() const
    {
        return health;
    }

    void takeDamage(int damage)
    {
        health -= damage;
        if (health < 0)
            health = 0;
    }

    void attack(Enemy *enemy)
    {
        enemy->takeDamage(equippedWeapon->getDamage());
        std::cout << "You attacked the enemy with " << equippedWeapon->getDescription() << " dealing " << equippedWeapon->getDamage() << " damage." << std::endl;
    }

    Weapon *getEquippedWeapon() const
    {
        return equippedWeapon;
    }

    std::vector<Item *> &getInventory()
    {
        return inventory;
    }

    void resetXP()
    {
        xp = 0;
    }
};

#endif // PLAYER_H
