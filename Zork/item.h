#ifndef ITEM_H
#define ITEM_H

#include "entity.h"
#include <string>

class Item : public Entity {
public:
    Item(const std::string& name,
        const std::string& description,
        const std::string& direction,
        Entity* parent = nullptr);

    virtual void Use();
    std::string GetDirection() const;

protected:
    std::string direction;
};

#endif // ITEM_H
