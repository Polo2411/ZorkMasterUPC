#include "world.h"
#include <iostream>
#include "item.h"

World::World() {
    // Create the starting room with a paper on the floor
    startRoom = new Room("Entrance", "You are at the entrance of a mysterious building. A paper lies on the floor.", nullptr);
    // Create a paper item in the start room
    Item* paper = new Item("Paper", "A crumpled paper with a welcome message.", startRoom);
    // Create the player starting in the starting room
    player = new Player("Player", "The brave adventurer.", startRoom);
}

World::~World() {
    delete startRoom;
    delete player;
}

void World::Run() {
    std::cout << "Welcome to Zork\n";
    startRoom->Look();

    std::string command;
    while (true) {
        std::cout << "\n> ";
        std::getline(std::cin, command);
        if (command == "exit")
            break;
        ProcessCommand(command);
    }
}

void World::ProcessCommand(const std::string& command) {
    if (command == "look") {
        player->GetCurrentRoom()->Look();
    }
    else if (command == "take paper") {
        Room* currentRoom = player->GetCurrentRoom();
        Entity* e = currentRoom->FindChild("Paper", ITEM);
        if (e) {
            player->PickUp(static_cast<Item*>(e));
            currentRoom->children.remove(e);
        }
        else {
            std::cout << "Paper is not here.\n";
        }
    }
    else if (command == "move north") {
        player->Move("north");
    }
    else {
        std::cout << "I don't understand that command.\n";
    }
}
