#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "room.h"
#include "item.h"
#include <vector>

class Player : public Entity {
public:
    Player(const std::string& name, const std::string& description, Room* startRoom);
    // Moves the player in the specified direction
    void Move(const std::string& direction);
    // Displays the player's inventory
    void ShowInventory() const;
    // Picks up an item
    void PickUp(Item* obj);
    // Drops an item
    void Drop(Item* obj);
    Room* GetCurrentRoom() const;

private:
    Room* currentRoom;
    std::vector<Item*> inventory;
};

#endif // PLAYER_H
