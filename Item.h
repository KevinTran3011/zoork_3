#ifndef ITEM_H
#define ITEM_H

#include "GameObject.h"

class Item : public GameObject
{
public:
    Item(const std::string &n, const std::string &d) : GameObject(n, d) {}
};

#endif // ITEM_H
