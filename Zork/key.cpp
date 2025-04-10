#include "key.h"
#include <iostream>

Key::Key(const std::string& name,
    const std::string& description,
    const std::string& direction,
    Room* opensRoom,
    Entity* parent)
    : Item(name, description, direction, parent)
    , opensRoom(opensRoom)
{
}

void Key::Use() {
    std::cout << "You use the " << name << ".\n";
}

Room* Key::GetOpensRoom() const {
    return opensRoom;
}

// NUEVO: SetOpensRoom
void Key::SetOpensRoom(Room* r) {
    opensRoom = r;
}
