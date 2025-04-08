#include "bag.h"
#include <iostream>
#include <algorithm>

Bag::Bag(const std::string& name, const std::string& description, int capacity, const std::string& direction, Entity* parent)
    : Item(name, description, direction, parent), capacity(capacity)
{
}

void Bag::Use() {
    std::cout << "Opening " << name << "...\n";
    ListContents();
}

bool Bag::AddItem(Item* item) {
    if (items.size() < static_cast<size_t>(capacity)) {
        items.push_back(item);
        std::cout << "Added " << item->name << " to " << name << ".\n";
        return true;
    }
    std::cout << name << " is full.\n";
    return false;
}

bool Bag::RemoveItem(const std::string& itemName) {
    auto it = std::find_if(items.begin(), items.end(), [&](Item* i) { return i->name == itemName; });
    if (it != items.end()) {
        items.erase(it);
        std::cout << "Removed " << itemName << " from " << name << ".\n";
        return true;
    }
    std::cout << itemName << " not found in " << name << ".\n";
    return false;
}

void Bag::ListContents() const {
    if (items.empty()) {
        std::cout << name << " is empty.\n";
        return;
    }
    for (auto i : items) {
        std::cout << " - " << i->name << "\n";
    }
}

int Bag::GetRemainingCapacity() const {
    return capacity - static_cast<int>(items.size());
}
