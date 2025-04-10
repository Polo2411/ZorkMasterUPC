#ifndef SWORD_H
#define SWORD_H

#include "item.h"

class Sword : public Item {
public:
    Sword(const std::string& name,
        const std::string& description,
        int damage,
        const std::string& direction,
        Entity* parent = nullptr);

    virtual void Use() override;

    int  GetDamage() const;
    void SetDamage(int dmg); // NUEVO

private:
    int damage;
};

#endif // SWORD_H
