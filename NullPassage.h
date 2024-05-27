#ifndef NULLPASSAGE_H
#define NULLPASSAGE_H

#include "Passage.h"
#include <iostream>

class NullPassage : public Passage
{
public:
    NullPassage(Room *r) : Passage(r) {}

    void enter() override
    {
        std::cout << "This passage does not exist." << std::endl;
    }
};

#endif // NULLPASSAGE_H
