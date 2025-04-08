#ifndef SWORD_H
#define SWORD_H

#include "item.h"

class Sword : public Item {
public:
    Sword(const std::string& name, const std::string& description, int damage, Entity* parent = nullptr);
    virtual void Use() override;
    int GetDamage() const;

private:
    int damage;
};

#endif // SWORD_H

