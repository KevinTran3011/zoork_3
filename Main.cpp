#include "GameFacade.h"
#include <iostream>
#include <string>

int main()
{
    GameFacade game;
    game.startGame();

    // Game loop
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
