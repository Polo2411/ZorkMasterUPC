#ifndef ENEMY_H
#define ENEMY_H

#include "creature.h"

// Clase base Enemy: cualquier enemigo genérico
class Enemy : public Creature {
public:
    Enemy(const std::string& name, const std::string& description, Room* startRoom);
    virtual ~Enemy() = default;

    // Método de ataque genérico (daño base = 10, por ejemplo).
    virtual void Attack(Creature* target);
    // Recibir daño genérico.
    virtual void TakeDamage(int dmg);

    // Se llamará después de cada comando del jugador (en cada "turno")
    // para que el enemigo decida moverse o atacar.
    virtual void Update(class Player* player);
};

// Subclase Demon: un enemigo con menos HP pero que ataca rápido
class Demon : public Enemy {
public:
    Demon(const std::string& name, const std::string& description, Room* startRoom);
    virtual ~Demon() = default;

    // Podríamos sobreescribir Attack si queremos, 
    // pero si solo cambiamos stats podemos usarlo tal cual de la clase Enemy.
};

// Subclase DemonKnight: un enemigo más fuerte, con más HP y más daño
class DemonKnight : public Enemy {
public:
    DemonKnight(const std::string& name, const std::string& description, Room* startRoom);
    virtual ~DemonKnight() = default;

    // Podríamos sobreescribir Attack para más daño, o usar 
    // una variable de daño para personalizar la subclase.
};

#endif // ENEMY_H
