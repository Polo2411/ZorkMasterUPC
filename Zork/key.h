#ifndef KEY_H
#define KEY_H

#include "item.h"
#include "room.h"  // Para usar Room*

class Key : public Item {
public:
    // Ahora se recibe además la room that the key opens.
    Key(const std::string& name, const std::string& description, const std::string& direction, Room* opensRoom, Entity* parent = nullptr);
    virtual void Use() override;
    Room* GetOpensRoom() const;

private:
    Room* opensRoom;
};

#endif // KEY_H
