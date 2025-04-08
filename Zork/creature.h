#ifndef CREATURE_H
#define CREATURE_H

#include "entity.h"
#include "room.h"
#include "item.h"
#include <vector>

class Creature : public Entity {
public:
    Creature(const std::string& name, const std::string& description, Room* startRoom);
    virtual ~Creature() = default;

    void SetHealth(int hp);
    int GetHealth() const;
    bool IsAlive() const;

    void AddItem(Item* itm);
    void RemoveItem(Item* itm);
    Room* GetLocation() const;
    void SetLocation(Room* newRoom);

protected:
    int health;
    Room* location;
    std::vector<Item*> inventory;
};

#endif // CREATURE_H

