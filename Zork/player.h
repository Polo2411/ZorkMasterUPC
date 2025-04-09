#ifndef PLAYER_H
#define PLAYER_H

#include "creature.h"
#include "room.h"
#include "item.h"
#include "bag.h"
#include <vector>
#include <string>

class Player : public Creature {
public:
    Player(const std::string& name, const std::string& description, Room* startRoom);

    // "move direction" => move the player within the current room.
    void Move(const std::string& direction);

    // "exit direction" => exit to another room if there's an open Exit.
    void ExitRoom(const std::string& direction);

    // "take itemName" => pick up an item from the current player's direction.
    void TakeItem(const std::string& itemName);

    // "drop itemName" => drop an item in the current player's direction.
    void DropItem(const std::string& itemName);

    // Shows the player's inventory.
    void ShowInventory() const;

    // Returns the current room.
    Room* GetCurrentRoom() const;

    // Returns the player's current internal direction within the room.
    std::string GetPlayerDirection() const;
    // Sets the player's internal direction.
    void SetPlayerDirection(const std::string& dir);

    // Displays the player's status.
    void Status() const;

    // Returns a copy of the player's inventory.
    std::vector<Item*> GetInventory() const;

private:
    Room* currentRoom;
    std::string playerDirection;  // e.g., "center", "north", "south", "east", "west".
    std::vector<Item*> inventory;

    bool CanCarry(Item* obj) const;
};

#endif // PLAYER_H
