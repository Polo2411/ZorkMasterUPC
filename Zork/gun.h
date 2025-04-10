#ifndef GUN_H
#define GUN_H

#include "item.h"

// Gun hereda de Item, pero posee un contador de balas y daño.
class Gun : public Item {
public:
    Gun(const std::string& name,
        const std::string& description,
        int damage,
        const std::string& direction,
        Entity* parent = nullptr);

    int  GetDamage() const;
    void SetDamage(int dmg);

    int  GetAmmoCount() const;
    void AddAmmo(int amount);

private:
    int damage;
    int ammoCount;  // Contador de balas
};

#endif // GUN_H
