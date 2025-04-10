// upgrader.cpp
#include "upgrader.h"

Upgrader::Upgrader(const std::string& name,
    const std::string& description,
    const std::string& direction,
    Entity* parent)
    : Item(name, description, direction, parent)
{
}
