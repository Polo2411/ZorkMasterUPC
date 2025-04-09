#include "world.h"
#include "sword.h"
#include "bag.h"
#include "health_potion.h"
#include "exit.h"
#include "string_utils.h"
#include <iostream>
#include <sstream>
#include <map>
#include <functional>
#include <vector>


std::vector<std::string> tokenize(const std::string& input) {
    std::istringstream iss(input);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
        // Convertir token a minúsculas
        token = toLower(token);
        tokens.push_back(token);
    }
    return tokens;
}


World::World() {
    Room* roomOne = new Room(
        "RoomOne",
        "You see a Sword to the east, a Bag to the south, and an Exit to the north."
        "The center is fairly empty."
    );
    Room* roomTwo = new Room(
        "RoomTwo",
        "A second room with a HealthPotion to the east. The center is also empty."
    );

    // Creamos los ítems, pero NO se añaden solos a la room, hay que hacerlo manual
    Sword* sword = new Sword("Sword", "A shiny sword on the floor.", 20, "east", roomOne);
    roomOne->AddEntity("east", sword);

    Bag* bag = new Bag("Bag", "A sturdy leather bag.", 5, "south", roomOne);
    roomOne->AddEntity("south", bag);

    // Exit en roomOne -> north
    Exit* exitOne = new Exit("Exit", "A passage leading to the second room.", roomOne, roomTwo);
    exitOne->SetState(OPEN);
    roomOne->AddEntity("north", exitOne);

    // En roomTwo
    HealthPotion* potion = new HealthPotion("HealthPotion", "Heals you quite a bit.", 50, "east", roomTwo);
    roomTwo->AddEntity("east", potion);

    // Exit de roomTwo -> north para volver
    Exit* exitTwo = new Exit("Exit", "A way back to RoomOne.", roomTwo, roomOne);
    exitTwo->SetState(OPEN);
    roomTwo->AddEntity("north", exitTwo);

    rooms.push_back(roomOne);
    rooms.push_back(roomTwo);

    // Player en roomOne
    player = new Player("Player", "An intrepid adventurer.", roomOne);
}

World::~World() {
    for (auto r : rooms) {
        delete r;
    }
    delete player;
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
    }
}

void World::ProcessCommand(const std::string& command) {
    auto tokens = tokenize(command);
    if (tokens.empty()) {
        std::cout << "Please type a command.\n";
        return;
    }

    std::map<std::string, std::function<void(const std::vector<std::string>&)>> cmdMap;

    cmdMap["look"] = [this](auto args) {
        player->GetCurrentRoom()->Look();
        };

    cmdMap["move"] = [this](auto args) {
        if (args.empty()) {
            std::cout << "Move where? (north, south, east, west, center)\n";
            return;
        }
        player->Move(args[0]);
        };

    cmdMap["exit"] = [this](auto args) {
        if (args.empty()) {
            std::cout << "Exit which direction?\n";
            return;
        }
        player->ExitRoom(args[0]);
        };

    cmdMap["take"] = [this](auto args) {
        if (args.empty()) {
            std::cout << "Take what?\n";
            return;
        }
        player->TakeItem(args[0]);
        };

    cmdMap["drop"] = [this](auto args) {
        if (args.empty()) {
            std::cout << "Drop what?\n";
            return;
        }
        player->DropItem(args[0]);
        };

    cmdMap["status"] = [this](auto args) {
        player->Status();
        };

    std::string verb = tokens[0];
    tokens.erase(tokens.begin());

    auto it = cmdMap.find(verb);
    if (it != cmdMap.end()) {
        it->second(tokens);
    }
    else {
        std::cout << "I don't understand " << verb << "\n";
    }
}
