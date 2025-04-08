#include "world.h"

#include "sword.h"
#include "key.h"
#include "bag.h"
#include "health_potion.h"
#include "exit.h"
#include "entity.h"

#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <functional>

// Helper para dividir la cadena de comando en tokens
std::vector<std::string> tokenize(const std::string& input) {
    std::istringstream iss(input);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

// Constructor: creamos las habitaciones y colocamos los items
World::World() {
    // Creamos la primera habitación
    Room* roomOne = new Room("RoomOne",
        "You are in a dimly lit room. There's a sword to the south, a bag to the north, and a key to the west. "
        "An old door stands to the east.",
        nullptr);

    // Creamos la segunda habitación
    Room* roomTwo = new Room("RoomTwo",
        "A quieter chamber. At the north side, you see a health potion on a small pedestal.",
        nullptr);

    // Creamos los items en la primera habitación
    // Sword al sur
    Sword* sword = new Sword("Sword", "A gleaming sword on the floor.", 20, "south", roomOne);
    // Bag al norte
    Bag* bag = new Bag("Bag", "A sturdy bag with limited capacity.", 5, "north", roomOne);
    // Key al oeste
    Key* key = new Key("Key", "A small rusty key.", "west", nullptr, roomOne);

    // Creamos la puerta (Exit) al este de la primera habitación, que lleva a la segunda.
    Exit* door = new Exit("Door", "A locked wooden door with iron hinges.", roomOne, roomTwo);
    door->SetState(LOCKED); // Puerta cerrada inicialmente.

    // En la segunda habitación, ponemos una health_potion al norte
    HealthPotion* potion = new HealthPotion("Health", "A potion that restores health.", 50, "north", roomTwo);

    // Guardamos las habitaciones en el vector para eliminarlas luego
    rooms.push_back(roomOne);
    rooms.push_back(roomTwo);

    // Creamos el jugador empezando en la primera habitación
    player = new Player("Player", "A brave adventurer.", roomOne);
}

// Destructor: liberamos memoria
World::~World() {
    for (Room* r : rooms) {
        delete r;
    }
    delete player;
}

// Bucle principal de juego
void World::Run() {
    std::cout << "Welcome to Zork\n";
    player->GetCurrentRoom()->Look();

    std::string command;
    while (true) {
        std::cout << "\n> ";
        std::getline(std::cin, command);
        if (command == "exit")
            break;
        ProcessCommand(command);
    }
}

// Procesamiento de comandos usando lambdas en un mapa
void World::ProcessCommand(const std::string& command) {
    std::vector<std::string> tokens = tokenize(command);
    if (tokens.empty()) {
        std::cout << "Please enter a command.\n";
        return;
    }

    // Mapa de verbos a funciones
    std::map<std::string, std::function<void(const std::vector<std::string>&)>> commandMap;

    // Comando "look"
    commandMap["look"] = [this](const std::vector<std::string>& params) {
        player->GetCurrentRoom()->Look();
        };

    // Comando "move"
    commandMap["move"] = [this](const std::vector<std::string>& params) {
        if (params.size() < 1) {
            std::cout << "Move where?\n";
            return;
        }
        player->Move(params[0]);
        };

    // Comando "take" (requiere 2 parámetros: objeto y dirección)
    commandMap["take"] = [this](const std::vector<std::string>& params) {
        if (params.size() < 2) {
            std::cout << "Take what and from where? e.g., 'take key west'\n";
            return;
        }
        Room* current = player->GetCurrentRoom();
        std::string objectName = params[0];
        std::string direction = params[1];
        Entity* e = current->FindChild(objectName, ITEM);
        if (e) {
            // Verificamos si la dirección coincide
            Item* itemPtr = static_cast<Item*>(e);
            if (itemPtr->GetDirection() == direction) {
                // Distinguimos el tipo
                if (objectName == "Key")
                    player->PickUp(static_cast<Key*>(e));
                else if (objectName == "Sword")
                    player->PickUp(static_cast<Sword*>(e));
                else if (objectName == "Bag")
                    player->PickUp(static_cast<Bag*>(e));
                else if (objectName == "Health")
                    player->PickUp(static_cast<HealthPotion*>(e));
                else
                    player->PickUp(itemPtr);

                current->children.remove(e);
            }
            else {
                std::cout << "There is no " << objectName << " in the " << direction << " direction.\n";
            }
        }
        else {
            std::cout << "No " << objectName << " found in that direction.\n";
        }
        };

    // Comando "open" (por ejemplo, "open door")
    commandMap["open"] = [this](const std::vector<std::string>& params) {
        if (params.empty()) {
            std::cout << "Open what?\n";
            return;
        }
        if (params[0] == "door") {
            Entity* doorEnt = player->GetCurrentRoom()->FindChild("Door", EXIT);
            if (doorEnt) {
                Exit* door = dynamic_cast<Exit*>(doorEnt);
                if (door->GetState() == LOCKED) {
                    bool hasKey = false;
                    for (auto itm : player->GetInventory()) {
                        if (itm->name == "Key") {
                            hasKey = true;
                            break;
                        }
                    }
                    if (hasKey) {
                        door->Open();
                        std::cout << "You unlocked and opened the door.\n";
                    }
                    else {
                        std::cout << "The door is locked. You need a key.\n";
                    }
                }
                else {
                    std::cout << "The door is already open.\n";
                }
            }
            else {
                std::cout << "There is no door here.\n";
            }
        }
        else {
            std::cout << "I don't know how to open " << params[0] << "\n";
        }
        };

    // Comando "drop" (deja un item en la habitación)
    commandMap["drop"] = [this](const std::vector<std::string>& params) {
        if (params.empty()) {
            std::cout << "Drop what?\n";
            return;
        }
        std::string itemName = params[0];
        auto inv = player->GetInventory();
        bool found = false;
        for (auto item : inv) {
            if (item->name == itemName) {
                player->Drop(item);
                found = true;
                break;
            }
        }
        if (!found)
            std::cout << "You don't have that item.\n";
        };

    // Obtenemos el verbo
    std::string verb = tokens[0];
    // Quitamos el verbo de la lista para dejar solo los parámetros
    tokens.erase(tokens.begin());

    // Buscamos el verbo en el mapa
    auto it = commandMap.find(verb);
    if (it != commandMap.end()) {
        it->second(tokens);
    }
    else {
        std::cout << "I don't understand that command.\n";
    }
}
