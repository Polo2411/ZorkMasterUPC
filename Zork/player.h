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

    // Atacar cuerpo a cuerpo
    void AttackEnemy(Enemy* targetEnemy);

    // Disparar con Gun
    void ShootEnemy(Enemy* targetEnemy);

    // Manejo de vida m�xima
    void SetMaxHealth(int m);
    int GetMaxHealth() const;

    // Mec�nica Devil Trigger
    void ActivateDevilTrigger();     // Llamado con "transform" cuando cumpla condiciones
    bool IsInDevilTrigger() const;   // Saber si est� activo
    void OnTurnPassed();             // Llamado cada turno, para decrementar devilTriggerRounds

private:
    Room* currentRoom;
    std::string playerDirection;
    std::vector<Item*> inventory;

    int maxHealth;

    // =====================
    // DEVIL TRIGGER FIELDS
    // =====================
    int  consecutiveAttacks;   // cu�ntas veces ha atacado seguidas
    bool inDevilTrigger;
    int  devilTriggerRounds;   // cu�ntos turnos dura
};

#endif // PLAYER_H
