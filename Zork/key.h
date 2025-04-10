// En key.h
#ifndef KEY_H
#define KEY_H

#include "item.h"
#include "room.h"

class Key : public Item {
public:
    Key(const std::string& name,
        const std::string& description,
        const std::string& direction,
        Room* opensRoom,
        Entity* parent = nullptr);

    virtual void Use() override;

    Room* GetOpensRoom() const;


    void SetOpensRoom(Room* r);

private:
    Room* opensRoom;
};

#endif // KEY_H
