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

    void Move(const std::string& direction);
    void ExitRoom(const std::string& direction);
    void TakeItem(const std::string& itemName);
    void DropItem(const std::string& itemName);

    // Inserta un ítem al inventario si cabe; si no, lo suelta en la sala
    void InsertItemToInventory(Item* item);

    // Saca un ítem específico del inventario (si existe)
    void RemoveItemFromInventory(Item* item);

    // Busca un ítem por nombre (ignora mayúsculas) en el inventario
    Item* FindItemInInventory(const std::string& itemName);

    void ShowInventory() const;
    Room* GetCurrentRoom() const;
    std::string GetPlayerDirection() const;
    void SetPlayerDirection(const std::string& dir);
    void Status() const;

    // Devuelve una COPIA del vector. (No se puede usar para borrar ítems directamente)
    std::vector<Item*> GetInventory() const;

    // Para chequear si el Player puede llevar un ítem extra
    bool CanCarry(Item* obj) const;
    bool HasOpenBag() const;

    void AttackEnemy(class Enemy* targetEnemy);


private:
    Room* currentRoom;
    std::string playerDirection;
    std::vector<Item*> inventory;
};

#endif // PLAYER_H
