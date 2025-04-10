#ifndef WORLD_H
#define WORLD_H

#include <string>
#include <vector>
#include "room.h"
#include "player.h"
#include "enemy.h"   // para Enemy y subclases

class World {
public:
    World();
    ~World();

    void Run();
    void ProcessCommand(const std::string& command);

private:
    std::vector<Room*> rooms;
    Player* player;

    // Contenedor de enemigos
    std::vector<Enemy*> enemies;

    bool equalIgnoreCase(const std::string& a, const std::string& b) const;

    // Busca un Bag en la dirección actual o en el inventario
    class Bag* findBag(const std::string& containerName);

    // Busca un Exit en la dirección actual
    class Exit* findExit(const std::string& exitName);

    // Busca un Item en la sala/dirección actual o en el inventario
    class Item* findItem(const std::string& itemName);

    // Métodos para abrir/cerrar Exits
    void openExit(class Exit* exitPtr);
    void closeExit(class Exit* exitPtr);

    // Actualiza enemigos tras cada turno
    void UpdateEnemies();
};

#endif // WORLD_H
