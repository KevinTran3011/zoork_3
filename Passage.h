#ifndef PASSAGE_H
#define PASSAGE_H

#include "Command.h"

class Room;

class Passage : public Command
{
protected:
    Room *destination;

public:
    Passage(Room *dest) : destination(dest) {}
    virtual void enter() = 0;
};

#endif // PASSAGE_H
