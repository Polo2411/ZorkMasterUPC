#include "exit.h"
#include <iostream>

Exit::Exit(const std::string& name, const std::string& description, Room* src, Room* dest)
    : Entity(name, description), source(src), destination(dest), state(OPEN)
{
    type = EXIT;
    if (source)
        source->children.push_back(this);
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
    std::cout << "You open " << name << "\n";
}

void Exit::Close() {
    state = CLOSED;
    std::cout << "You close " << name << "\n";
}

void Exit::Lock() {
    state = LOCKED;
    std::cout << "You lock " << name << "\n";
}
