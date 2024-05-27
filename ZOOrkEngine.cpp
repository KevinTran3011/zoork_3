#include "ZOOrkEngine.h"
#include "Item.h"
#include "Weapon.h"
#include <iostream>

void ZOOrkEngine::initializeGame()
{
    // Create rooms
    Room *entrance = new Room("Entrance", "This is the entrance of the dungeon.");
    Room *mainHallway = new Room("Main Hallway", "A long hallway with several doors.");
    Room *enemyRoom1 = new Room("Enemy Room 1", "A room filled with enemies.");
    Room *arsenal = new Room("Arsenal", "A room filled with weapons.");
    Room *enemyRoom2 = new Room("Enemy Room 2", "Another room filled with enemies.");
    Room *passage = new Room("Passage", "A passage filled with enemies.");
    Room *library = new Room("Library", "A room filled with books.");
    Room *cathedral = new Room("Cathedral", "A large room with high ceilings.");
    Room *throneRoom = new Room("Throne Room", "The final room with the boss.");
    Room *dungeon = new Room("Dungeon", "A dark and damp dungeon.");
    Room *hiddenBossRoom = new Room("Hidden Boss Room", "A secret room with the hidden boss.");

    // Connect rooms
    entrance->setExit("north", mainHallway);
    mainHallway->setExit("south", entrance);
    mainHallway->setExit("west", enemyRoom1);
    mainHallway->setExit("east", arsenal);
    mainHallway->setExit("north", enemyRoom2);
    enemyRoom2->setExit("south", mainHallway);
    enemyRoom2->setExit("north", passage);
    passage->setExit("south", enemyRoom2);
    passage->setExit("west", library);
    passage->setExit("east", cathedral);
    library->setExit("east", passage);
    cathedral->setExit("west", passage);
    cathedral->setExit("north", throneRoom);
    throneRoom->setExit("south", cathedral);
    entrance->setExit("down", dungeon);
    dungeon->setExit("up", entrance);
    dungeon->setExit("north", hiddenBossRoom);
    hiddenBossRoom->setExit("south", dungeon);

    // Add rooms to vector for cleanup
    rooms.push_back(entrance);
    rooms.push_back(mainHallway);
    rooms.push_back(enemyRoom1);
    rooms.push_back(arsenal);
    rooms.push_back(enemyRoom2);
    rooms.push_back(passage);
    rooms.push_back(library);
    rooms.push_back(cathedral);
    rooms.push_back(throneRoom);
    rooms.push_back(dungeon);
    rooms.push_back(hiddenBossRoom);

    // Set current room to entrance
    currentRoom = entrance;

    // Initialize player
    Weapon *sword = new Sword();
    Weapon *bow = new Bow();
    Weapon *dagger = new Dagger();
    player.addItem(new Item("Sword", "A sharp blade."));
    player.addItem(new Item("Bow", "A ranged weapon."));
    player.addItem(new Item("Dagger", "A small but deadly weapon."));
    player.equipWeapon(sword);
}

void ZOOrkEngine::handleLookCommand(const std::string &arguments)
{
    if (arguments.empty())
    {
        std::cout << currentRoom->getDescription() << std::endl;
    }
    else
    {
        // Lookup item or feature in the room or player's inventory
        for (auto item : currentRoom->getItems())
        {
            if (item->getDescription() == arguments)
            {
                std::cout << item->getDescription() << std::endl;
                return;
            }
        }

        for (auto item : player.getInventory())
        {
            if (item->getDescription() == arguments)
            {
                std::cout << item->getDescription() << std::endl;
                return;
            }
        }

        std::cout << "Item not found." << std::endl;
    }
}

void ZOOrkEngine::handleTakeCommand(const std::string &arguments)
{
    // Implementation to move an item from the room to the player's inventory
    for (auto it = currentRoom->getItems().begin(); it != currentRoom->getItems().end(); ++it)
    {
        if ((*it)->getDescription() == arguments)
        {
            player.addItem(static_cast<Item *>(*it));
            currentRoom->getItems().erase(it);
            std::cout << "You have taken the " << arguments << "." << std::endl;
            return;
        }
    }
    std::cout << "Item not found." << std::endl;
}

void ZOOrkEngine::handleDropCommand(const std::string &arguments)
{
    // Implementation to move an item from the player's inventory to the room
    for (auto it = player.getInventory().begin(); it != player.getInventory().end(); ++it)
    {
        if ((*it)->getDescription() == arguments)
        {
            currentRoom->addItem(*it);
            player.getInventory().erase(it);
            std::cout << "You have dropped the " << arguments << "." << std::endl;
            return;
        }
    }
    std::cout << "Item not found." << std::endl;
}

void ZOOrkEngine::movePlayer(const std::string &direction)
{
    Room *nextRoom = currentRoom->getExit(direction);
    if (nextRoom == nullptr)
    {
        std::cout << "You can't go that way!" << std::endl;
    }
    else
    {
        currentRoom = nextRoom;
    }
}

void ZOOrkEngine::handlePlayerAttack()
{
    player.attack(&enemy); // Use the Player's attack method
}

void ZOOrkEngine::handleEnemyAttack()
{
    player.takeDamage(enemy.getDamage());
    std::cout << "You were hit by an enemy! Your health is now " << player.getHealth() << "." << std::endl;

    if (player.getHealth() <= 0)
    {
        std::cout << "Game Over! You have been defeated." << std::endl;
        gameOver = true;
    }
}
