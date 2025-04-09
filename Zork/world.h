#ifndef WORLD_H
#define WORLD_H

#include <string>
#include <vector>
#include "room.h"
#include "player.h"

class World {
public:
    World();
    ~World();

    void Run();
    void toLower(const std::string& str);
    void ProcessCommand(const std::string& command);

private:
    std::vector<Room*> rooms;
    Player* player;
};

#endif // WORLD_H
