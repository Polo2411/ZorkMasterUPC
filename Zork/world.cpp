#include "world.h"
#include "sword.h"
#include "bag.h"
#include "health_potion.h"
#include "exit.h"
#include "key.h"
#include "item.h"
#include "enemy.h"        
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

// ------------------------------------
// tokenize: convertir la entrada a minúsculas
// ------------------------------------
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

// ------------------------------------
// Constructor: "Devil May Zork" scenario
// ------------------------------------
World::World()
{
    isGameOver = false;
    std::cout << "Welcome to Devil May Zork!\n"
        << "You are Dante, the legendary devil hunter and son of Sparda.\n"
        << "Your mission is to rid this cursed castle of demons...\n"
        << "Your brother, Vergil, awaits in the deepest chamber.\n"
        << "Defeat him to fulfill your destiny!\n\n";

    // ================
    // MAIN HALL
    // ================
    Room* mainHall = new Room(
        "Main Hall",
        "A grandiose entrance hall with towering pillars. The air crackles with demonic energy."
    );

    // Items en Main Hall
    Sword* sword = new Sword(
        "DevilSword",
        "A broadsword gleaming with faint demonic energy.",
        20,
        "south",
        mainHall
    );
    mainHall->AddEntity("south", sword);

    Bag* bag = new Bag(
        "HunterBag",
        "A rugged leather bag with enough space for multiple items.",
        5,
        "north",
        mainHall
    );
    mainHall->AddEntity("north", bag);

    // Salidas:
    //  - West => Devil Kitchen (abierta)
    //  - East => Long Hallway (cerrada)

    // ================
    // DEVIL KITCHEN (west of Main Hall)
    // ================
    Room* devilKitchen = new Room(
        "Devil Kitchen",
        "A decrepit kitchen that reeks of sulfur. Broken utensils lie everywhere."
    );

    Exit* kitchenExit = new Exit(
        "KitchenDoor",
        "A battered wooden door leading to the Devil Kitchen.",
        mainHall,
        devilKitchen
    );
    kitchenExit->SetState(OPEN);
    mainHall->AddEntity("west", kitchenExit);
    devilKitchen->AddEntity("east", kitchenExit);

    // Demon en la Devil Kitchen
    Demon* kitchenDemon = new Demon(
        "KitchenDemon",
        "A lesser demon prowling around the kitchen.",
        devilKitchen
    );
    enemies.push_back(kitchenDemon);

    // Items en Devil Kitchen
    Key* kitchenKey = new Key(
        "KeyDevilHallway",
        "A rusty key. Might fit the locked hallway door in the Main Hall.",
        "south",
        /* opensRoom= */ nullptr,
        devilKitchen
    );
    devilKitchen->AddEntity("south", kitchenKey);

    HealthPotion* kitchenPotion = new HealthPotion(
        "KitchenHealthPotion",
        "A pungent vial that restores health.",
        50,
        "north",
        devilKitchen
    );
    devilKitchen->AddEntity("north", kitchenPotion);

    // ================
    // LONG HALLWAY (east of Main Hall, locked)
    // ================
    Room* longHallway = new Room(
        "Long Hallway",
        "A long corridor lined with ominous portraits. The atmosphere feels tense."
    );

    Exit* hallwayExit = new Exit(
        "LongHallwayDoor",
        "A heavy iron door leading to the Long Hallway.",
        mainHall,
        longHallway
    );
    hallwayExit->SetState(CLOSED);

    // Vinculamos la key del Kitchen para abrir este pasaje
    kitchenKey->SetOpensRoom(longHallway);

    mainHall->AddEntity("east", hallwayExit);
    longHallway->AddEntity("west", hallwayExit);

    // Demon en Long Hallway
    Demon* hallwayDemon = new Demon(
        "HallwayDemon",
        "A lurking demon patrolling the corridor.",
        longHallway
    );
    enemies.push_back(hallwayDemon);

    // Items en Long Hallway
    Bullet* hallwayBullet = new Bullet(
        "HallwayBullet",
        "A single bullet for a gun.",
        "west",
        longHallway
    );
    longHallway->AddEntity("west", hallwayBullet);

    Gun* hallwayGun = new Gun(
        "DevilGun",
        "An old revolver engraved with demonic markings.",
        30,
        "west",
        longHallway
    );
    longHallway->AddEntity("west", hallwayGun);

    Upgrader* hallwayUp = new Upgrader(
        "ArcaneUpgrader",
        "A small arcane device that can boost a weapon's damage by +10.",
        "south",
        longHallway
    );
    longHallway->AddEntity("south", hallwayUp);

    // Salidas en Long Hallway => north (cerrada), east (abierta)

    // ================
    // DEVIL BATHROOM (east of Long Hallway, open)
    // ================
    Room* devilBathroom = new Room(
        "Devil Bathroom",
        "A foul restroom with shattered mirrors and demonic graffiti on the walls."
    );
    Exit* bathroomExit = new Exit(
        "BathroomDoor",
        "A cracked door leading to a horrifying bathroom.",
        longHallway,
        devilBathroom
    );
    bathroomExit->SetState(OPEN);
    longHallway->AddEntity("east", bathroomExit);
    devilBathroom->AddEntity("west", bathroomExit);

    Key* bathroomKey = new Key(
        "KeyKnightChambers",
        "A silver key carved with knightly motifs.",
        "north",
        nullptr,
        devilBathroom
    );
    devilBathroom->AddEntity("north", bathroomKey);

    VitalityPotion* bathroomVP = new VitalityPotion(
        "BathroomVitalityPotion",
        "Increases your max HP by 20 and heals 20 HP.",
        20,
        20,
        "east",
        devilBathroom
    );
    devilBathroom->AddEntity("east", bathroomVP);

    // ================
    // KNIGHT'S CHAMBERS (north from Long Hallway)
    // ================
    Room* knightsChambers = new Room(
        "Knight's Chambers",
        "An imposing chamber guarded by ghostly suits of armor. A grim aura hangs in the air."
    );
    Exit* knightsExit = new Exit(
        "KnightDoor",
        "A fortified gate leading to the Knight's Chambers.",
        longHallway,
        knightsChambers
    );
    knightsExit->SetState(CLOSED);
    bathroomKey->SetOpensRoom(knightsChambers);

    longHallway->AddEntity("north", knightsExit);
    knightsChambers->AddEntity("south", knightsExit);

    DemonKnight* knight = new DemonKnight(
        "DevilKnight",
        "A formidable demonic knight in black armor.",
        knightsChambers
    );
    enemies.push_back(knight);

    Bullet* kcBulletN = new Bullet(
        "KnightBulletN",
        "A single bullet for a gun.",
        "north",
        knightsChambers
    );
    knightsChambers->AddEntity("north", kcBulletN);

    Bullet* kcBulletS = new Bullet(
        "KnightBulletS",
        "Another bullet rolling on the floor.",
        "south",
        knightsChambers
    );
    knightsChambers->AddEntity("south", kcBulletS);

    Upgrader* kcUp = new Upgrader(
        "KnightUpgrader",
        "Infused with greater power for forging mightier weapons.",
        "east",
        knightsChambers
    );
    knightsChambers->AddEntity("east", kcUp);

    HealthPotion* kcHP = new HealthPotion(
        "KnightHealthPotion",
        "A potent healing brew.",
        50,
        "west",
        knightsChambers
    );
    knightsChambers->AddEntity("west", kcHP);

    // ================
    // DEVIL GUEST ROOM (east from Devil Kitchen)
    // ================
    Room* devilGuestRoom = new Room(
        "Devil Guest Room",
        "A dusty guest room reeking of demonic presence."
    );
    Exit* guestExit = new Exit(
        "GuestDoor",
        "A battered door leading to the Guest Room.",
        devilKitchen,
        devilGuestRoom
    );
    guestExit->SetState(OPEN);
    devilKitchen->AddEntity("east", guestExit);
    devilGuestRoom->AddEntity("west", guestExit);

    // Items en Devil Guest Room
    VitalityPotion* guestVP = new VitalityPotion(
        "GuestVitalityPotion",
        "Augments your maximum life by 20 and heals 20 HP.",
        20,
        20,
        "north",
        devilGuestRoom
    );
    devilGuestRoom->AddEntity("north", guestVP);

    Bullet* guestBullet = new Bullet(
        "GuestBullet",
        "A single bullet left behind.",
        "north",
        devilGuestRoom
    );
    devilGuestRoom->AddEntity("north", guestBullet);

    HealthPotion* guestHP = new HealthPotion(
        "GuestHealthPotion",
        "Heals you quite a bit.",
        50,
        "west",
        devilGuestRoom
    );
    devilGuestRoom->AddEntity("west", guestHP);

    Key* guestKey = new Key(
        "KeyDeathHall",
        "An ornate key for a door deeper in the castle.",
        "south",
        nullptr,
        devilGuestRoom
    );
    devilGuestRoom->AddEntity("south", guestKey);

    // ================
    // DEATH HALL (north from Devil Guest Room)
    // ================
    Room* deathHall = new Room(
        "Death Hall",
        "A grim chamber filled with tortured wails. Evil saturates the walls."
    );
    Exit* deathHallExit = new Exit(
        "DeathHallDoor",
        "A massive door leading to the Death Hall.",
        devilGuestRoom,
        deathHall
    );
    deathHallExit->SetState(CLOSED);
    guestKey->SetOpensRoom(deathHall);

    devilGuestRoom->AddEntity("north", deathHallExit);
    deathHall->AddEntity("south", deathHallExit);

    Demon* dhDemon = new Demon(
        "DeathHallDemon",
        "A hulking beast prowling the Death Hall.",
        deathHall
    );
    enemies.push_back(dhDemon);

    DemonKnight* dhKnight = new DemonKnight(
        "DeathHallKnight",
        "A grim champion overshadowing lesser fiends.",
        deathHall
    );
    enemies.push_back(dhKnight);

    HealthPotion* dhHP = new HealthPotion(
        "DeathHallHealthPotion",
        "Another vile brew that restores health.",
        50,
        "south",
        deathHall
    );
    deathHall->AddEntity("south", dhHP);

    Key* dhKey = new Key(
        "KeyKingsRoom",
        "The final key, etched with regal inscriptions.",
        "south",
        nullptr,
        deathHall
    );
    deathHall->AddEntity("south", dhKey);

    Bullet* dhBulletE = new Bullet(
        "RoyalBulletE",
        "A bullet with a faint royal crest.",
        "east",
        deathHall
    );
    deathHall->AddEntity("east", dhBulletE);

    VitalityPotion* dhVP = new VitalityPotion(
        "RoyalVitalityPotion",
        "Enhances your maximum life by 20 and heals 20 HP.",
        20,
        20,
        "north",
        deathHall
    );
    deathHall->AddEntity("north", dhVP);

    Bullet* dhBulletW = new Bullet(
        "LostBulletW",
        "A bullet hidden behind a broken statue.",
        "west",
        deathHall
    );
    deathHall->AddEntity("west", dhBulletW);

    Upgrader* dhUp = new Upgrader(
        "FinalUpgrader",
        "A final device of augmentation, brimming with malevolent energy.",
        "west",
        deathHall
    );
    deathHall->AddEntity("west", dhUp);

    // ================
    // KING'S ROOM (north from Death Hall)
    // ================
    Room* kingsRoom = new Room(
        "King's Room",
        "An opulent throne room corrupted by demonic influences."
    );
    Exit* kingsExit = new Exit(
        "KingsRoomDoor",
        "A grand door leading to the King's Room.",
        deathHall,
        kingsRoom
    );
    kingsExit->SetState(CLOSED);
    dhKey->SetOpensRoom(kingsRoom);

    deathHall->AddEntity("north", kingsExit);
    kingsRoom->AddEntity("south", kingsExit);

    // Vergil
    Vergil* vergil = new Vergil(
        "Vergil",
        "Your brother, a powerful demon swordsman, waiting in the heart of Mundus's castle.",
        kingsRoom
    );
    enemies.push_back(vergil);

    // Agregamos las salas al vector
    rooms.push_back(mainHall);
    rooms.push_back(devilKitchen);
    rooms.push_back(longHallway);
    rooms.push_back(devilBathroom);
    rooms.push_back(knightsChambers);
    rooms.push_back(devilGuestRoom);
    rooms.push_back(deathHall);
    rooms.push_back(kingsRoom);

    // Por último, creamos al player "Dante"
    player = new Player(
        "Dante",
        "The legendary devil hunter, son of Sparda.",
        mainHall
    );
}

// ------------------------------------
// Destructor
// ------------------------------------
World::~World() {
    for (auto r : rooms) {
        delete r;
    }
    for (auto e : enemies) {
        delete e;
    }
    delete player;
}

// ------------------------------------
// Comparar ignorando mayúsculas
// ------------------------------------
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

// ------------------------------------
// Buscar Bag
// ------------------------------------
Bag* World::findBag(const std::string& containerName) {
    auto inv = player->GetInventory();
    for (auto i : inv) {
        if (equalIgnoreCase(i->name, containerName)) {
            return dynamic_cast<Bag*>(i);
        }
    }
    auto ents = player->GetCurrentRoom()->GetEntities(player->GetPlayerDirection());
    for (auto e : ents) {
        if (e->type == ITEM && equalIgnoreCase(e->name, containerName)) {
            return dynamic_cast<Bag*>(e);
        }
    }
    return nullptr;
}

// ------------------------------------
// Buscar Exit
// ------------------------------------
Exit* World::findExit(const std::string& exitName) {
    auto ents = player->GetCurrentRoom()->GetEntities(player->GetPlayerDirection());
    for (auto e : ents) {
        if (e->type == EXIT && equalIgnoreCase(e->name, exitName)) {
            return dynamic_cast<Exit*>(e);
        }
    }
    return nullptr;
}

// ------------------------------------
// Buscar Item
// ------------------------------------
Item* World::findItem(const std::string& itemName) {
    auto inv = player->GetInventory();
    for (auto i : inv) {
        if (equalIgnoreCase(i->name, itemName)) {
            return i;
        }
    }
    auto ents = player->GetCurrentRoom()->GetEntities(player->GetPlayerDirection());
    for (auto e : ents) {
        if (e->type == ITEM && equalIgnoreCase(e->name, itemName)) {
            return dynamic_cast<Item*>(e);
        }
    }
    return nullptr;
}

// ------------------------------------
// openExit / closeExit
// ------------------------------------
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
    for (auto i : inv) {
        if (auto realKey = dynamic_cast<Key*>(i)) {
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
    for (auto i : inv) {
        if (auto realKey = dynamic_cast<Key*>(i)) {
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

// ------------------------------------
// Run (bucle principal)
// ------------------------------------
void World::Run() {
    std::cout << "Welcome to Zork\n";
    player->GetCurrentRoom()->Look();

    while (!isGameOver) {
        std::cout << "\n> ";
        std::string command;
        std::getline(std::cin, command);
        if (command == "quit") {
            break;
        }
        ProcessCommand(command);
    }
    std::cout << "Exiting game...\n";
}

// ------------------------------------
// ProcessCommand
// ------------------------------------
void World::ProcessCommand(const std::string& command) {
    auto tokens = tokenize(command);
    if (tokens.empty()) {
        std::cout << "Please type a command.\n";
        return;
    }

    bool causeTurn = false;  // Indica si se gasta turno
    std::string verb = tokens[0];

    // Bloqueo si bag abierto en caso de move, exit, drop, attack, use, shoot
    // (transform NO gasta turno, y la idea es permitir hacerlo sin bag abierto)
    if (player->HasOpenBag()) {
        if (verb == "move" || verb == "exit" || verb == "drop"
            || verb == "attack" || verb == "use" || verb == "shoot")
        {
            std::cout << "You have an open bag. Close it before performing that action.\n";
            return;
        }
    }

    // Mapa de comandos
    std::map<std::string, std::function<void(const std::vector<std::string>&)>> cmdMap;

    // 1) LOOK
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

    // 2) MOVE
    cmdMap["move"] = [this, &causeTurn](auto args) {
        if (args.empty()) {
            std::cout << "Move where? (north, south, east, west, center)\n";
            return;
        }
        player->Move(args[0]);
        causeTurn = true;
        };

    // 3) EXIT
    cmdMap["exit"] = [this, &causeTurn](auto args) {
        if (args.empty()) {
            std::cout << "Exit which direction?\n";
            return;
        }
        player->ExitRoom(args[0]);
        causeTurn = true;
        };

    // 4) TAKE
    cmdMap["take"] = [this](auto args) {
        if (args.empty()) {
            std::cout << "Take what?\n";
            return;
        }
        if (args.size() == 1) {
            std::string itemName = args[0];
            // Manejo especial bullet
            if (equalIgnoreCase(itemName, "bullet")) {
                Item* bulletItem = findItem("bullet");
                if (!bulletItem) {
                    std::cout << "No bullet here.\n";
                    return;
                }
                Bullet* b = dynamic_cast<Bullet*>(bulletItem);
                if (!b) {
                    std::cout << "That isn't a bullet.\n";
                    return;
                }
                // Ver si hay Gun
                bool hasGun = false;
                Gun* gunPtr = nullptr;
                auto inv = player->GetInventory();
                for (auto i : inv) {
                    if (auto g = dynamic_cast<Gun*>(i)) {
                        hasGun = true;
                        gunPtr = g;
                        break;
                    }
                }
                if (!hasGun) {
                    std::cout << "You have no gun to store bullets in.\n";
                    return;
                }
                // +1 ammo, remover bullet de la sala
                gunPtr->AddAmmo(1);
                player->GetCurrentRoom()->RemoveEntity(player->GetPlayerDirection(), b);
                std::cout << "You add a bullet to your gun. Ammo is now "
                    << gunPtr->GetAmmoCount() << ".\n";
                return;
            }
            // normal take
            player->TakeItem(itemName);
            return;
        }
        // "take x from y"
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

    // 5) DROP
    cmdMap["drop"] = [this](auto args) {
        if (args.empty()) {
            std::cout << "Drop what?\n";
            return;
        }
        player->DropItem(args[0]);
        };

    // 6) STATUS
    cmdMap["status"] = [this](auto args) {
        player->Status();
        };

    // 7) OPEN
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

    // 8) CLOSE
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

    // 9) SAVE
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

    // 10) ATTACK
    cmdMap["attack"] = [this, &causeTurn](auto args) {
        if (args.empty()) {
            std::cout << "Attack who?\n";
            return;
        }
        std::string enemyName = args[0];
        Room* curr = player->GetCurrentRoom();
        Enemy* targetEnemy = nullptr;
        for (auto ent : curr->children) {
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

    // 11) SHOOT
    cmdMap["shoot"] = [this, &causeTurn](auto args) {
        if (args.empty()) {
            std::cout << "Shoot who?\n";
            return;
        }
        std::string enemyName = args[0];
        Room* curr = player->GetCurrentRoom();
        Enemy* targetEnemy = nullptr;
        for (auto ent : curr->children) {
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
        player->ShootEnemy(targetEnemy);
        causeTurn = true;
        };

    // 12) USE
    cmdMap["use"] = [this, &causeTurn](auto args) {
        if (args.empty()) {
            std::cout << "Use what?\n";
            return;
        }
        // "use x in y"
        if (args.size() == 3 && args[1] == "in") {
            std::string itemName = args[0];
            std::string targetName = args[2];
            Item* it = player->FindItemInInventory(itemName);
            if (!it) {
                std::cout << "You don't have " << itemName << " in your inventory.\n";
                return;
            }
            // Upgrader
            if (auto up = dynamic_cast<Upgrader*>(it)) {
                Item* targ = player->FindItemInInventory(targetName);
                if (!targ) {
                    std::cout << "You don't have " << targetName << " in your inventory.\n";
                    return;
                }
                // Sword
                if (auto sw = dynamic_cast<Sword*>(targ)) {
                    sw->SetDamage(sw->GetDamage() + 10);
                    std::cout << "You upgrade your sword! Damage is now "
                        << sw->GetDamage() << ".\n";
                    player->RemoveItemFromInventory(it);
                    return;
                }
                // Gun
                if (auto gun = dynamic_cast<Gun*>(targ)) {
                    gun->SetDamage(gun->GetDamage() + 10);
                    std::cout << "You upgrade your gun! Damage is now "
                        << gun->GetDamage() << ".\n";
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
        // "use x"
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
            // Otro item => no se puede
            std::cout << "You can't use " << itemName << " like that.\n";
            causeTurn = true;
            return;
        }
        std::cout << "Usage: use <item> [in <target>]\n";
        };

    // 13) TRANSFORM (Devil Trigger) => no gasta turno
    cmdMap["transform"] = [this](auto args) {
        // Comprobar que no haya bag abierto => se hace antes
        // "transform" no gasta turno => causeTurn = false
        if (player->IsInDevilTrigger()) {
            std::cout << "You are already transformed!\n";
            return;
        }
        // Si no => activamos
        player->ActivateDevilTrigger();
        // NO set causeTurn = true, no gasta turno
        };

    // -------------------------------------------
    // Ejecutar el comando
    // -------------------------------------------
    tokens.erase(tokens.begin());
    auto itCmd = cmdMap.find(verb);
    if (itCmd != cmdMap.end()) {
        itCmd->second(tokens);
    }
    else {
        std::cout << "I don't understand " << verb << "\n";
    }

    // -------------------------------------------
    // Si causeTurn => onTurnPassed y UpdateEnemies
    // -------------------------------------------
    if (causeTurn) {
        // Jugador reduce devilTriggerRounds si está activado
        player->OnTurnPassed();

        // Enemigos actualizan su IA
        UpdateEnemies();
        CheckGameState();
    }
}

// ------------------------------------
// UpdateEnemies: IA
// ------------------------------------
void World::UpdateEnemies() {
    for (auto e : enemies) {
        e->Update(player);
    }
}

void World::CheckGameState() {
    // 1) Si Dante murió => game over
    if (!player->IsAlive()) {
        std::cout << "You have been slain... Game Over!\n";
        isGameOver = true;
        return;
    }
    // 2) Comprobar si Vergil u otro enemigo principal (boss) fue vencido
    //    (por ejemplo, buscas un enemigo con nombre "Vergil" en 'enemies')
    bool vergilDefeated = false;
    for (Enemy* e : enemies) {
        // si es Vergil
        if (e->name == "Vergil") {
            if (!e->IsAlive()) {
                vergilDefeated = true;
                break;
            }
        }
    }
    if (vergilDefeated) {
        std::cout << "Vergil collapses, swearing vengeance as he disappears into the shadows.\n"
            << "Congratulations! You have freed the castle of Mundus's lingering threat!\n"
            << "Game Over - You Win!\n";
        isGameOver = true;
        return;
    }
}
