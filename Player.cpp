#include "Player.h"
#include <iostream>

Player::Player() : level(1), health(100), xp(0), equippedWeapon(nullptr) {}

Player::~Player() {
    for (auto item : inventory) {
        delete item;
    }
    delete equippedWeapon;
}

void Player::gainXP(int amount) {
    xp += amount;
    if (xp >= 10) {
        levelUp();
    }
}

void Player::levelUp() {
    level++;
    xp = 0;
    update();
}

void Player::update() {
    if (level >= 5 && equippedWeapon) {
        equippedWeapon = new AwakenedWeapon(equippedWeapon);
    }
}

void Player::equipWeapon(Weapon *weapon) {
    equippedWeapon = weapon;
}

void Player::addItem(Item *item) {
    inventory.push_back(item);
}

int Player::getLevel() const {
    return level;
}

int Player::getHealth() const {
    return health;
}

void Player::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
}

void Player::attack(Enemy *enemy) {
    enemy->takeDamage(equippedWeapon->getDamage());
    std::cout << "You attacked the enemy with " << equippedWeapon->getDescription() << " dealing " << equippedWeapon->getDamage() << " damage." << std::endl;
}

Weapon *Player::getEquippedWeapon() const {
    return equippedWeapon;
}

std::vector<Item *> &Player::getInventory() {
    return inventory;
}

void Player::resetXP() {
    xp = 0;
}
