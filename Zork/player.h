#ifndef PLAYER_H
#define PLAYER_H

#include "creature.h"
#include "room.h"
#include "item.h"

class Player : public Creature {
public:
    Player(const std::string& name, const std::string& description, Room* startRoom);

    // Moves the player to the room in the given direction.
    void Move(const std::string& direction);
    // Shows the player's inventory.
    void ShowInventory() const;
    // Picks up an item.
    void PickUp(Item* obj);
    // Drops an item.
    void Drop(Item* obj);
    // Returns the current room of the player.
    Room* GetCurrentRoom() const;
};

#endif // PLAYER_H
