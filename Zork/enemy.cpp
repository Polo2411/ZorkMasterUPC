#include "enemy.h"
#include "exit.h"
#include "room.h"
#include <iostream>
#include <random>

// --------------------
// Enemy (base)
// --------------------
Enemy::Enemy(const std::string& name, const std::string& description, Room* startRoom)
    : Creature(name, description, startRoom)
{
    health = 100;
}

// Tomamos Attack como pura en .h, así que no definimos Attack aquí.

void Enemy::TakeDamage(int dmg) {
    health -= dmg;
    std::cout << name << " takes " << dmg << " damage, health is now " << health << ".\n";
    if (!IsAlive()) {
        std::cout << name << " has been defeated.\n";
        if (location)
            location->children.remove(this);
    }
}

// IA
void Enemy::Update(Player* player) {
    if (!IsAlive()) return;

    // Si está en la misma sala, ataca
    if (GetLocation() == player->GetCurrentRoom()) {
        std::cout << name << " sees " << player->name << "!\n";
        Attack(player);
        return;
    }
    // Movimiento
    Room* current = GetLocation();
    if (!current) return;

    // Buscar direcciones
    const std::map<std::string, std::vector<Entity*>>& dirsMap = current->GetDirections();
    // 50% de moverse, 50% de quedarse
    static std::random_device rd;
    static std::mt19937 rng(rd());
    std::uniform_int_distribution<int> coin(0, 1);
    if (coin(rng) == 0) {
        // No se mueve este turno
        return;
    }

    // Recolectar dirs con exits abiertas
    std::vector<std::string> possibleDirs;
    for (std::map<std::string, std::vector<Entity*>>::const_iterator it = dirsMap.begin();
        it != dirsMap.end(); ++it)
    {
        const std::string& dir = it->first;
        const std::vector<Entity*>& ents = it->second;
        bool openExit = false;
        for (size_t i = 0; i < ents.size(); i++) {
            if (ents[i]->type == EXIT) {
                Exit* ex = dynamic_cast<Exit*>(ents[i]);
                if (ex && ex->GetState() == OPEN) {
                    openExit = true;
                    break;
                }
            }
        }
        if (openExit) {
            possibleDirs.push_back(dir);
        }
    }
    if (possibleDirs.empty()) {
        return;
    }
    std::uniform_int_distribution<int> dist(0, (int)possibleDirs.size() - 1);
    int choice = dist(rng);
    std::string chosenDir = possibleDirs[choice];

    const std::vector<Entity*>& dirEnts = current->GetEntities(chosenDir);
    for (size_t i = 0; i < dirEnts.size(); i++) {
        if (dirEnts[i]->type == EXIT) {
            Exit* ex = dynamic_cast<Exit*>(dirEnts[i]);
            if (ex && ex->GetState() == OPEN) {
                Room* dest = ex->GetDestinationFor(current);
                if (dest) {
                    std::cout << name << " wanders " << chosenDir
                        << " to " << dest->name << ".\n";
                    SetLocation(dest);
                }
                return;
            }
        }
    }
}

// --------------------
// Demon: 10 damage
// --------------------
Demon::Demon(const std::string& name, const std::string& description, Room* startRoom)
    : Enemy(name, description, startRoom)
{
    health = 50;
}

void Demon::Attack(Creature* target) {
    int dmg = 10;
    std::cout << name << " attacks " << target->name << " for " << dmg << " damage!\n";
    target->SetHealth(target->GetHealth() - dmg);
    if (target->IsAlive()) {
        std::cout << target->name << " has now " << target->GetHealth() << " HP.\n";
    }
    else {
        std::cout << target->name << " is defeated by " << name << "...\n";
    }
}

// --------------------
// DemonKnight: 20 damage
// --------------------
DemonKnight::DemonKnight(const std::string& name, const std::string& description, Room* startRoom)
    : Enemy(name, description, startRoom)
{
    health = 100;
}

void DemonKnight::Attack(Creature* target) {
    int dmg = 20;
    std::cout << name << " strikes " << target->name << " for " << dmg << " damage!\n";
    target->SetHealth(target->GetHealth() - dmg);
    if (target->IsAlive()) {
        std::cout << target->name << " has now " << target->GetHealth() << " HP.\n";
    }
    else {
        std::cout << target->name << " is destroyed by " << name << "...\n";
    }
}
