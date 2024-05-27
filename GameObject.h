#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>

class GameObject
{
protected:
    std::string name;
    std::string description;

public:
    GameObject(const std::string &n, const std::string &d) : name(n), description(d) {}
    virtual ~GameObject() {}

    void setName(const std::string &s) { name = s; }
    std::string getName() const { return name; }

    void setDescription(const std::string &s) { description = s; }
    std::string getDescription() const { return description; }
};

#endif // GAMEOBJECT_H
