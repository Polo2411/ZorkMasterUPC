#ifndef WORLD_H
#define WORLD_H

#include <string>
#include <vector>
#include "room.h"
#include "player.h"

// World se encarga de crear las salas y de procesar los comandos del juego.
class World {
public:
    World();
    ~World();

    void Run();
    void ProcessCommand(const std::string& command);

private:
    std::vector<Room*> rooms;
    Player* player;

    // Función auxiliar para comparar cadenas ignorando mayúsculas
    bool equalIgnoreCase(const std::string& a, const std::string& b) const;

    // Busca una Bag en el inventario del player o en la sala en la dirección actual.
    class Bag* findBag(const std::string& containerName);
};

#endif // WORLD_H
