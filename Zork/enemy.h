#ifndef ENEMY_H
#define ENEMY_H

#include "creature.h"

// Clase base Enemy: cualquier enemigo gen�rico
class Enemy : public Creature {
public:
    Enemy(const std::string& name, const std::string& description, Room* startRoom);
    virtual ~Enemy() = default;

    // M�todo de ataque gen�rico (da�o base = 10, por ejemplo).
    virtual void Attack(Creature* target);
    // Recibir da�o gen�rico.
    virtual void TakeDamage(int dmg);

    // Se llamar� despu�s de cada comando del jugador (en cada "turno")
    // para que el enemigo decida moverse o atacar.
    virtual void Update(class Player* player);
};

// Subclase Demon: un enemigo con menos HP pero que ataca r�pido
class Demon : public Enemy {
public:
    Demon(const std::string& name, const std::string& description, Room* startRoom);
    virtual ~Demon() = default;

    // Podr�amos sobreescribir Attack si queremos, 
    // pero si solo cambiamos stats podemos usarlo tal cual de la clase Enemy.
};

// Subclase DemonKnight: un enemigo m�s fuerte, con m�s HP y m�s da�o
class DemonKnight : public Enemy {
public:
    DemonKnight(const std::string& name, const std::string& description, Room* startRoom);
    virtual ~DemonKnight() = default;

    // Podr�amos sobreescribir Attack para m�s da�o, o usar 
    // una variable de da�o para personalizar la subclase.
};

#endif // ENEMY_H
