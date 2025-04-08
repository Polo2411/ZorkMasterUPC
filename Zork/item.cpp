#include "item.h"
#include <iostream>

Item::Item(const std::string& name, const std::string& description, Entity* parent)
    : Entity(name, description, parent)
{
    type = ITEM;
}

void Item::Use() {
    std::cout << "Using " << name << "\n";
}
