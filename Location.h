#ifndef LOCATION_H
#define LOCATION_H

#include "GameObject.h"
#include "Command.h"
#include <vector>
#include <memory>

class Location : public GameObject
{
protected:
    std::vector<GameObject *> items;
    std::shared_ptr<Command> enterCommand;

public:
    Location(const std::string &n, const std::string &desc) : GameObject(n, desc) {}
    Location(const std::string &n, const std::string &desc, std::shared_ptr<Command> cmd) : GameObject(n, desc), enterCommand(std::move(cmd)) {}

    void addItem(GameObject *item) { items.push_back(item); }
    std::vector<GameObject *> getItems() const { return items; }

    void setEnterCommand(std::shared_ptr<Command> cmd) { enterCommand = std::move(cmd); }
    void enter()
    {
        if (enterCommand)
        {
            enterCommand->execute();
        }
    }
};

#endif // LOCATION_H
