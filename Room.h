#ifndef ROOM_H
#define ROOM_H

#include "Location.h"
#include <map>
#include <memory> // Include for std::shared_ptr

class RoomDefaultEnterCommand;

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

#endif // ROOM_H
