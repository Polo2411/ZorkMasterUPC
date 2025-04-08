#include "npc.h"
#include <iostream>

NPC::NPC(const std::string& name, const std::string& description, Room* startRoom)
    : Creature(name, description, startRoom)
{
    // NPC may have different initial health or stats if needed
}

void NPC::Talk() {
    std::cout << name << " says: 'Hello, traveler.'\n";
}
