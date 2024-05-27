#include "Room.h"
#include "RoomDefaultEnterCommand.h"
#include <iostream>

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
