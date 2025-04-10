// exit.cpp
#include "exit.h"
#include <iostream>
#include <cctype>

Exit::Exit(const std::string& name, const std::string& description,
    Room* src, Room* dest)
    : Entity(name, description), source(src), destination(dest), state(CLOSED)
{
    type = EXIT;
}

Room* Exit::GetSource() const {
    return source;
}

Room* Exit::GetDestination() const {
    return destination;
}

ExitState Exit::GetState() const {
    return state;
}

void Exit::SetState(ExitState newState) {
    state = newState;
}

// Retorna a dónde se viaja si el jugador está en 'current'
Room* Exit::GetDestinationFor(Room* current) const {
    if (current == source) {
        return destination;
    }
    if (current == destination) {
        return source;
    }
    // Si no coincide con source/destination, no lleva a ningún lado.
    return nullptr;
}

void Exit::Open() {
    state = OPEN;
    std::cout << "You open " << name << ".\n";
}

void Exit::Close() {
    state = CLOSED;
    std::cout << "You close " << name << ".\n";
}

void Exit::Lock() {
    state = LOCKED;
    std::cout << "You lock " << name << ".\n";
}

void Exit::OpenWithKey(const Key* key) {
    if (!key) {
        std::cout << "You don't have a valid key.\n";
        return;
    }
    // Si la Key abre la 'destination'
    if (key->GetOpensRoom() == destination) {
        state = OPEN;
        std::cout << "You use " << key->name << " to open " << name << ".\n";
    }
    else {
        std::cout << "The key doesn't work on " << name << ".\n";
    }
}

void Exit::CloseWithKey(const Key* key) {
    if (!key) {
        std::cout << "You don't have a valid key.\n";
        return;
    }
    if (key->GetOpensRoom() == destination) {
        state = CLOSED;
        std::cout << "You use " << key->name << " to close " << name << ".\n";
    }
    else {
        std::cout << "The key doesn't work on " << name << ".\n";
    }
}
