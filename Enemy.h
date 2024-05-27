#ifndef ENEMY_H
#define ENEMY_H

class Enemy
{
private:
    int health;
    int damage;

public:
    Enemy() : health(5), damage(5) {}
    Enemy(int h, int d) : health(h), damage(d) {}

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

    int getDamage() const
    {
        return damage;
    }
};

#endif // ENEMY_H
