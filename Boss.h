#ifndef BOSS_H
#define BOSS_H

#include "Player.h"

class Boss
{
protected:
    int health;

public:
    virtual ~Boss() {}
    virtual void fight(Player &player) = 0;
};

class HiddenBoss : public Boss
{
public:
    HiddenBoss() { health = 100; }
    void fight(Player &player) override
    {
        if (player.getLevel() < 5)
        {
            player.takeDamage(player.getHealth() - 1);
            // Logic to teleport player to a room full of monsters can be added here
        }
        else
        {
            // Fight the beast and then the hidden boss
            player.takeDamage(20); // Example damage from the beast
            if (player.getHealth() > 0)
            {
                // Player wins and receives the god slayer blade
                Weapon *godSlayer = new Sword(); // Replace with actual god slayer blade implementation
                player.addItem(new Item("God Slayer", "A blade capable of one-shotting any enemy."));
                player.equipWeapon(godSlayer);
            }
        }
    }
};

class BossProxy : public Boss
{
private:
    HiddenBoss *realBoss;
    Player &player;

public:
    BossProxy(HiddenBoss *boss, Player &player) : realBoss(boss), player(player) {}

    void fight(Player &player) override
    {
        realBoss->fight(player);
    }
};

#endif // BOSS_H
