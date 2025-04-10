#ifndef ENEMY_H
#define ENEMY_H

#include "creature.h"
#include "player.h"
#include <string>

// Base Enemy
class Enemy : public Creature {
public:
    Enemy(const std::string& name, const std::string& description, Room* startRoom);
    virtual ~Enemy() = default;

    virtual void Attack(Creature* target) = 0; // Daño dif. en subclases
    virtual void TakeDamage(int dmg);
    virtual void Update(Player* player); // IA

protected:
    
};

// Demon
class Demon : public Enemy {
public:
    Demon(const std::string& name, const std::string& description, Room* startRoom);
    virtual void Attack(Creature* target) override;
};

// DemonKnight
class DemonKnight : public Enemy {
public:
    DemonKnight(const std::string& name, const std::string& description, Room* startRoom);
    virtual void Attack(Creature* target) override;
};

// Vergil con su minimecánica
class Vergil : public Enemy {
public:
    Vergil(const std::string& name, const std::string& description, Room* startRoom);

    virtual void Attack(Creature* target) override;
    virtual void Update(Player* player) override; // su IA

private:
    int consecutiveAttacks;
    bool inDevilTrigger;
    int devilTriggerRounds;
};

#endif
