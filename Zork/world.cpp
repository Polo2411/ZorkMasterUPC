#include "world.h"
#include "sword.h"
#include "bag.h"
#include "health_potion.h"
#include "exit.h"
#include "key.h"
#include "item.h"
#include "enemy.h"        // Enemy, Demon, DemonKnight
#include "player.h"
#include "string_utils.h"
#include "gun.h"
#include "bullet.h"
#include "vitality_potion.h"
#include "upgrader.h"
#include <iostream>
#include <sstream>
#include <map>
#include <functional>
#include <vector>
#include <algorithm>
#include <cctype>

std::vector<std::string> tokenize(const std::string& input) {
    std::istringstream iss(input);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
        token = toLower(token);
        tokens.push_back(token);
    }
    return tokens;
}

World::World() {
    // Creamos 2 salas
    Room* roomOne = new Room("RoomOne", "A simple room with minimal furniture.");
    Room* roomTwo = new Room("RoomTwo", "A second room with a strange aura.");

    // Objetos en RoomOne
    // 1) Sword (ya existente)
    Sword* sword = new Sword("Sword", "A shiny sword on the floor.", 20, "east", roomOne);
    roomOne->AddEntity("east", sword);

    // 2) Bag (ya existente)
    Bag* bag = new Bag("Bag", "A sturdy leather bag.", 5, "south", roomOne);
    roomOne->AddEntity("south", bag);

    // 3) Key (ya existente)
    Key* key = new Key("Key", "A small key.", "west", roomTwo, roomOne);
    roomOne->AddEntity("west", key);

    // 4) Gun
    Gun* gun = new Gun("Gun", "A powerful firearm that deals 30 damage.", 30, "center", roomOne);
    roomOne->AddEntity("center", gun);

    // 5) Bullet
    Bullet* bullet = new Bullet("Bullet", "A single bullet for a gun.", "north", roomOne);
    roomOne->AddEntity("north", bullet);

    // Exit que conecta RoomOne <-> RoomTwo
    Exit* exitOne = new Exit("Door", "A passage to another room.", roomOne, roomTwo);
    exitOne->SetState(CLOSED);
    roomOne->AddEntity("north", exitOne);
    roomTwo->AddEntity("south", exitOne);

    // Objetos en RoomTwo
    // 1) HealthPotion (ya existente)
    HealthPotion* potion = new HealthPotion("HealthPotion", "Heals you quite a bit.", 50, "east", roomTwo);
    roomTwo->AddEntity("east", potion);

    // 2) VitalityPotion
    VitalityPotion* vit = new VitalityPotion("VitalityPotion", "Raises max HP by 20 and heals 20 HP.", 20, 20, "west", roomTwo);
    roomTwo->AddEntity("west", vit);

    // 3) Upgrader
    Upgrader* up = new Upgrader("Upgrader", "Allows you to upgrade a Sword or Gun by +10 damage.", "south", roomTwo);
    roomTwo->AddEntity("south", up);

    // Creamos un Demon en la segunda sala
    Demon* demon = new Demon("Demon", "A fiendish underworld creature.", roomTwo);
    enemies.push_back(demon);

    rooms.push_back(roomOne);
    rooms.push_back(roomTwo);

    // Creamos al jugador en roomOne
    player = new Player("Player", "An intrepid adventurer.", roomOne);
}

World::~World() {
    // Liberar salas
    for (size_t i = 0; i < rooms.size(); i++) {
        delete rooms[i];
    }
    // Liberar enemigos
    for (size_t i = 0; i < enemies.size(); i++) {
        delete enemies[i];
    }
    delete player;
}

// Comparación de cadenas ignorando mayúsculas
bool World::equalIgnoreCase(const std::string& a, const std::string& b) const {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); i++) {
        if (std::tolower((unsigned char)a[i]) !=
            std::tolower((unsigned char)b[i])) {
            return false;
        }
    }
    return true;
}

// Busca Bag
Bag* World::findBag(const std::string& containerName) {
    auto inv = player->GetInventory();
    for (size_t i = 0; i < inv.size(); i++) {
        if (equalIgnoreCase(inv[i]->name, containerName)) {
            return dynamic_cast<Bag*>(inv[i]);
        }
    }
    auto ents = player->GetCurrentRoom()->GetEntities(player->GetPlayerDirection());
    for (size_t i = 0; i < ents.size(); i++) {
        if (ents[i]->type == ITEM && equalIgnoreCase(ents[i]->name, containerName)) {
            return dynamic_cast<Bag*>(ents[i]);
        }
    }
    return nullptr;
}

// Busca Exit
Exit* World::findExit(const std::string& exitName) {
    auto ents = player->GetCurrentRoom()->GetEntities(player->GetPlayerDirection());
    for (size_t i = 0; i < ents.size(); i++) {
        if (ents[i]->type == EXIT && equalIgnoreCase(ents[i]->name, exitName)) {
            return dynamic_cast<Exit*>(ents[i]);
        }
    }
    return nullptr;
}

// Busca Item
Item* World::findItem(const std::string& itemName) {
    // primero en inventario
    auto inv = player->GetInventory();
    for (size_t i = 0; i < inv.size(); i++) {
        if (equalIgnoreCase(inv[i]->name, itemName)) {
            return inv[i];
        }
    }
    // en sala actual / dirección actual
    auto ents = player->GetCurrentRoom()->GetEntities(player->GetPlayerDirection());
    for (size_t i = 0; i < ents.size(); i++) {
        if (ents[i]->type == ITEM && equalIgnoreCase(ents[i]->name, itemName)) {
            return dynamic_cast<Item*>(ents[i]);
        }
    }
    return nullptr;
}

// Abrir exit
void World::openExit(Exit* exitPtr) {
    if (!exitPtr) return;
    if (exitPtr->GetState() == OPEN) {
        std::cout << "It's already open.\n";
        return;
    }
    if (exitPtr->GetState() == LOCKED) {
        std::cout << "It seems locked.\n";
        return;
    }
    bool hasKey = false;
    Item* chosenKey = nullptr;
    auto inv = player->GetInventory();
    for (size_t i = 0; i < inv.size(); i++) {
        if (auto realKey = dynamic_cast<Key*>(inv[i])) {
            if (realKey->GetOpensRoom() == exitPtr->GetDestination()) {
                hasKey = true;
                chosenKey = realKey;
                break;
            }
        }
    }
    if (!hasKey) {
        std::cout << "You don't have the right key to open this exit.\n";
        return;
    }
    exitPtr->SetState(OPEN);
    std::cout << "You use " << chosenKey->name
        << " to open the " << exitPtr->name << ".\n";
}

// Cerrar exit
void World::closeExit(Exit* exitPtr) {
    if (!exitPtr) return;
    if (exitPtr->GetState() == CLOSED) {
        std::cout << "It's already closed.\n";
        return;
    }
    if (exitPtr->GetState() == LOCKED) {
        std::cout << "It's locked.\n";
        return;
    }
    bool hasKey = false;
    Item* chosenKey = nullptr;
    auto inv = player->GetInventory();
    for (size_t i = 0; i < inv.size(); i++) {
        if (auto realKey = dynamic_cast<Key*>(inv[i])) {
            if (realKey->GetOpensRoom() == exitPtr->GetDestination()) {
                hasKey = true;
                chosenKey = realKey;
                break;
            }
        }
    }
    if (!hasKey) {
        std::cout << "You don't have the key to close this exit.\n";
        return;
    }
    exitPtr->SetState(CLOSED);
    std::cout << "You use " << chosenKey->name
        << " to close the " << exitPtr->name << ".\n";
}

void World::Run() {
    std::cout << "Welcome to Zork\n";
    player->GetCurrentRoom()->Look();

    while (true) {
        std::cout << "\n> ";
        std::string command;
        std::getline(std::cin, command);
        if (command == "quit" || command == "exit") {
            break;
        }
        ProcessCommand(command);
        // No llamamos UpdateEnemies() aquí,
        // sino en ProcessCommand según causeTurn.
    }
}

// Procesa comandos y marca causeTurn = true
// en move, exit, attack, use, shoot
void World::ProcessCommand(const std::string& command) {
    auto tokens = tokenize(command);
    if (tokens.empty()) {
        std::cout << "Please type a command.\n";
        return;
    }

    bool causeTurn = false;
    std::string verb = tokens[0];

    if (player->HasOpenBag()) {
        // Bloqueamos acciones de turno si bag abierto
        if (verb == "move" || verb == "exit" || verb == "drop"
            || verb == "attack" || verb == "use" || verb == "shoot")
        {
            std::cout << "You have an open bag. Close it before performing that action.\n";
            return;
        }
    }

    std::map<std::string, std::function<void(const std::vector<std::string>&)>> cmdMap;

    // look
    cmdMap["look"] = [this](auto args) {
        if (args.empty()) {
            player->GetCurrentRoom()->Look();
        }
        else if (args.size() == 1) {
            std::string itemName = args[0];
            Item* itemPtr = findItem(itemName);
            if (!itemPtr) {
                std::cout << "You don't see any " << itemName << " here.\n";
            }
            else {
                std::cout << itemPtr->name << "\n" << itemPtr->description << "\n";
            }
        }
        else {
            std::cout << "Usage: look [itemName]\n";
        }
        };

    // move
    cmdMap["move"] = [this, &causeTurn](auto args) {
        if (args.empty()) {
            std::cout << "Move where? (north, south, east, west, center)\n";
            return;
        }
        player->Move(args[0]);
        causeTurn = true;
        };

    // exit
    cmdMap["exit"] = [this, &causeTurn](auto args) {
        if (args.empty()) {
            std::cout << "Exit which direction?\n";
            return;
        }
        player->ExitRoom(args[0]);
        causeTurn = true;
        };

    // take
    cmdMap["take"] = [this](auto args) {
        if (args.empty()) {
            std::cout << "Take what?\n";
            return;
        }
        // "take bullet" => Lógica especial
        if (args.size() == 1) {
            std::string itemName = args[0];
            // Comprobamos si es "bullet"
            if (equalIgnoreCase(itemName, "bullet")) {
                // Buscar la Bullet en la sala
                Item* bulletItem = findItem("bullet");
                if (!bulletItem) {
                    std::cout << "No bullet here.\n";
                    return;
                }
                // Ver si es un Bullet*
                Bullet* b = dynamic_cast<Bullet*>(bulletItem);
                if (!b) {
                    std::cout << "That isn't a bullet.\n";
                    return;
                }
                // Buscamos una Gun en el inventario del player
                bool hasGun = false;
                Gun* gunPtr = nullptr;
                auto inv = player->GetInventory();
                for (auto i : inv) {
                    if (auto g = dynamic_cast<Gun*>(i)) {
                        hasGun = true;
                        gunPtr = g;
                        break;
                    }
                    // Podrías mirar dentro de Bag si deseas, expandiendo la lógica
                }
                if (!hasGun) {
                    std::cout << "You have no gun to store bullets in. You cannot pick up the bullet.\n";
                    return;
                }
                // Si tenemos gun, sumamos 1 ammo
                gunPtr->AddAmmo(1);
                // Removemos la bullet de la sala
                player->GetCurrentRoom()->RemoveEntity(player->GetPlayerDirection(), b);
                std::cout << "You add a bullet to your gun. Ammo is now "
                    << gunPtr->GetAmmoCount() << ".\n";
                return;
            }
            // De lo contrario, no es bullet, => take normal
            player->TakeItem(args[0]);
            return;
        }
        // "take x from bag"
        if (args.size() == 3 && args[1] == "from") {
            std::string itemName = args[0];
            std::string containerName = args[2];
            Bag* bagPtr = findBag(containerName);
            if (!bagPtr) {
                std::cout << "No bag named " << containerName << " here.\n";
                return;
            }
            if (!bagPtr->IsOpen()) {
                std::cout << containerName << " is not open.\n";
                return;
            }
            Item* itemPtr = bagPtr->RemoveItem(itemName);
            if (!itemPtr) return;
            player->InsertItemToInventory(itemPtr);
        }
        else {
            std::cout << "Usage: take <item> [from <container>]\n";
        }
        };

    // drop
    cmdMap["drop"] = [this](auto args) {
        if (args.empty()) {
            std::cout << "Drop what?\n";
            return;
        }
        player->DropItem(args[0]);
        };

    // status
    cmdMap["status"] = [this](auto args) {
        player->Status();
        };

    // open
    cmdMap["open"] = [this](auto args) {
        if (args.empty()) {
            std::cout << "Open what?\n";
            return;
        }
        std::string targetName = args[0];
        Bag* bagPtr = findBag(targetName);
        if (bagPtr) {
            bagPtr->Use();
            return;
        }
        Exit* exitPtr = findExit(targetName);
        if (exitPtr) {
            openExit(exitPtr);
            return;
        }
        std::cout << "No exit here named " << targetName
            << ", and no bag named " << targetName << ".\n";
        };

    // close
    cmdMap["close"] = [this](auto args) {
        if (args.empty()) {
            std::cout << "Close what?\n";
            return;
        }
        std::string targetName = args[0];
        Bag* bagPtr = findBag(targetName);
        if (bagPtr) {
            bagPtr->Close();
            return;
        }
        Exit* exitPtr = findExit(targetName);
        if (exitPtr) {
            closeExit(exitPtr);
            return;
        }
        std::cout << "No exit here named " << targetName
            << ", and no bag named " << targetName << ".\n";
        };

    // save
    cmdMap["save"] = [this](auto args) {
        if (args.size() < 3) {
            std::cout << "Usage: save <item> in <container>\n";
            return;
        }
        std::string itemName = args[0];
        if (args[1] != "in") {
            std::cout << "Usage: save <item> in <container>\n";
            return;
        }
        std::string containerName = args[2];
        Item* toSave = player->FindItemInInventory(itemName);
        if (!toSave) {
            std::cout << "You don't have " << itemName << ".\n";
            return;
        }
        Bag* bagPtr = findBag(containerName);
        if (!bagPtr) {
            std::cout << "No bag named " << containerName << " here.\n";
            return;
        }
        if (!bagPtr->IsOpen()) {
            std::cout << containerName << " is not open.\n";
            return;
        }
        player->RemoveItemFromInventory(toSave);
        if (!bagPtr->AddItem(toSave)) {
            player->InsertItemToInventory(toSave);
        }
        else {
            std::cout << "Saved " << itemName << " in " << containerName << ".\n";
        }
        };

    // attack
    cmdMap["attack"] = [this, &causeTurn](auto args) {
        if (args.empty()) {
            std::cout << "Attack who?\n";
            return;
        }
        std::string enemyName = args[0];
        Room* curr = player->GetCurrentRoom();
        Enemy* targetEnemy = nullptr;
        for (std::list<Entity*>::iterator it = curr->children.begin();
            it != curr->children.end(); ++it)
        {
            Entity* ent = *it;
            if (ent->type == CREATURE) {
                Enemy* en = dynamic_cast<Enemy*>(ent);
                if (en && equalIgnoreCase(en->name, enemyName)) {
                    targetEnemy = en;
                    break;
                }
            }
        }
        if (!targetEnemy) {
            std::cout << "No enemy called " << enemyName << " here.\n";
            return;
        }
        player->AttackEnemy(targetEnemy);
        causeTurn = true;
        };

    // shoot <enemy>
    cmdMap["shoot"] = [this, &causeTurn](auto args) {
        if (args.empty()) {
            std::cout << "Shoot who?\n";
            return;
        }
        std::string enemyName = args[0];
        // Buscar enemy en la sala
        Room* curr = player->GetCurrentRoom();
        Enemy* targetEnemy = nullptr;
        for (std::list<Entity*>::iterator it = curr->children.begin();
            it != curr->children.end(); ++it)
        {
            Entity* ent = *it;
            if (ent->type == CREATURE) {
                Enemy* en = dynamic_cast<Enemy*>(ent);
                if (en && equalIgnoreCase(en->name, enemyName)) {
                    targetEnemy = en;
                    break;
                }
            }
        }
        if (!targetEnemy) {
            std::cout << "No enemy called " << enemyName << " here.\n";
            return;
        }
        // Disparo. Lógica en player->ShootEnemy(...) (que revisa Gun y ammo)
        player->ShootEnemy(targetEnemy);
        causeTurn = true;
        };

    // use (con subcasos "use <item>" y "use <item> in <target>")
    cmdMap["use"] = [this, &causeTurn](auto args) {
        if (args.empty()) {
            std::cout << "Use what?\n";
            return;
        }
        // Caso "use X in Y"
        if (args.size() == 3 && args[1] == "in") {
            std::string itemName = args[0];
            std::string targetName = args[2];
            // Buscar itemName en inventory
            Item* it = player->FindItemInInventory(itemName);
            if (!it) {
                std::cout << "You don't have " << itemName << " in your inventory.\n";
                return;
            }
            // Upgrader?
            Upgrader* up = dynamic_cast<Upgrader*>(it);
            if (up) {
                // Buscar arma (Sword o Gun)
                Item* targ = player->FindItemInInventory(targetName);
                if (!targ) {
                    std::cout << "You don't have " << targetName << " in your inventory.\n";
                    return;
                }
                // Si es Sword
                if (auto sw = dynamic_cast<Sword*>(targ)) {
                    sw->SetDamage(sw->GetDamage() + 10);
                    std::cout << "You upgrade your sword! Damage is now "
                        << sw->GetDamage() << ".\n";
                    // Consumir upgrader
                    player->RemoveItemFromInventory(it);
                    return;
                }
                // Si es Gun
                if (auto gun = dynamic_cast<Gun*>(targ)) {
                    gun->SetDamage(gun->GetDamage() + 10);
                    std::cout << "You upgrade your gun! Damage is now "
                        << gun->GetDamage() << ".\n";
                    // Consumir Upgrader
                    player->RemoveItemFromInventory(it);
                    return;
                }
                std::cout << "You cannot upgrade " << targetName << " with " << itemName << ".\n";
                return;
            }
            std::cout << "You can't use " << itemName << " in that way.\n";
            causeTurn = true;
            return;
        }
        // Caso "use X" normal
        if (args.size() == 1) {
            std::string itemName = args[0];
            Item* it = player->FindItemInInventory(itemName);
            if (!it) {
                std::cout << "You don't have " << itemName << " in your inventory.\n";
                return;
            }
            // HealthPotion
            if (auto hp = dynamic_cast<HealthPotion*>(it)) {
                int heal = hp->GetHealAmount();
                int newHP = player->GetHealth() + heal;
                if (newHP > player->GetMaxHealth()) {
                    newHP = player->GetMaxHealth();
                }
                player->SetHealth(newHP);
                std::cout << "You drink the " << hp->name
                    << " and restore " << heal
                    << " HP! Your health is now " << player->GetHealth() << ".\n";
                player->RemoveItemFromInventory(it);
                causeTurn = true;
                return;
            }
            // VitalityPotion
            if (auto vp = dynamic_cast<VitalityPotion*>(it)) {
                // Sube max health +20 y cura +20
                int extraMax = vp->GetExtraMax();
                int heal = vp->GetHealAmount();
                player->SetMaxHealth(player->GetMaxHealth() + extraMax);
                int newHP = player->GetHealth() + heal;
                if (newHP > player->GetMaxHealth()) {
                    newHP = player->GetMaxHealth();
                }
                player->SetHealth(newHP);
                std::cout << "You drink the " << vp->name
                    << " and feel power surge through you!\n"
                    << "Max health is now " << player->GetMaxHealth()
                    << ", current health is " << player->GetHealth() << ".\n";
                player->RemoveItemFromInventory(it);
                causeTurn = true;
                return;
            }
            std::cout << "You can't use " << itemName << " like that.\n";
            causeTurn = true;
            return;
        }
        std::cout << "Usage: use <item> [in <target>]\n";
        };

    // Ejecutar
    tokens.erase(tokens.begin());
    auto it = cmdMap.find(verb);
    if (it != cmdMap.end()) {
        it->second(tokens);
    }
    else {
        std::cout << "I don't understand " << verb << "\n";
    }

    // Si causeTurn => UpdateEnemies
    if (causeTurn) {
        UpdateEnemies();
    }
}

// Llamado tras cada turno
void World::UpdateEnemies() {
    for (size_t i = 0; i < enemies.size(); i++) {
        enemies[i]->Update(player);
    }
}
