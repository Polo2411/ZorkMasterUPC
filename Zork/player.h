#ifndef PLAYER_H
#define PLAYER_H

#include "creature.h"
#include "room.h"
#include "item.h"
#include "bag.h"  // For dynamic_cast checking
#include <vector>

class Player : public Creature {
public:
    Player(const std::string& name, const std::string& description, Room* startRoom);

    // Moves the player to the room in the given direction.
    void Move(const std::string& direction);
    // Shows the player's inventory (the two-hand items plus bag details if any)
    void ShowInventory() const;

    // Picks up an item if allowed (up to two non-bag items; bags don't count).
    void PickUp(Item* obj);
    // Drops an item.
    void Drop(Item* obj);

    Room* GetCurrentRoom() const;

    // Returns the player's inventory (all items currently held)
    std::vector<Item*> GetInventory() const;
    // Displays status: player's name, health, location, and inventory details.
    void Status() const;

private:
    Room* currentRoom;
    std::vector<Item*> inventory;

    // Checks if adding this item would exceed the two-hand limit (excluding bags)
    bool CanCarry(Item* obj) const;
};

#endif // PLAYER_H
