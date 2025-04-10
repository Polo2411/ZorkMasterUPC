#include "enemy.h"
#include "exit.h"
#include "room.h"
#include "player.h"
#include <iostream>
#include <random>

// ======================
// Base Enemy
// ======================
Enemy::Enemy(const std::string& name,
    const std::string& description,
    Room* startRoom)
    : Creature(name, description, startRoom)
{
    // Podrías forzar health = 100 si lo deseas
}

void Enemy::TakeDamage(int dmg) {
    health -= dmg;
    std::cout << name << " takes " << dmg << " damage, health is now " << health << ".\n";
    if (!IsAlive()) {
        std::cout << name << " has been defeated.\n";
        if (location) {
            location->children.remove(this);
        }
    }
}

// Lógica de Update por defecto:
// 1) Si está en la misma sala que el jugador => Attack
// 2) Si no, hay 50% de probabilidad de moverse
// 3) Si decide moverse, elige aleatoriamente una dirección con un Exit abierto
//    y se desplaza allí.
void Enemy::Update(Player* player) {
    if (!IsAlive()) return;

    // Si Enemy y Player comparten sala => Attack
    if (GetLocation() == player->GetCurrentRoom()) {
        Attack(player);
        return;
    }

    // 50% de probabilidades de no moverse
    static std::random_device rd;
    static std::mt19937 rng(rd());
    std::uniform_int_distribution<int> coin(0, 1);
    if (coin(rng) == 0) {
        // No se mueve
        return;
    }

    // Buscar si hay direcciones con un Exit abierto
    Room* current = GetLocation();
    if (!current) return;

    const auto& dirsMap = current->GetDirections();  // o .GetDirections() según tu Room
    std::vector<std::string> openDirs;
    for (auto& kv : dirsMap) {
        const std::string& dir = kv.first;
        const auto& ents = kv.second;
        bool foundOpenExit = false;
        for (auto e : ents) {
            if (e->type == EXIT) {
                Exit* ex = dynamic_cast<Exit*>(e);
                if (ex && ex->GetState() == OPEN) {
                    foundOpenExit = true;
                    break;
                }
            }
        }
        if (foundOpenExit) {
            openDirs.push_back(dir);
        }
    }
    if (openDirs.empty()) {
        // No hay salidas abiertas
        return;
    }

    // Elegir dirección aleatoria
    std::uniform_int_distribution<int> dist(0, (int)openDirs.size() - 1);
    int choice = dist(rng);
    std::string chosenDir = openDirs[choice];

    // Moverse
    auto entsInDir = current->GetEntities(chosenDir);
    for (auto e : entsInDir) {
        if (e->type == EXIT) {
            Exit* ex = dynamic_cast<Exit*>(e);
            if (ex->GetState() == OPEN) {
                Room* dest = ex->GetDestinationFor(current);
                if (dest) {
                    std::cout << name << " moves " << chosenDir
                        << " to " << dest->name << ".\n";
                    SetLocation(dest);
                }
                return;
            }
        }
    }
}

// ======================
// Demon (10 damage)
// ======================
Demon::Demon(const std::string& name,
    const std::string& description,
    Room* startRoom)
    : Enemy(name, description, startRoom)
{
    health = 50;
}

void Demon::Attack(Creature* target) {
    int dmg = 10;
    std::cout << name << " attacks " << target->name
        << " for " << dmg << " damage!\n";
    target->SetHealth(target->GetHealth() - dmg);
    if (!target->IsAlive()) {
        std::cout << target->name << " is defeated...\n";
    }
}

// ======================
// DemonKnight (20 damage)
// ======================
DemonKnight::DemonKnight(const std::string& name,
    const std::string& description,
    Room* startRoom)
    : Enemy(name, description, startRoom)
{
    health = 150;
}

void DemonKnight::Attack(Creature* target) {
    int dmg = 20;
    std::cout << name << " strikes " << target->name
        << " for " << dmg << " damage!\n";
    target->SetHealth(target->GetHealth() - dmg);
    if (!target->IsAlive()) {
        std::cout << target->name << " is destroyed by " << name << "...\n";
    }
}

// ======================
// Vergil
// ======================
Vergil::Vergil(const std::string& name,
    const std::string& description,
    Room* startRoom)
    : Enemy(name, description, startRoom),
    consecutiveAttacks(0),
    inDevilTrigger(false),
    devilTriggerRounds(0)
{
    health = 120;
}

void Vergil::Attack(Creature* target) {
    // Aumentamos el contador de ataques
    consecutiveAttacks++;

    int baseDamage = 15;
    int dmg = baseDamage;
    if (inDevilTrigger) {
        dmg = (int)(baseDamage * 1.5f);
    }

    std::cout << name << " slashes " << target->name
        << " for " << dmg << " damage!\n";
    target->SetHealth(target->GetHealth() - dmg);
    if (!target->IsAlive()) {
        std::cout << target->name << " is annihilated by " << name << "...\n";
    }
}

void Vergil::Update(Player* player) {
    if (!IsAlive()) return;

    // Si está en devilTrigger => descontar un turno
    if (inDevilTrigger) {
        devilTriggerRounds--;
        if (devilTriggerRounds <= 0) {
            inDevilTrigger = false;
            std::cout << name << "'s devil trigger wears off.\n";
        }
    }

    // Si no está en devilTrigger y ha atacado 2 veces => transform
    if (!inDevilTrigger && consecutiveAttacks >= 2) {
        inDevilTrigger = true;
        devilTriggerRounds = 2;
        consecutiveAttacks = 0;
        std::cout << name << " activates Devil Trigger! (2 turns, +50% dmg)\n";
    }

    // Luego el mismo comportamiento base: Attack si la sala coincide, sino 50% moverse
    if (GetLocation() == player->GetCurrentRoom()) {
        std::cout << name << " sees " << player->name << "!\n";
        Attack(player);
        return;
    }

    // 50% chance skip
    static std::random_device rd;
    static std::mt19937 rng(rd());
    std::uniform_int_distribution<int> coin(0, 1);
    if (coin(rng) == 0) {
        // no se mueve
        return;
    }

    Room* current = GetLocation();
    if (!current) return;

    const auto& dirsMap = current->GetDirections();
    std::vector<std::string> openDirs;
    for (auto& kv : dirsMap) {
        bool foundOpenExit = false;
        for (auto e : kv.second) {
            if (e->type == EXIT) {
                Exit* ex = dynamic_cast<Exit*>(e);
                if (ex && ex->GetState() == OPEN) {
                    foundOpenExit = true;
                    break;
                }
            }
        }
        if (foundOpenExit) {
            openDirs.push_back(kv.first);
        }
    }
    if (openDirs.empty()) return;

    std::uniform_int_distribution<int> dist(0, (int)openDirs.size() - 1);
    int choice = dist(rng);
    std::string chosenDir = openDirs[choice];

    auto entsInDir = current->GetEntities(chosenDir);
    for (auto e : entsInDir) {
        if (e->type == EXIT) {
            Exit* ex = dynamic_cast<Exit*>(e);
            if (ex->GetState() == OPEN) {
                Room* dest = ex->GetDestinationFor(current);
                if (dest) {
                    std::cout << name << " moves " << chosenDir
                        << " to " << dest->name << ".\n";
                    SetLocation(dest);
                }
                return;
            }
        }
    }
}
