#include "room.h"
#include <iostream>

Room::Room(const std::string& name, const std::string& description, Entity* parent)
    : Entity(name, description, parent)
{
    type = ROOM;
}

void Room::Connect(const std::string& direction, Room* otherRoom) {
    connections[direction] = otherRoom;
}

Room* Room::GetConnectedRoom(const std::string& direction) const {
    auto it = connections.find(direction);
    return (it != connections.end()) ? it->second : nullptr;
}

void Room::Look() const {
    Entity::Look();
    std::cout << "Available connections:\n";
    for (auto& conn : connections)
        std::cout << " " << conn.first << " -> " << conn.second->name << "\n";
}
