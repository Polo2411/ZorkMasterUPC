#ifndef WORLD_H
#define WORLD_H

#include <string>
#include <vector>
#include "room.h"
#include "player.h"
#include "enemy.h"

// Forward declarations si quieres
// class Gun; class Bullet; class VitalityPotion; class Upgrader;

class World {
public:
    World();
    ~World();

    void Run();
    void ProcessCommand(const std::string& command);

private:
    std::vector<Room*> rooms;
    Player* player;

    // Enemigos
    std::vector<Enemy*> enemies;

    bool equalIgnoreCase(const std::string& a, const std::string& b) const;

    // Buscar Bag en inventario o dirección
    class Bag* findBag(const std::string& containerName);

    // Buscar Exit en dirección
    class Exit* findExit(const std::string& exitName);

    // Buscar Item en inventario o dirección
    class Item* findItem(const std::string& itemName);

    // Abrir/cerrar Exit
    void openExit(class Exit* exitPtr);
    void closeExit(class Exit* exitPtr);

    // Actualizar enemigos tras cada turno
    void UpdateEnemies();
};

#endif // WORLD_H
