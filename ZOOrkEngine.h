#ifndef ZOORKENGINE_H
#define ZOORKENGINE_H

#include "Room.h"
#include "Player.h"
#include "Enemy.h"
#include <vector>

class ZOOrkEngine
{
private:
    Room *currentRoom;
    Player player;
    Enemy enemy;
    std::vector<Room *> rooms;
    bool gameOver;

public:
    ZOOrkEngine() : currentRoom(nullptr), gameOver(false) {}

    void initializeGame();
    void handleLookCommand(const std::string &arguments);
    void handleTakeCommand(const std::string &arguments);
    void handleDropCommand(const std::string &arguments);
    void movePlayer(const std::string &direction);
    void handleEnemyAttack();
    void handlePlayerAttack();
    Room *getCurrentRoom() const { return currentRoom; }
    bool isGameOver() const { return gameOver || player.getHealth() <= 0; }
};

#endif // ZOORKENGINE_H
