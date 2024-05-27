#ifndef ROOMDEFAULTENTERCOMMAND_H
#define ROOMDEFAULTENTERCOMMAND_H

#include "Command.h"

class Room;

class RoomDefaultEnterCommand : public Command
{
private:
    Room *room;

public:
    RoomDefaultEnterCommand(Room *r) : room(r) {}

    void execute() override;
};

#endif // ROOMDEFAULTENTERCOMMAND_H
