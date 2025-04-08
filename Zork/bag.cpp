#include "bag.h"
#include <iostream>
#include <algorithm>

Bag::Bag(const std::string& name, const std::string& description, int capacity, Entity* parent)
    : Item(name, description, parent), capacity(capacity)
{
}

void Bag::Use() {
    std::cout << "You open the " << name << " and see:\n";
    ListContents();
}

bool Bag::AddItem(Item* item) {
    if (items.size() < static_cast<size_t>(capacity)) {
        items.push_back(item);
        std::cout << "Item " << item->name << " added to " << name << ".\n";
        return true;
    }
    std::cout << name << " is full.\n";
    return false;
}

bool Bag::RemoveItem(const std::string& itemName) {
    auto it = std::find_if(items.begin(), items.end(), [&](Item* i) { return i->name == itemName; });
    if (it != items.end()) {
        items.erase(it);
        std::cout << "Item " << itemName << " removed from " << name << ".\n";
        return true;
    }
    std::cout << "Item " << itemName << " not found in " << name << ".\n";
    return false;
}

void Bag::ListContents() const {
    if (items.empty()) {
        std::cout << "The bag is empty.\n";
        return;
    }
    for (auto item : items) {
        std::cout << " - " << item->name << "\n";
    }
}

int Bag::GetRemainingCapacity() const {
    return capacity - static_cast<int>(items.size());
}
