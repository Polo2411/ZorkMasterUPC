#ifndef ITEM_H
#define ITEM_H

#include "entity.h"

class Item : public Entity {
public:
    Item(const std::string& name, const std::string& description, Entity* parent = nullptr);
    virtual void Use();
};

#endif // ITEM_H
