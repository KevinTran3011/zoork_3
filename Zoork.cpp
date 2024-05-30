#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>

// Forward declarations to resolve dependencies
class Room;
class Command;
class Enemy;
class Player;

// Command.h
class Command
{
public:
    virtual ~Command() {}
    virtual void execute() = 0;
};

// GameObject.h
class GameObject
{
protected:
    std::string name;
    std::string description;

public:
    GameObject(const std::string &n, const std::string &d) : name(n), description(d) {}
    virtual ~GameObject() {}

    void setName(const std::string &s)
    {
        name = s;
    }

    std::string getName() const
    {
        return name;
    }

    void setDescription(const std::string &s)
    {
        description = s;
    }

    std::string getDescription() const
    {
        return description;
    }
};

// RoomDefaultEnterCommand.h
class RoomDefaultEnterCommand : public Command
{
private:
    Room *room;

public:
    RoomDefaultEnterCommand(Room *r) : room(r) {}

    void execute() override;
};

// Location.h
class Location : public GameObject
{
protected:
    std::vector<GameObject *> items;
    std::shared_ptr<Command> enterCommand;

public:
    Location(const std::string &n, const std::string &desc) : GameObject(n, desc) {}
    Location(const std::string &n, const std::string &desc, std::shared_ptr<Command> cmd) : GameObject(n, desc), enterCommand(std::move(cmd)) {}

    void addItem(GameObject *item)
    {
        items.push_back(item);
    }

    std::vector<GameObject *> &getItems()
    {
        return items;
    }

    void setEnterCommand(std::shared_ptr<Command> cmd)
    {
        enterCommand = std::move(cmd);
    }

    void enter()
    {
        if (enterCommand)
        {
            enterCommand->execute();
        }
    }
};

// Room.h
class Room : public Location
{
private:
    std::map<std::string, Room *> exits;

public:
    Room(const std::string &n, const std::string &d);
    Room(const std::string &n, const std::string &d, std::shared_ptr<Command> c);

    void setExit(const std::string &direction, Room *neighbor);
    Room *getExit(const std::string &direction);
    std::string getExitString();
};

// Room.cpp
void RoomDefaultEnterCommand::execute()
{
    std::cout << "You have entered " << room->getDescription() << std::endl;
}

Room::Room(const std::string &n, const std::string &d) : Location(n, d)
{
    enterCommand = std::make_shared<RoomDefaultEnterCommand>(this);
}

Room::Room(const std::string &n, const std::string &d, std::shared_ptr<Command> c) : Location(n, d, std::move(c)) {}

void Room::setExit(const std::string &direction, Room *neighbor)
{
    exits[direction] = neighbor;
}

Room *Room::getExit(const std::string &direction)
{
    if (exits.find(direction) != exits.end())
    {
        return exits[direction];
    }
    else
    {
        return nullptr;
    }
}

std::string Room::getExitString()
{
    std::string exitString = "Exits: ";
    for (auto exit : exits)
    {
        exitString += exit.first + " ";
    }
    return exitString;
}

// Passage.h
class Passage : public Command
{
protected:
    Room *destination;

public:
    Passage(Room *dest) : destination(dest) {}
    virtual void enter() = 0;
};

// NullPassage.h
class NullPassage : public Passage
{
public:
    NullPassage(Room *r) : Passage(r) {}

    void enter() override
    {
        std::cout << "This passage does not exist." << std::endl;
    }
};

// PassageDefaultEnterCommand.h
class PassageDefaultEnterCommand : public Command
{
private:
    Room *room;

public:
    PassageDefaultEnterCommand(Room *r) : room(r) {}

    void execute() override
    {
        std::cout << "You have entered " << room->getDescription() << std::endl;
    }
};

// Observer.h
class Observer
{
public:
    virtual ~Observer() {}
    virtual void update() = 0;
};

// Item.h
class Item : public GameObject
{
public:
    enum ItemType
    {
        WEAPON,
        POTION
    };

private:
    ItemType type;

public:
    Item(const std::string &n, const std::string &d, ItemType t) : GameObject(n, d), type(t) {}

    ItemType getType() const
    {
        return type;
    }

    virtual void use(Player &player) = 0;
};

// Weapon.h
class Weapon : public Item
{
public:
    Weapon(const std::string &n, const std::string &d) : Item(n, d, Item::WEAPON) {}
    virtual ~Weapon() {}

    virtual std::string getDescription() const = 0;
    virtual int getDamage() const = 0;

    void use(Player &player) override;
};

class Sword : public Weapon
{
public:
    Sword() : Weapon("Sword", "A sharp blade.") {}
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
    Bow() : Weapon("Bow", "A ranged weapon.") {}
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
    Dagger() : Weapon("Dagger", "A small but deadly weapon.") {}
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
    WeaponDecorator(Weapon *weapon) : Weapon(weapon->getName(), weapon->getDescription()), wrappedWeapon(weapon) {}
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

// HealthPotion.h
class HealthPotion : public Item
{
private:
    int healingAmount;

public:
    HealthPotion(const std::string &n, const std::string &d, int h) : Item(n, d, Item::POTION), healingAmount(h) {}

    int getHealingAmount() const
    {
        return healingAmount;
    }

    void use(Player &player) override;
};

// Enemy.h
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

// Player.h
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

    void gainXP(int amount);
    void levelUp();
    void update() override;
    void equipWeapon(Weapon *weapon);
    void addItem(Item *item);
    void removeItem(Item *item);
    int getLevel() const;
    int getHealth() const;
    void takeDamage(int damage);
    void heal(int amount);
    void attack(Enemy *enemy);
    Weapon *getEquippedWeapon() const;
    std::vector<Item *> &getInventory();
    void resetXP();
};

// Player.cpp
void Player::gainXP(int amount)
{
    xp += amount;
    if (xp >= 10)
    {
        levelUp();
    }
}

void Player::levelUp()
{
    level++;
    xp = 0;
    update();
}

void Player::update()
{
    if (level >= 5 && equippedWeapon)
    {
        equippedWeapon = new AwakenedWeapon(equippedWeapon);
    }
}

void Player::equipWeapon(Weapon *weapon)
{
    equippedWeapon = weapon;
}

void Player::addItem(Item *item)
{
    inventory.push_back(item);
}

void Player::removeItem(Item *item)
{
    auto it = std::find(inventory.begin(), inventory.end(), item);
    if (it != inventory.end())
    {
        inventory.erase(it);
    }
}

void Player::heal(int amount)
{
    health += amount;
    if (health > 100)
    {
        health = 100;
    }
}

int Player::getLevel() const
{
    return level;
}

int Player::getHealth() const
{
    return health;
}

void Player::takeDamage(int damage)
{
    health -= damage;
    if (health < 0)
        health = 0;
}

void Player::attack(Enemy *enemy)
{
    if (equippedWeapon)
    {
        enemy->takeDamage(equippedWeapon->getDamage());
        std::cout << "You attacked the enemy with " << equippedWeapon->getDescription() << " dealing " << equippedWeapon->getDamage() << " damage." << std::endl;
    }
    else
    {
        std::cout << "You have no weapon equipped!" << std::endl;
    }
}

Weapon *Player::getEquippedWeapon() const
{
    return equippedWeapon;
}

std::vector<Item *> &Player::getInventory()
{
    return inventory;
}

void Player::resetXP()
{
    xp = 0;
}

// Boss.h
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
                player.addItem(godSlayer);
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

// ZOOrkEngine.h
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
    void handleUseCommand(const std::string &arguments);
    void movePlayer(const std::string &direction);
    void handleCheckInventoryCommand();
    void handleEnemyAttack();
    void handlePlayerAttack();
    Room *getCurrentRoom() const
    {
        return currentRoom;
    }
    bool isGameOver() const
    {
        return gameOver || player.getHealth() <= 0;
    }
};

// ZOOrkEngine.cpp
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
    arsenal->setExit("west", mainHallway);
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

    // add weapons to the arsenal
    arsenal->addItem(new Sword());
    arsenal->addItem(new Bow());
    library->addItem(new HealthPotion("HEalth Potion", "Restores 20 health.", 20));

    // Set current room to entrance
    currentRoom = entrance;

    // Initialize player
    player.addItem(new Sword());
    player.addItem(new Bow());
    player.addItem(new Dagger());
    player.addItem(new HealthPotion("Health Potion", "Restores 20 health.", 20));
    player.equipWeapon(static_cast<Weapon *>(player.getInventory()[0]));
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
    auto &items = currentRoom->getItems(); // Get a reference to the items vector
    for (auto it = items.begin(); it != items.end(); ++it)
    {
        if ((*it)->getName() == arguments)
        {
            player.addItem(static_cast<Item *>(*it));
            items.erase(it);
            std::cout << "You have taken the " << arguments << "." << std::endl;
            return;
        }
    }
    std::cout << "Item not found." << std::endl;
}

void ZOOrkEngine::handleDropCommand(const std::string &arguments)
{
    auto &inventory = player.getInventory(); // Get a reference to the inventory vector
    for (auto it = inventory.begin(); it != inventory.end(); ++it)
    {
        if ((*it)->getName() == arguments)
        {
            currentRoom->addItem(*it);
            inventory.erase(it);
            std::cout << "You have dropped the " << arguments << "." << std::endl;
            return;
        }
    }
    std::cout << "Item not found." << std::endl;
}

void ZOOrkEngine::handleCheckInventoryCommand()
{
    std::cout << "Inventory: ";
    for (auto item : player.getInventory())
    {
        std::cout << item->getName() << " ";
    }
    std::cout << std::endl;
}

void ZOOrkEngine::handleUseCommand(const std::string &arguments)
{
    auto &inventory = player.getInventory(); // Get a reference to the inventory vector
    for (auto it = inventory.begin(); it != inventory.end(); ++it)
    {
        if ((*it)->getName() == arguments)
        {
            (*it)->use(player);
            player.removeItem(*it);
            delete *it; // Ensure to delete the item to prevent memory leaks
            return;
        }
    }
    std::cout << "Item not found or cannot be used." << std::endl;
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
        currentRoom->enter();
    }
}

void ZOOrkEngine::handlePlayerAttack()
{
    if (player.getEquippedWeapon())
    {
        player.attack(&enemy);
        enemy.takeDamage(player.getEquippedWeapon()->getDamage());
        if (enemy.getHealth() <= 0)
        {
            std::cout << "You have defeated the enemy!" << std::endl;
            player.gainXP(5);
            std::cout << "You gained 5 XP. You now have " << player.getLevel() << " level." << std::endl;
        }
        else
        {
            handleEnemyAttack();
        }
    }
    else
    {
        std::cout << "You have no weapon equipped!" << std::endl;
    }
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

// GameFacade.h
class GameFacade
{
private:
    ZOOrkEngine *engine;

public:
    GameFacade()
    {
        engine = new ZOOrkEngine();
    }

    ~GameFacade()
    {
        delete engine;
    }

    void startGame()
    {
        engine->initializeGame();
    }

    void processCommand(const std::string &command);
    bool isGameOver();
    void displayCurrentRoom();
};

// GameFacade.cpp
void GameFacade::processCommand(const std::string &command)
{
    if (command == "look")
    {
        engine->handleLookCommand("");
    }
    else if (command.find("go ") == 0)
    {
        std::string direction = command.substr(3);
        engine->movePlayer(direction);
    }
    else if (command.find("take ") == 0)
    {
        std::string item = command.substr(5);
        engine->handleTakeCommand(item);
    }
    else if (command.find("drop ") == 0)
    {
        std::string item = command.substr(5);
        engine->handleDropCommand(item);
    }
    else if (command == "attack")
    {
        engine->handlePlayerAttack();
    }
    else if (command.find("use ") == 0)
    {
        std::string item = command.substr(4);
        engine->handleUseCommand(item);
    }
    else if (command == "check inventory")
    {
        engine->handleCheckInventoryCommand();
    }
    else
    {
        std::cout << "I don't understand that command." << std::endl;
    }
}

bool GameFacade::isGameOver()
{
    return engine->isGameOver();
}

void GameFacade::displayCurrentRoom()
{
    std::cout << "You are in " << engine->getCurrentRoom()->getDescription() << std::endl;
    std::cout << engine->getCurrentRoom()->getExitString() << std::endl;
}

// Main.cpp
int main()
{
    GameFacade game;
    game.startGame();

    std::string command;
    while (!game.isGameOver())
    {
        game.displayCurrentRoom();
        std::cout << "> ";
        std::getline(std::cin, command);

        if (command == "quit")
        {
            break;
        }
        else
        {
            game.processCommand(command);
        }
    }

    if (game.isGameOver())
    {
        std::cout << "Game Over!" << std::endl;
    }

    return 0;
}

// Implementations for methods declared but not defined in header files
void Weapon::use(Player &player)
{
    player.equipWeapon(this);
    std::cout << "You equipped the " << getDescription() << "." << std::endl;
}

void HealthPotion::use(Player &player)
{
    player.heal(healingAmount);
    std::cout << "You used a " << getDescription() << " and restored " << healingAmount << " health." << std::endl;
}
