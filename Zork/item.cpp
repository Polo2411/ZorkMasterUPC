#include "item.h"
#include "room.h"
#include <iostream>

Item::Item(const std::string& name,
    const std::string& description,
    const std::string& direction,
    Entity* parent)
    : Entity(name, description, parent), direction(direction)
{
    type = ITEM;
    // Eliminamos el autoinsertado en la Room que estaba aquí.
    // Ahora NO hacemos nada con room->AddEntity(...)
    // Serás tú quien lo haga manualmente en world.cpp
}

void Item::Use() {
    std::cout << "Using " << name << "\n";
}

std::string Item::GetDirection() const {
    return direction;
}
