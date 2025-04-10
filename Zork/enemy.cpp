#include "enemy.h"
#include "player.h"
#include "exit.h"
#include "room.h"
#include <iostream>
#include <random>  // Para movimiento aleatorio

// --------------------
// Enemy (clase base)
// --------------------
Enemy::Enemy(const std::string& name, const std::string& description, Room* startRoom)
    : Creature(name, description, startRoom)
{
    health = 100;
    // type = ENEMY; // si quieres un tipo enumerado distinto
}

void Enemy::Attack(Creature* target) {
    std::cout << name << " attacks " << target->name << "!\n";
    int dmg = 10;  // Daño base
    target->SetHealth(target->GetHealth() - dmg);
    if (!target->IsAlive()) {
        std::cout << target->name << " is defeated by " << name << "...\n";
    }
}

void Enemy::TakeDamage(int dmg) {
    health -= dmg;
    std::cout << name << " takes " << dmg << " damage, health is now " << health << ".\n";
    if (!IsAlive()) {
        std::cout << name << " has been defeated.\n";
        if (location) {
            // Lo quitamos de la sala
            location->children.remove(this);
        }
    }
}

// Se llama tras cada comando del jugador
void Enemy::Update(Player* player) {
    if (!IsAlive()) return;

    // Si Enemy y Player comparten sala, Attack
    if (GetLocation() == player->GetCurrentRoom()) {
        std::cout << name << " sees " << player->name << " and attacks!\n";
        Attack(player);
        return;
    }

    // De lo contrario, el Enemy intenta moverse
    Room* current = GetLocation();
    if (!current) return;

    // Recogemos el mapa de direcciones
    const std::map<std::string, std::vector<Entity*>>& dirsMap = current->GetDirections();
    std::vector<std::string> possibleDirs;

    // Iterar a la vieja usanza para no usar auto
    for (std::map<std::string, std::vector<Entity*>>::const_iterator it = dirsMap.begin();
        it != dirsMap.end();
        ++it)
    {
        const std::string& dir = it->first;
        const std::vector<Entity*>& ents = it->second;
        bool hasOpenExit = false;
        // Buscamos una Exit abierta
        for (size_t i = 0; i < ents.size(); i++) {
            if (ents[i]->type == EXIT) {
                Exit* ex = dynamic_cast<Exit*>(ents[i]);
                if (ex && ex->GetState() == OPEN) {
                    hasOpenExit = true;
                    break;
                }
            }
        }
        if (hasOpenExit) {
            possibleDirs.push_back(dir);
        }
    }

    if (possibleDirs.empty()) {
        // No hay salidas abiertas, se queda quieto
        return;
    }

    // Elegir dirección aleatoria
    static std::random_device rd;
    static std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(0, (int)possibleDirs.size() - 1);
    int choice = dist(rng);
    std::string chosenDir = possibleDirs[choice];

    // Movernos a la exit en chosenDir si está OPEN
    const std::vector<Entity*>& entsInDir = current->GetEntities(chosenDir);
    for (size_t i = 0; i < entsInDir.size(); i++) {
        Entity* e = entsInDir[i];
        if (e->type == EXIT) {
            Exit* ex = dynamic_cast<Exit*>(e);
            if (ex && ex->GetState() == OPEN) {
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

// --------------------
// Demon
// --------------------
Demon::Demon(const std::string& name, const std::string& description, Room* startRoom)
    : Enemy(name, description, startRoom)
{
    // Menos vida
    health = 50;
}

// --------------------
// DemonKnight
// --------------------
DemonKnight::DemonKnight(const std::string& name, const std::string& description, Room* startRoom)
    : Enemy(name, description, startRoom)
{
    // Más vida
    health = 150;
}
