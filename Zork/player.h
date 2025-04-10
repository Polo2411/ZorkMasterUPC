#ifndef PLAYER_H
#define PLAYER_H

#include "creature.h"
#include "room.h"
#include "item.h"
#include "bag.h"
#include <vector>
#include <string>

class Enemy; // forward declaration

class Player : public Creature {
public:
    Player(const std::string& name, const std::string& description, Room* startRoom);

    // Movimiento dentro de la sala
    void Move(const std::string& direction);

    // Cruzar una Exit
    void ExitRoom(const std::string& direction);

    // Tomar un �tem en la direcci�n actual
    void TakeItem(const std::string& itemName);

    // Soltar un �tem en la direcci�n actual
    void DropItem(const std::string& itemName);

    // Insertar �tem a inventario si cabe
    void InsertItemToInventory(Item* item);

    // Eliminar �tem del inventario
    void RemoveItemFromInventory(Item* item);

    // Buscar �tem por nombre en el inventario
    Item* FindItemInInventory(const std::string& itemName);

    // Mostrar inventario, con stats de armas/pociones
    void ShowInventory() const;

    // Retorna la habitaci�n actual
    Room* GetCurrentRoom() const;

    // Direcci�n interna en la sala
    std::string GetPlayerDirection() const;
    void SetPlayerDirection(const std::string& dir);

    // Mostrar estado del jugador (salud, inventario, etc.)
    void Status() const;

    // Devuelve una copia del vector de �tems
    std::vector<Item*> GetInventory() const;

    // L�mite de �tems: 2 a menos que sean Bag
    bool CanCarry(Item* obj) const;

    // Ver si hay una Bag abierta
    bool HasOpenBag() const;

    // Atacar a un enemigo cuerpo a cuerpo
    void AttackEnemy(Enemy* targetEnemy);

    // Disparar a un enemigo con Gun (si se tiene)
    void ShootEnemy(Enemy* targetEnemy);

    // Manejo de vida m�xima
    void SetMaxHealth(int m);
    int GetMaxHealth() const;

private:
    Room* currentRoom;
    std::string playerDirection;
    std::vector<Item*> inventory;

    // Vida m�xima
    int maxHealth;
};

#endif // PLAYER_H
