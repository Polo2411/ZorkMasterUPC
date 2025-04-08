#ifndef WORLD_H
#define WORLD_H

#include <string>
#include "room.h"
#include "player.h"

class World {
public:
    World();
    ~World();
    void Run();

private:
    Room* startRoom;
    Player* player;
    void ProcessCommand(const std::string& command);
};

#endif // WORLD_H
