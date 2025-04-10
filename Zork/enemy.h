#ifndef ENEMY_H
#define ENEMY_H

#include "creature.h"
#include "player.h"  // Para usar Player* en Update

class Enemy : public Creature {
public:
    Enemy(const std::string& name, const std::string& description, Room* startRoom);
    virtual ~Enemy() = default;

    // Cada subclase definirá su daño en Attack
    virtual void Attack(Creature* target) = 0;
    virtual void TakeDamage(int dmg);
    virtual void Update(Player* player);

protected:
    // Tus subclases podrían usar un int damage;
};

class Demon : public Enemy {
public:
    Demon(const std::string& name, const std::string& description, Room* startRoom);
    virtual ~Demon() = default;

    virtual void Attack(Creature* target) override;
};

class DemonKnight : public Enemy {
public:
    DemonKnight(const std::string& name, const std::string& description, Room* startRoom);
    virtual ~DemonKnight() = default;

    virtual void Attack(Creature* target) override;
};

#endif // ENEMY_H
