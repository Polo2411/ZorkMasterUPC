#include "item.h"
#include <iostream>

Item::Item(const std::string& name, const std::string& description, const std::string& direction, Entity* parent)
    : Entity(name, description, parent), direction(direction)
{
    type = ITEM;
}

void Item::Use() {
    std::cout << "Using " << name << "\n";
}

std::string Item::GetDirection() const {
    return direction;
}
