#ifndef HEALTH_POTION_H
#define HEALTH_POTION_H

#include "item.h"

class HealthPotion : public Item {
public:
    HealthPotion(const std::string& name,
        const std::string& description,
        int healAmount,
        const std::string& direction,
        Entity* parent = nullptr);

    virtual void Use() override;
    int GetHealAmount() const;

private:
    int healAmount;
};

#endif // HEALTH_POTION_H
