#ifndef KEY_H
#define KEY_H

#include "item.h"
#include "room.h"

class Key : public Item {
public:
    // La llave abre la room pasada en opensRoom (asociada a la exit cuyo destino es esa room)
    Key(const std::string& name, const std::string& description, const std::string& direction, Room* opensRoom, Entity* parent = nullptr);
    virtual void Use() override;
    Room* GetOpensRoom() const;

private:
    Room* opensRoom;
};

#endif // KEY_H
