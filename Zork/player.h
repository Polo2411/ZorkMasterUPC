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

    // Tomar un ítem en la dirección actual
    void TakeItem(const std::string& itemName);

    // Soltar un ítem en la dirección actual
    void DropItem(const std::string& itemName);

    // Insertar ítem a inventario si cabe
    void InsertItemToInventory(Item* item);

    // Eliminar ítem del inventario
    void RemoveItemFromInventory(Item* item);

    // Buscar ítem por nombre en el inventario
    Item* FindItemInInventory(const std::string& itemName);

    // Mostrar inventario, con stats de armas/pociones
    void ShowInventory() const;

    // Retorna la habitación actual
    Room* GetCurrentRoom() const;

    // Dirección interna en la sala
    std::string GetPlayerDirection() const;
    void SetPlayerDirection(const std::string& dir);

    // Mostrar estado del jugador (salud, inventario, etc.)
    void Status() const;

    // Devuelve una copia del vector de ítems
    std::vector<Item*> GetInventory() const;

    // Límite de ítems: 2 a menos que sean Bag
    bool CanCarry(Item* obj) const;

    // Ver si hay una Bag abierta
    bool HasOpenBag() const;

    // Atacar cuerpo a cuerpo
    void AttackEnemy(Enemy* targetEnemy);

    // Disparar con Gun
    void ShootEnemy(Enemy* targetEnemy);

    // Manejo de vida máxima
    void SetMaxHealth(int m);
    int GetMaxHealth() const;

    // Mecánica Devil Trigger
    void ActivateDevilTrigger();     // Llamado con "transform" cuando cumpla condiciones
    bool IsInDevilTrigger() const;   // Saber si está activo
    void OnTurnPassed();             // Llamado cada turno, para decrementar devilTriggerRounds

private:
    Room* currentRoom;
    std::string playerDirection;
    std::vector<Item*> inventory;

    int maxHealth;

    // =====================
    // DEVIL TRIGGER FIELDS
    // =====================
    int  consecutiveAttacks;   // cuántas veces ha atacado seguidas
    bool inDevilTrigger;
    int  devilTriggerRounds;   // cuántos turnos dura
};

#endif // PLAYER_H
