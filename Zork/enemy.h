#ifndef ENEMY_H
#define ENEMY_H

#include "creature.h"

class Enemy : public Creature {
public:
    Enemy(const std::string& name, const std::string& description, Room* startRoom);
    virtual ~Enemy() = default;

    virtual void Attack(Creature* target);
    virtual void TakeDamage(int dmg);
};

#endif // ENEMY_H

