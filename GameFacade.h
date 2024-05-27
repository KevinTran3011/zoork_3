#ifndef GAMEFACADE_H
#define GAMEFACADE_H

#include "ZOOrkEngine.h"

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

    void processCommand(const std::string &command)
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
            engine->handleEnemyAttack();
        }
        else
        {
            std::cout << "I don't understand that command." << std::endl;
        }
    }

    bool isGameOver()
    {
        return engine->isGameOver();
    }

    void displayCurrentRoom()
    {
        std::cout << "You are in " << engine->getCurrentRoom()->getDescription() << std::endl;
        std::cout << engine->getCurrentRoom()->getExitString() << std::endl;
    }
};

#endif // GAMEFACADE_H
