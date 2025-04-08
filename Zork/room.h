#ifndef ROOM_H
#define ROOM_H

#include "entity.h"
#include <map>
#include <string>

class Room : public Entity {
public:
    Room(const std::string& name, const std::string& description, Entity* parent = nullptr);
    // Connects this room with another room in a given direction
    void Connect(const std::string& direction, Room* otherRoom);
    // Returns the room connected in the given direction (or nullptr if none)
    Room* GetConnectedRoom(const std::string& direction) const;
    virtual void Look() const override;

private:
    std::map<std::string, Room*> connections;
};

#endif // ROOM_H
