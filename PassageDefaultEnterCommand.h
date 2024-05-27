#ifndef PASSAGEDEFAULTENTERCOMMAND_H
#define PASSAGEDEFAULTENTERCOMMAND_H

#include "Command.h"
#include <iostream> // Include for std::cout and std::endl

class Room; // Forward declaration to resolve incomplete type error

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

#endif // PASSAGEDEFAULTENTERCOMMAND_H
