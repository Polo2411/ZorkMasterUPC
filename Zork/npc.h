#ifndef NPC_H
#define NPC_H

#include "creature.h"

class NPC : public Creature {
public:
    NPC(const std::string& name, const std::string& description, Room* startRoom);
    virtual ~NPC() = default;
    virtual void Talk();
};

#endif // NPC_H

