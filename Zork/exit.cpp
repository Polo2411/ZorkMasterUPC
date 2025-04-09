#include "exit.h"
#include <iostream>
#include <cctype>

Exit::Exit(const std::string& name, const std::string& description, Room* src, Room* dest)
    : Entity(name, description), source(src), destination(dest), state(CLOSED) // Inicialmente cerrada.
{
    type = EXIT;
    // Se asume que la exit se agrega manualmente a la Room.
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
    // La llave es válida si su opensRoom coincide con el destino.
    if (key && key->GetOpensRoom() == destination) {
        state = OPEN;
        std::cout << "You use " << key->name << " to open " << name << ".\n";
    }
    else {
        std::cout << "The key doesn't work on " << name << ".\n";
    }
}

void Exit::CloseWithKey(const Key* key) {
    if (key && key->GetOpensRoom() == destination) {
        state = CLOSED;
        std::cout << "You use " << key->name << " to close " << name << ".\n";
    }
    else {
        std::cout << "The key doesn't work on " << name << ".\n";
    }
}
