// vitality_potion.h
#ifndef VITALITY_POTION_H
#define VITALITY_POTION_H

#include "item.h"

class VitalityPotion : public Item {
public:
    VitalityPotion(const std::string& name,
        const std::string& description,
        int extraMax,
        int heal,
        const std::string& direction,
        Entity* parent = nullptr);

    int GetExtraMax() const;
    int GetHealAmount() const;
private:
    int extraMax;
    int heal;
};

#endif // VITALITY_POTION_H

